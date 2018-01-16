#include <stdext/format.h>


namespace stdext
{
    namespace detail
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

        void format_integer(const std::function<bool (char ch)>& out, string_view fmt, uintmax_t uval, bool negative)
        {
            auto options = detail::parse_format_options(fmt);
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

            uint base;
            bool upper = false;
            switch (specifier)
            {
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

            if (options.test_any(detail::format_options::alternative_form) && base == 8)
                ++len;

            if (len > precision)
                precision = len;

            len = precision;

            if (options.test_any(detail::format_options::alternative_form) && base == 16)
                len += 2;

            if (negative || options.test_any(detail::format_options::show_sign | detail::format_options::pad_sign))
                ++len;

            if (len > width)
                width = len;

            auto put = [&](char ch)
            {
                out(ch);
                --width;
            };

            // Generate output
            if (!options.test_any(detail::format_options::left_justified))
            {
                while (width > len)
                    put(options.test_any(detail::format_options::zero_pad) ? '0' : ' ');
            }

            if (negative)
                put('-');
            else if (options.test_any(detail::format_options::show_sign))
                put('+');
            else if (options.test_any(detail::format_options::pad_sign))
                put(' ');

            if (options.test_any(detail::format_options::alternative_form) && base == 16)
            {
                put('0');
                put(upper ? 'X' : 'x');
            }

            uintmax_t test = 1;
            for (unsigned n = 0; n < precision; ++n)
                test *= base;

            for (; precision != 0; --precision)
            {
                unsigned digit = uval / test;
                uval %= test;
                put(digit < 10 ? '0' + digit : upper ? 'A' - 10 + digit : 'a' - 10 + digit);
                test /= base;
            }

            if (options.test_any(detail::format_options::left_justified))
            {
                while (width > 0)
                    put(' ');
            }
        }
    }
}

