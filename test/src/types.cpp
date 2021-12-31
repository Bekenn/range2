#include <stdext/types.h>

#include <type_traits>


// Types imported from cstddef
static_assert(std::is_same_v<stdext::byte, std::byte>);
static_assert(std::is_same_v<stdext::ptrdiff_t, std::ptrdiff_t>);
static_assert(std::is_same_v<stdext::size_t, std::size_t>);
static_assert(std::is_same_v<stdext::max_align_t, std::max_align_t>);
static_assert(std::is_same_v<stdext::nullptr_t, std::nullptr_t>);

// Types imported from cstdint
static_assert(std::is_same_v<stdext::int8_t, std::int8_t>);
static_assert(std::is_same_v<stdext::int16_t, std::int16_t>);
static_assert(std::is_same_v<stdext::int32_t, std::int32_t>);
static_assert(std::is_same_v<stdext::int64_t, std::int64_t>);
static_assert(std::is_same_v<stdext::intmax_t, std::intmax_t>);
static_assert(std::is_same_v<stdext::intptr_t, std::intptr_t>);

static_assert(std::is_same_v<stdext::uint8_t, std::uint8_t>);
static_assert(std::is_same_v<stdext::uint16_t, std::uint16_t>);
static_assert(std::is_same_v<stdext::uint32_t, std::uint32_t>);
static_assert(std::is_same_v<stdext::uint64_t, std::uint64_t>);
static_assert(std::is_same_v<stdext::uintmax_t, std::uintmax_t>);
static_assert(std::is_same_v<stdext::uintptr_t, std::uintptr_t>);
