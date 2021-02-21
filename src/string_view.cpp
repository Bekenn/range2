#include <stdext/string_view.h>
#include <stdext/scope_guard.h>

#include <cctype>


namespace stdext
{
    namespace
    {
        template <class Int>
        Int ston(string_view str, size_t* idx, int base)
        {
            using UInt = std::make_unsigned_t<Int>;
            using SInt = std::make_signed_t<Int>;

            auto p = find_if_not(str, isspace);
            if (str.is_end_pos(p))
                throw std::invalid_argument("invalid numeric string");

            bool negative = false;
            switch (str.at_pos(p))
            {
                case '-':
                    negative = true;
                    [[fallthrough]];
                case '+':
                    str.inc_pos(p);
            }

            bool valid = false;
            auto q = p;
            at_scope_success([&]
            {
                if (idx != nullptr)
                    *idx = str.distance(str.begin_pos(), q);
            });
            if (base == 0 || base == 16)
            {
                if (str.at_pos(p) == '0')
                {
                    valid = true;
                    q = str.inc_pos(p);
                    if (str.at_pos(p) == 'x' || str.at_pos(p) == 'X')
                    {
                        base = 16;
                        str.inc_pos(p);
                    }
                    else if (base == 0)
                    {
                        if (str.at_pos(p) == 'b')
                        {
                            base = 2;
                            str.inc_pos(p);
                        }
                        else
                            base = 8;
                    }
                }

                if (base == 0)
                    base = 10;
            }

            UInt value = 0;
            for (; !str.is_end_pos(p); q = str.inc_pos(p))
            {
                int c = str.at_pos(p);
                if (c >= '0' && c <= '9')
                    c -= '0';
                else if (c >= 'A' && c <= 'Z')
                    c -= 'A' - 10;
                else if (c >= 'a' && c <= 'z')
                    c -= 'a' - 10;
                else
                    break;

                if (c >= base)
                    break;

                UInt v = base * value + c;
                if (v < value)
                    throw std::out_of_range("numeric value too large");

                value = v;
                valid = true;
            }

            if (!valid)
                throw std::invalid_argument("invalid numeric string");

            SInt result = value;
            if (result < 0)
                throw std::out_of_range("numeric value too large");

            return negative ? -result : result;
        }

#if 0
        template <class Float>
        Float stox(string_view str, size_t* idx)
        {
        }
#endif
    }

    int stoi(string_view str, size_t* idx, int base)
    {
        return ston<int>(str, idx, base);
    }

    long stol(string_view str, size_t* idx, int base)
    {
        return ston<long>(str, idx, base);
    }

    unsigned long stoul(string_view str, size_t* idx, int base)
    {
        return ston<unsigned long>(str, idx, base);
    }

    long long stoll(string_view str, size_t* idx, int base)
    {
        return ston<long long>(str, idx, base);
    }

    unsigned long long stoull(string_view str, size_t* idx, int base)
    {
        return ston<unsigned long long>(str, idx, base);
    }

#if 0
    float stof(string_view str, size_t* idx)
    {
        return stox<float>(str, idx);
    }

    double stod(string_view str, size_t* idx)
    {
        return stox<double>(str, idx);
    }

    long double stold(string_view str, size_t* idx)
    {
        return stox<long double>(str, idx);
    }
#endif
}
