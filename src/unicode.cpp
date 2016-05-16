#include <stdext/unicode.h>


namespace stdext
{
    namespace
    {
        bool utf8_decode_first(uint8_t code, utfstate_t& state);
        bool utf8_validate_second(uint8_t code, utfstate_t state);
        bool utf8_decode_trailing(uint8_t code, utfstate_t& state);
    }

    std::pair<utf_result, char32_t> to_utf32(char in, utfstate_t& state)
    {
        switch (state.consumed)
        {
        case 0:
            if (!utf8_decode_first(in, state))
                return { utf_result::error, char32_t() };

            if (state.remaining == 0)
                return { utf_result::ok, state.code };

            state.consumed = 1;
            return { utf_result::partial_read, char32_t() };

        case 1:
            if (!utf8_validate_second(in, state))
                return { utf_result::error, char32_t() };

        // fall through
        default:
            if (state.remaining != 0)
            {
                if (!utf8_decode_trailing(in, state))
                    return { utf_result::error, char32_t() };

                ++state.consumed;
                if (--state.remaining != 0)
                    return { utf_result::partial_read, char32_t() };
            }
        }

        state.consumed = 0;
        return { utf_result::ok, state.code };
    }

    std::pair<utf_result, char32_t> to_utf32(char16_t in, utfstate_t& state)
    {
        switch (state.consumed)
        {
        case 0:
            if (!is_surrogate(in))
                return { utf_result::ok, in };

            if (is_trailing_surrogate(in))
                return { utf_result::error, char32_t() };

            state.code = (in & 0x03FF) + 0x40;
            state.consumed = 1;
            state.remaining = 1;
            return { utf_result::partial_read, char32_t() };

        case 1:
            if (!is_trailing_surrogate(in))
                return { utf_result::error, char32_t() };

            state.code = (state.code << 10) | (in & 0x03FF);
            state.consumed = 0;
            state.remaining = 0;
            return { utf_result::ok, state.code };
        }

        return { utf_result::error, char32_t() };
    }

    std::pair<utf_result, char> to_utf8(char32_t in, utfstate_t& state)
    {
        if (state.produced == 0)
        {
            if (in < 0x80)
                return { utf_result::ok, char(in) };

            if (in >= 0x110000 || (in < 0x10000 && is_surrogate(char16_t(in))))
                return { utf_result::error, char() };

            state.code = in;
            state.produced = 1;
            state.remaining = in < 0x800 ? 1
                : in < 0x1000 ? 2
                : 3;

            switch (state.remaining)
            {
            case 1:
                return { utf_result::partial_write, char(0xC0 | (state.code >> 6)) };
            case 2:
                return { utf_result::partial_write, char(0xE0 | (state.code >> 12)) };
            case 3:
                return { utf_result::partial_write, char(0xF0 | (state.code >> 18)) };
            }
        }

        if (--state.remaining != 0)
        {
            ++state.produced;
            return { utf_result::partial_write, char(0x80 | ((state.code >> 6 * state.remaining) & 0x3F)) };
        }

        state.produced = 0;
        return { utf_result::ok, char(0x80 | (state.remaining & 0x3F)) };
    }

    std::pair<utf_result, char16_t> to_utf16(char32_t in, utfstate_t& state)
    {
        if (state.produced == 0)
        {
            if (in < 0x10000)
            {
                if (is_surrogate(char16_t(in)))
                    return { utf_result::error, char16_t() };
                return { utf_result::ok, char16_t(in) };
            }

            if (in >= 0x110000)
                return { utf_result::error, char16_t() };

            state.code = in;
            state.produced = 1;
            state.remaining = 1;
            return { utf_result::partial_write, char16_t(0xD800 | ((state.code >> 16) - 1)) };
        }

        state.produced = 0;
        state.remaining = 0;
        return { utf_result::ok, char16_t(0xDC00 | (state.code & 0xFFFF)) };
    }

    std::pair<utf_result, char16_t> to_utf16(char in, utfstate_t& state)
    {
        if (state.produced == 0)
        {
            auto result = to_utf32(in, state);
            if (result.first != utf_result::ok)
                return { result.first, char16_t() };
            state.code = result.second;
        }

        return to_utf16(char32_t(state.code), state);
    }

    std::pair<utf_result, char> to_utf8(char16_t in, utfstate_t& state)
    {
        if (state.produced == 0)
        {
            auto result = to_utf32(in, state);
            if (result.first != utf_result::ok)
                return { result.first, char() };
            state.code = result.second;
        }

        return to_utf8(char32_t(state.code), state);
    }

    namespace
    {
        bool utf8_decode_first(uint8_t code, utfstate_t& state)
        {
            if ((code & 0x80) == 0x00)
            {
                state.code = code;
                state.remaining = 0;
            }
            else if ((code & 0xE0) == 0xC0)
            {
                state.code = code & 0x1F;
                state.remaining = 1;
            }
            else if ((code & 0xF0) == 0xE0)
            {
                state.code = code & 0x0F;
                state.remaining = 2;
            }
            else if ((code & 0xF8) == 0xF0)
            {
                state.code = code & 0x07;
                state.remaining = 3;
            }
            else
                return false;

            return true;
        }

        bool utf8_validate_second(uint8_t code, utfstate_t state)
        {
            switch (state.remaining)
            {
            case 2:
                if ((state.code == 0x00 && code < 0xA0)
                    || (state.code == 0x0D && code >= 0xA0))
                {
                    return false;
                }
                break;
            case 3:
                if ((state.code == 0x00 && code < 0x90)
                    || (state.code == 0x04 && code >= 0x90))
                {
                    return false;
                }
                break;

            default:
                break;
            }

            return true;
        }

        bool utf8_decode_trailing(uint8_t code, utfstate_t& state)
        {
            if ((code & 0xC0) != 0x80)
                return false;

            state.code <<= 6;
            state.code |= code;
            return true;
        }
    }
}
