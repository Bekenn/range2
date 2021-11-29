#include <stdext/traits.h>


// declval
static_assert(std::is_same_v<decltype(stdext::declval<int>()), int&&>);
static_assert(std::is_same_v<decltype(stdext::declval<int&>()), int&>);
static_assert(std::is_same_v<decltype(stdext::declval<int&&>()), int&&>);
static_assert(std::is_same_v<decltype(stdext::declval<void>()), void>);

// integral_constant
static_assert(std::is_same_v<decltype(stdext::integral_constant<int, 0>::value), const int>);
static_assert(stdext::integral_constant<int, 0>::value == 0);
static_assert(std::is_same_v<typename stdext::integral_constant<int, 0>::value_type, int>);
static_assert(std::is_same_v<typename stdext::integral_constant<int, 0>::type, stdext::integral_constant<int, 0>>);
static_assert(int(stdext::integral_constant<int, 0>()) == 0);
static_assert(stdext::integral_constant<int, 0>()() == 0);

// bool_constant
static_assert(std::is_same_v<stdext::bool_constant<false>, stdext::integral_constant<bool, false>>);
static_assert(std::is_same_v<decltype(stdext::bool_constant<false>::value), const bool>);
static_assert(stdext::bool_constant<false>::value == false);
static_assert(std::is_same_v<typename stdext::bool_constant<false>::value_type, bool>);
static_assert(std::is_same_v<typename stdext::bool_constant<false>::type, stdext::bool_constant<false>>);
static_assert(bool(stdext::bool_constant<false>()) == false);
static_assert(stdext::bool_constant<false>()() == false);

// false_type
static_assert(std::is_same_v<stdext::false_type, stdext::bool_constant<false>>);
static_assert(std::is_same_v<decltype(stdext::false_type::value), const bool>);
static_assert(stdext::false_type::value == false);
static_assert(std::is_same_v<typename stdext::false_type::value_type, bool>);
static_assert(std::is_same_v<typename stdext::false_type::type, stdext::false_type>);
static_assert(bool(stdext::false_type()) == false);
static_assert(stdext::false_type()() == false);

// true_type
static_assert(std::is_same_v<stdext::true_type, stdext::bool_constant<true>>);
static_assert(std::is_same_v<decltype(stdext::true_type::value), const bool>);
static_assert(stdext::true_type::value == true);
static_assert(std::is_same_v<typename stdext::true_type::value_type, bool>);
static_assert(std::is_same_v<typename stdext::true_type::type, stdext::true_type>);
static_assert(bool(stdext::true_type()) == true);
static_assert(stdext::true_type()() == true);

// error_type
// nothing to test

// error_value
// nothing to test

// is_equality_comparable
static_assert(std::is_base_of_v<stdext::true_type, stdext::is_equality_comparable<int, int>>);
static_assert(std::is_base_of_v<stdext::false_type, stdext::is_equality_comparable<int, void>>);
static_assert(std::is_base_of_v<stdext::false_type, stdext::is_equality_comparable<void, int>>);
static_assert(std::is_base_of_v<stdext::true_type, stdext::is_equality_comparable<int, short>>);

// is_equality_comparable_v
static_assert(stdext::is_equality_comparable_v<int, int>);
static_assert(!stdext::is_equality_comparable_v<int, void>);
static_assert(!stdext::is_equality_comparable_v<void, int>);
static_assert(stdext::is_equality_comparable_v<int, short>);

// preserve_const
static_assert(std::is_same_v<stdext::preserve_const<int, float>::type, float>);
static_assert(std::is_same_v<stdext::preserve_const<const int, float>::type, const float>);
static_assert(std::is_same_v<stdext::preserve_const<int, const float>::type, const float>);

// preserve_const_t
static_assert(std::is_same_v<stdext::preserve_const_t<int, float>, float>);
static_assert(std::is_same_v<stdext::preserve_const_t<const int, float>, const float>);
static_assert(std::is_same_v<stdext::preserve_const_t<int, const float>, const float>);

// equivalent_sized_type
static_assert(std::is_same_v<stdext::equivalent_sized_type<signed char>::type, std::int8_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type<unsigned char>::type, std::uint8_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type<signed short>::type, std::int16_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type<unsigned short>::type, std::uint16_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type<signed int>::type, std::int32_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type<unsigned int>::type, std::uint32_t>);
#if __SIZEOF_LONG__ == 4 || defined __LLP64__ || STDEXT_COMPILER_MSVC
static_assert(std::is_same_v<stdext::equivalent_sized_type<signed long>::type, std::int32_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type<unsigned long>::type, std::uint32_t>);
#elif __SIZEOF_LONG__ == 8 || defined __LP64__
static_assert(std::is_same_v<stdext::equivalent_sized_type<signed long>::type, std::int64_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type<unsigned long>::type, std::uint64_t>);
#else
#error Unknown sizeof(long)
#endif
static_assert(std::is_same_v<stdext::equivalent_sized_type<signed long long>::type, std::int64_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type<unsigned long long>::type, std::uint64_t>);

// equivalent_sized_type_t
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<signed char>, std::int8_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<unsigned char>, std::uint8_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<signed short>, std::int16_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<unsigned short>, std::uint16_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<signed int>, std::int32_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<unsigned int>, std::uint32_t>);
#if __SIZEOF_LONG__ == 4 || defined __LLP64__ || STDEXT_COMPILER_MSVC
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<signed long>, std::int32_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<unsigned long>, std::uint32_t>);
#elif __SIZEOF_LONG__ == 8 || defined __LP64__
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<signed long>, std::int64_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<unsigned long>, std::uint64_t>);
#else
#error Unknown sizeof(long)
#endif
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<signed long long>, std::int64_t>);
static_assert(std::is_same_v<stdext::equivalent_sized_type_t<unsigned long long>, std::uint64_t>);
