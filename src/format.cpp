#include <stdext/format.h>


namespace stdext
{
    namespace _private
    {
        flags<format_options> parse_format_options(string_view& fmt)
        {
            flags<format_options> options = format_options::none;

            for (; fmt.size() > 0; fmt.remove_prefix(1))
            {
                switch (fmt.front())
                {
                    case '-':
                        options.set(format_options::left_justified);
                        break;
                    case '+':
                        options.set(format_options::show_sign);
                        break;
                    case ' ':
                        options.set(format_options::pad_sign);
                        break;
                    case '#':
                        options.set(format_options::alternative_form);
                        break;
                    case '0':
                        options.set(format_options::zero_pad);
                        break;
                    default:
                        goto done;
                }
            }

done:
            return options;
        }

        bool write_formatted_character(const std::function<bool (char ch)>& out, string_view fmt, uintmax_t uval, format_type type)
        {
            auto options = _private::parse_format_options(fmt);

            size_t len = 0;
            unsigned width = !fmt.empty() && isdigit(fmt.front()) ? stoi(fmt, &len) : 0;
            fmt.remove_prefix(len);

            if (!fmt.empty())
                throw format_error("Invalid format string");

            if (!options.test_any(format_options::left_justified))
            {
                for (; width > 1; --width)
                {
                    if (!out(' '))
                        return false;
                }
            }

            switch (type)
            {
            case format_type::_char:
                if (!out(char(uval)))
                    return false;
                break;

#if STDEXT_HAS_C_UNICODE
            case format_type::_char16:
                {
                    char16_t s[] = { char16_t(uval), u'\0' };
                    if (!format(out, "$0", s))
                        return false;
                }
                break;

            case format_type::_char32:
                {
                    char32_t s[] = { char32_t(uval), U'\0' };
                    if (!format(out, "$0", s))
                        return false;
                }
                break;
#endif

            case format_type::_wchar:
                {
                    wchar_t s[] = { wchar_t(uval), L'\0' };
                    if (!format(out, "$0", s))
                        return false;
                }
                break;

            default:
                assert(false);
            }

            if (options.test_any(_private::format_options::left_justified))
            {
                for (; width > 1; --width)
                {
                    if (!out(' '))
                        return false;
                }
            }

            return true;
        }

        bool write_formatted_integer(const std::function<bool (char ch)>& out, string_view fmt, uintmax_t uval, format_type type)
        {
            auto options = _private::parse_format_options(fmt);
            bool negative = false;
            if (type == format_type::_signed && intmax_t(uval) < 0)
            {
                uval = -intmax_t(uval);
                negative = true;
            }

            size_t len = 0;
            unsigned width = !fmt.empty() && isdigit(fmt.front()) ? stoi(fmt, &len) : 0;
            fmt.remove_prefix(len);

            unsigned precision = 1;
            if (!fmt.empty() && fmt.front() == '.')
            {
                fmt.remove_prefix(1);
                precision = stoi(fmt, &len);
                fmt.remove_prefix(len);
            }

            char specifier = 'd';
            if (!fmt.empty())
            {
                specifier = fmt.front();
                fmt.remove_prefix(1);
            }
            if (!fmt.empty())
                throw format_error("Invalid format string");

            unsigned base;
            bool upper = false;
            switch (specifier)
            {
                case 'B':
                    upper = true;
                    [[fallthrough]];
                case 'b':
                    base = 2;
                    break;
                case 'd':
                    base = 10;
                    break;
                case 'o':
                    base = 8;
                    break;
                case 'X':
                    upper = true;
                    [[fallthrough]];
                case 'x':
                    base = 16;
                    break;
                default:
                    throw format_error("Invalid format string");
            }

            // Calculate required precision and field width
            len = 0;
            for (uintmax_t test = 1, prev = 0; prev < test && test <= uval; prev = test, test *= base)
                ++len;

            if (options.test_any(_private::format_options::alternative_form) && base == 8 && uval != 0)
                ++len;

            if (len > precision)
                precision = unsigned(len);

            len = precision;

            if (options.test_any(_private::format_options::alternative_form) && (base == 16 || base == 2))
                len += 2;

            if (negative || options.test_any(make_flags(_private::format_options::show_sign, _private::format_options::pad_sign)))
                ++len;

            if (len > width)
                width = unsigned(len);

            // Generate output
            auto put = [&](char ch)
            {
                if (!out(ch))
                    return false;
                --width;
                return true;
            };

            if (!options.test_any(_private::format_options::left_justified))
            {
                while (width > len)
                {
                    if (!put(options.test_any(_private::format_options::zero_pad) ? '0' : ' '))
                        return false;
                }
            }

            if (negative)
            {
                if (!put('-'))
                    return false;
            }
            else if (options.test_any(_private::format_options::show_sign))
            {
                if (!put('+'))
                    return false;
            }
            else if (options.test_any(_private::format_options::pad_sign))
            {
                if (!put(' '))
                    return false;
            }

            if (options.test_any(_private::format_options::alternative_form) && base == 16)
            {
                if (!put('0'))
                    return false;
                if (!put(upper ? 'X' : 'x'))
                    return false;
            }

            if (options.test_any(_private::format_options::alternative_form) && base == 2)
            {
                if (!put('0'))
                    return false;
                if (!put(upper ? 'B' : 'b'))
                    return false;
            }

            uintmax_t test = 1;
            for (unsigned n = 1; n < precision; ++n)
                test *= base;

            for (; precision != 0; --precision)
            {
                auto digit = unsigned(uval / test);
                uval %= test;
                if (!put(digit < 10 ? char('0' + digit) : upper ? char('A' + digit - 10) : char('a' + digit - 10)))
                    return false;
                test /= base;
            }

            if (options.test_any(_private::format_options::left_justified))
            {
                while (width > 0)
                {
                    if (!put(' '))
                        return false;
                }
            }

            return true;
        }

        bool format_integer(const std::function<bool (char ch)>& out, string_view fmt, uintmax_t uval, format_type type)
        {
            switch (type)
            {
                case format_type::_char:
#if STDEXT_HAS_C_UNICODE
                case format_type::_char16:
                case format_type::_char32:
#endif
                case format_type::_wchar:
                    return write_formatted_character(out, fmt, uval, type);

                case format_type::_signed:
                case format_type::_unsigned:
                    return write_formatted_integer(out, fmt, uval, type);
            }

            unreachable();
        }
    }
}
