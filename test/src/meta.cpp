#include <stdext/meta.h>

#include <stdext/preprocessor.h>


namespace test
{
    // constant
    static_assert(std::is_same_v<stdext::constant<5>::type, int>);
    static_assert(stdext::constant<5>::value == 5);
    static_assert(std::is_same_v<stdext::constant<'f'>::type, char>);
    static_assert(stdext::constant<'f'>::value == 'f');

    // list_head
    static_assert(std::is_same_v<stdext::list_head<stdext::type_list<bool, int, char>>::type, bool>);
    static_assert(std::is_same_v<stdext::list_head<stdext::type_list<int, char, bool>>::type, int>);
    static_assert(stdext::list_head<stdext::value_list<'a', 2, false>>::value == 'a');
    static_assert(stdext::list_head<stdext::value_list<2, false, 'a'>>::value == 2);

    // list_head_t
    static_assert(std::is_same_v<stdext::list_head_t<stdext::type_list<bool, int, char>>, bool>);
    static_assert(std::is_same_v<stdext::list_head_t<stdext::type_list<int, char, bool>>, int>);

    // list_head_v
    static_assert(stdext::list_head_v<stdext::value_list<'a', 2, false>> == 'a');
    static_assert(stdext::list_head_v<stdext::value_list<2, false, 'a'>> == 2);

    // list_tail
    static_assert(std::is_same_v<stdext::list_tail<stdext::type_list<bool, int, char>>::type, stdext::type_list<int, char>>);
    static_assert(std::is_same_v<stdext::list_tail<stdext::type_list<int, char, bool>>::type, stdext::type_list<char, bool>>);
    static_assert(std::is_same_v<stdext::list_tail<stdext::value_list<'a', 2, false>>::type, stdext::value_list<2, false>>);
    static_assert(std::is_same_v<stdext::list_tail<stdext::value_list<2, false, 'a'>>::type, stdext::value_list<false, 'a'>>);

    // list_tail_t
    static_assert(std::is_same_v<stdext::list_tail_t<stdext::type_list<bool, int, char>>, stdext::type_list<int, char>>);
    static_assert(std::is_same_v<stdext::list_tail_t<stdext::type_list<int, char, bool>>, stdext::type_list<char, bool>>);
    static_assert(std::is_same_v<stdext::list_tail_t<stdext::value_list<'a', 2, false>>, stdext::value_list<2, false>>);
    static_assert(std::is_same_v<stdext::list_tail_t<stdext::value_list<2, false, 'a'>>, stdext::value_list<false, 'a'>>);

    // list_length
    static_assert(stdext::list_length<stdext::type_list<int, int, char>>::value == 3);
    static_assert(stdext::list_length<stdext::type_list<float, double>>::value == 2);
    static_assert(stdext::list_length<stdext::value_list<3ull, nullptr, 'x'>>::value == 3);
    static_assert(stdext::list_length<stdext::value_list<57, L'y'>>::value == 2);

    // list_length_v
    static_assert(stdext::list_length_v<stdext::type_list<int, int, char>> == 3);
    static_assert(stdext::list_length_v<stdext::type_list<float, double>> == 2);
    static_assert(stdext::list_length_v<stdext::value_list<3ull, nullptr, 'x'>> == 3);
    static_assert(stdext::list_length_v<stdext::value_list<57, L'y'>> == 2);

    // list_element
    static_assert(std::is_same_v<stdext::list_element<stdext::type_list<int, short, double, long>, 0>::type, int>);
    static_assert(std::is_same_v<stdext::list_element<stdext::type_list<int, short, double, long>, 1>::type, short>);
    static_assert(std::is_same_v<stdext::list_element<stdext::type_list<int, short, double, long>, 2>::type, double>);
    static_assert(std::is_same_v<stdext::list_element<stdext::type_list<int, short, double, long>, 3>::type, long>);
    static_assert(stdext::list_element<stdext::value_list<'a', u'b', 4UL, std::byte(-123)>, 0>::value == 'a');
    static_assert(stdext::list_element<stdext::value_list<'a', u'b', 4UL, std::byte(-123)>, 1>::value == u'b');
    static_assert(stdext::list_element<stdext::value_list<'a', u'b', 4UL, std::byte(-123)>, 2>::value == 4UL);
    static_assert(stdext::list_element<stdext::value_list<'a', u'b', 4UL, std::byte(-123)>, 3>::value == std::byte(-123));

    // list_element_t
    static_assert(std::is_same_v<stdext::list_element_t<stdext::type_list<int, short, double, long>, 0>, int>);
    static_assert(std::is_same_v<stdext::list_element_t<stdext::type_list<int, short, double, long>, 1>, short>);
    static_assert(std::is_same_v<stdext::list_element_t<stdext::type_list<int, short, double, long>, 2>, double>);
    static_assert(std::is_same_v<stdext::list_element_t<stdext::type_list<int, short, double, long>, 3>, long>);

    // list_element_v
    static_assert(stdext::list_element_v<stdext::value_list<'a', u'b', 4UL, std::byte(-123)>, 0> == 'a');
    static_assert(stdext::list_element_v<stdext::value_list<'a', u'b', 4UL, std::byte(-123)>, 1> == u'b');
    static_assert(stdext::list_element_v<stdext::value_list<'a', u'b', 4UL, std::byte(-123)>, 2> == 4UL);
    static_assert(stdext::list_element_v<stdext::value_list<'a', u'b', 4UL, std::byte(-123)>, 3> == std::byte(-123));

    // list_index_of
    static_assert(stdext::list_index_of<stdext::type_list<int, char, double, int>, int>::value == 0);
    static_assert(stdext::list_index_of<stdext::type_list<int, char, double, int>, double>::value == 2);
    static_assert(stdext::list_index_of<stdext::type_list<int, char, double, int>, void>::value == 4);
    static_assert(stdext::list_index_of<stdext::value_list<25, U'x', 127>, stdext::constant<25>>::value == 0);
    static_assert(stdext::list_index_of<stdext::value_list<25, U'x', 127>, stdext::constant<U'x'>>::value == 1);
    static_assert(stdext::list_index_of<stdext::value_list<25, U'x', 127>, stdext::constant<35>>::value == 3);

    // list_index_of_v
    static_assert(stdext::list_index_of_v<stdext::type_list<int, char, double, int>, int> == 0);
    static_assert(stdext::list_index_of_v<stdext::type_list<int, char, double, int>, double> == 2);
    static_assert(stdext::list_index_of_v<stdext::type_list<int, char, double, int>, void> == 4);
    static_assert(stdext::list_index_of_v<stdext::value_list<25, U'x', 127>, stdext::constant<25>> == 0);
    static_assert(stdext::list_index_of_v<stdext::value_list<25, U'x', 127>, stdext::constant<U'x'>> == 1);
    static_assert(stdext::list_index_of_v<stdext::value_list<25, U'x', 127>, stdext::constant<35>> == 3);

    // list_prepend
    static_assert(std::is_same_v<stdext::list_prepend<stdext::type_list<>, int>::type, stdext::type_list<int>>);
    static_assert(std::is_same_v<stdext::list_prepend<stdext::type_list<int, float>, char>::type, stdext::type_list<char, int, float>>);
    static_assert(std::is_same_v<stdext::list_prepend<stdext::value_list<>, stdext::constant<5>>::type, stdext::value_list<5>>);
    static_assert(std::is_same_v<stdext::list_prepend<stdext::value_list<5, 29>, stdext::constant<31>>::type, stdext::value_list<31, 5, 29>>);

    // list_prepend_t
    static_assert(std::is_same_v<stdext::list_prepend_t<stdext::type_list<>, int>, stdext::type_list<int>>);
    static_assert(std::is_same_v<stdext::list_prepend_t<stdext::type_list<int, float>, char>, stdext::type_list<char, int, float>>);
    static_assert(std::is_same_v<stdext::list_prepend_t<stdext::value_list<>, stdext::constant<5>>, stdext::value_list<5>>);
    static_assert(std::is_same_v<stdext::list_prepend_t<stdext::value_list<5, 29>, stdext::constant<31>>, stdext::value_list<31, 5, 29>>);

    // list_append
    static_assert(std::is_same_v<stdext::list_append<stdext::type_list<>, int>::type, stdext::type_list<int>>);
    static_assert(std::is_same_v<stdext::list_append<stdext::type_list<int, float>, char>::type, stdext::type_list<int, float, char>>);
    static_assert(std::is_same_v<stdext::list_append<stdext::value_list<>, stdext::constant<5>>::type, stdext::value_list<5>>);
    static_assert(std::is_same_v<stdext::list_append<stdext::value_list<5, 29>, stdext::constant<31>>::type, stdext::value_list<5, 29, 31>>);

    // list_append_t
    static_assert(std::is_same_v<stdext::list_append_t<stdext::type_list<>, int>, stdext::type_list<int>>);
    static_assert(std::is_same_v<stdext::list_append_t<stdext::type_list<int, float>, char>, stdext::type_list<int, float, char>>);
    static_assert(std::is_same_v<stdext::list_append_t<stdext::value_list<>, stdext::constant<5>>, stdext::value_list<5>>);
    static_assert(std::is_same_v<stdext::list_append_t<stdext::value_list<5, 29>, stdext::constant<31>>, stdext::value_list<5, 29, 31>>);

    // list_concat
    static_assert(std::is_same_v<stdext::list_concat<stdext::type_list<int, float>>::type, stdext::type_list<int, float>>);
    static_assert(std::is_same_v<stdext::list_concat<stdext::type_list<int, float>, stdext::type_list<short, double>>::type, stdext::type_list<int, float, short, double>>);
    static_assert(std::is_same_v<stdext::list_concat<stdext::type_list<int, float>, stdext::type_list<short, double>, stdext::type_list<long, long double>>::type, stdext::type_list<int, float, short, double, long, long double>>);
    static_assert(std::is_same_v<stdext::list_concat<stdext::value_list<1, 1>>::type, stdext::value_list<1, 1>>);
    static_assert(std::is_same_v<stdext::list_concat<stdext::value_list<1, 1>, stdext::value_list<2, 3>>::type, stdext::value_list<1, 1, 2, 3>>);
    static_assert(std::is_same_v<stdext::list_concat<stdext::value_list<1, 1>, stdext::value_list<2, 3>, stdext::value_list<5, 8>>::type, stdext::value_list<1, 1, 2, 3, 5, 8>>);

    // list_concat_t
    static_assert(std::is_same_v<stdext::list_concat_t<stdext::type_list<int, float>>, stdext::type_list<int, float>>);
    static_assert(std::is_same_v<stdext::list_concat_t<stdext::type_list<int, float>, stdext::type_list<short, double>>, stdext::type_list<int, float, short, double>>);
    static_assert(std::is_same_v<stdext::list_concat_t<stdext::type_list<int, float>, stdext::type_list<short, double>, stdext::type_list<long, long double>>, stdext::type_list<int, float, short, double, long, long double>>);
    static_assert(std::is_same_v<stdext::list_concat_t<stdext::value_list<1, 1>>, stdext::value_list<1, 1>>);
    static_assert(std::is_same_v<stdext::list_concat_t<stdext::value_list<1, 1>, stdext::value_list<2, 3>>, stdext::value_list<1, 1, 2, 3>>);
    static_assert(std::is_same_v<stdext::list_concat_t<stdext::value_list<1, 1>, stdext::value_list<2, 3>, stdext::value_list<5, 8>>, stdext::value_list<1, 1, 2, 3, 5, 8>>);

    // list_take
    static_assert(std::is_same_v<stdext::list_take<stdext::type_list<bool, char, short, int>, 0>::type, stdext::type_list<>>);
    static_assert(std::is_same_v<stdext::list_take<stdext::type_list<bool, char, short, int>, 1>::type, stdext::type_list<bool>>);
    static_assert(std::is_same_v<stdext::list_take<stdext::type_list<bool, char, short, int>, 2>::type, stdext::type_list<bool, char>>);
    static_assert(std::is_same_v<stdext::list_take<stdext::value_list<1, 1, 2, 3, 5>, 0>::type, stdext::value_list<>>);
    static_assert(std::is_same_v<stdext::list_take<stdext::value_list<1, 1, 2, 3, 5>, 2>::type, stdext::value_list<1, 1>>);
    static_assert(std::is_same_v<stdext::list_take<stdext::value_list<1, 1, 2, 3, 5>, 4>::type, stdext::value_list<1, 1, 2, 3>>);

    // list_take_t
    static_assert(std::is_same_v<stdext::list_take_t<stdext::type_list<bool, char, short, int>, 0>, stdext::type_list<>>);
    static_assert(std::is_same_v<stdext::list_take_t<stdext::type_list<bool, char, short, int>, 1>, stdext::type_list<bool>>);
    static_assert(std::is_same_v<stdext::list_take_t<stdext::type_list<bool, char, short, int>, 2>, stdext::type_list<bool, char>>);
    static_assert(std::is_same_v<stdext::list_take_t<stdext::value_list<1, 1, 2, 3, 5>, 0>, stdext::value_list<>>);
    static_assert(std::is_same_v<stdext::list_take_t<stdext::value_list<1, 1, 2, 3, 5>, 2>, stdext::value_list<1, 1>>);
    static_assert(std::is_same_v<stdext::list_take_t<stdext::value_list<1, 1, 2, 3, 5>, 4>, stdext::value_list<1, 1, 2, 3>>);

    // list_drop
    static_assert(std::is_same_v<stdext::list_drop<stdext::type_list<bool, char, short, int>, 0>::type, stdext::type_list<bool, char, short, int>>);
    static_assert(std::is_same_v<stdext::list_drop<stdext::type_list<bool, char, short, int>, 1>::type, stdext::type_list<char, short, int>>);
    static_assert(std::is_same_v<stdext::list_drop<stdext::type_list<bool, char, short, int>, 2>::type, stdext::type_list<short, int>>);
    static_assert(std::is_same_v<stdext::list_drop<stdext::value_list<1, 1, 2, 3, 5>, 0>::type, stdext::value_list<1, 1, 2, 3, 5>>);
    static_assert(std::is_same_v<stdext::list_drop<stdext::value_list<1, 1, 2, 3, 5>, 2>::type, stdext::value_list<2, 3, 5>>);
    static_assert(std::is_same_v<stdext::list_drop<stdext::value_list<1, 1, 2, 3, 5>, 4>::type, stdext::value_list<5>>);

    // list_drop_t
    static_assert(std::is_same_v<stdext::list_drop_t<stdext::type_list<bool, char, short, int>, 0>, stdext::type_list<bool, char, short, int>>);
    static_assert(std::is_same_v<stdext::list_drop_t<stdext::type_list<bool, char, short, int>, 1>, stdext::type_list<char, short, int>>);
    static_assert(std::is_same_v<stdext::list_drop_t<stdext::type_list<bool, char, short, int>, 2>, stdext::type_list<short, int>>);
    static_assert(std::is_same_v<stdext::list_drop_t<stdext::value_list<1, 1, 2, 3, 5>, 0>, stdext::value_list<1, 1, 2, 3, 5>>);
    static_assert(std::is_same_v<stdext::list_drop_t<stdext::value_list<1, 1, 2, 3, 5>, 2>, stdext::value_list<2, 3, 5>>);
    static_assert(std::is_same_v<stdext::list_drop_t<stdext::value_list<1, 1, 2, 3, 5>, 4>, stdext::value_list<5>>);

    // list_insert_element
    static_assert(std::is_same_v<stdext::list_insert_element<stdext::type_list<>, 0, int>::type, stdext::type_list<int>>);
    static_assert(std::is_same_v<stdext::list_insert_element<stdext::type_list<int, int>, 0, float>::type, stdext::type_list<float, int, int>>);
    static_assert(std::is_same_v<stdext::list_insert_element<stdext::type_list<int, int>, 1, float>::type, stdext::type_list<int, float, int>>);
    static_assert(std::is_same_v<stdext::list_insert_element<stdext::type_list<int, int>, 2, float>::type, stdext::type_list<int, int, float>>);
    static_assert(std::is_same_v<stdext::list_insert_element<stdext::value_list<>, 0, stdext::constant<5>>::type, stdext::value_list<5>>);
    static_assert(std::is_same_v<stdext::list_insert_element<stdext::value_list<3, 3>, 0, stdext::constant<5>>::type, stdext::value_list<5, 3, 3>>);
    static_assert(std::is_same_v<stdext::list_insert_element<stdext::value_list<3, 3>, 1, stdext::constant<5>>::type, stdext::value_list<3, 5, 3>>);
    static_assert(std::is_same_v<stdext::list_insert_element<stdext::value_list<3, 3>, 2, stdext::constant<5>>::type, stdext::value_list<3, 3, 5>>);

    // list_insert_element_t
    static_assert(std::is_same_v<stdext::list_insert_element_t<stdext::type_list<>, 0, int>, stdext::type_list<int>>);
    static_assert(std::is_same_v<stdext::list_insert_element_t<stdext::type_list<int, int>, 0, float>, stdext::type_list<float, int, int>>);
    static_assert(std::is_same_v<stdext::list_insert_element_t<stdext::type_list<int, int>, 1, float>, stdext::type_list<int, float, int>>);
    static_assert(std::is_same_v<stdext::list_insert_element_t<stdext::type_list<int, int>, 2, float>, stdext::type_list<int, int, float>>);
    static_assert(std::is_same_v<stdext::list_insert_element_t<stdext::value_list<>, 0, stdext::constant<5>>, stdext::value_list<5>>);
    static_assert(std::is_same_v<stdext::list_insert_element_t<stdext::value_list<3, 3>, 0, stdext::constant<5>>, stdext::value_list<5, 3, 3>>);
    static_assert(std::is_same_v<stdext::list_insert_element_t<stdext::value_list<3, 3>, 1, stdext::constant<5>>, stdext::value_list<3, 5, 3>>);
    static_assert(std::is_same_v<stdext::list_insert_element_t<stdext::value_list<3, 3>, 2, stdext::constant<5>>, stdext::value_list<3, 3, 5>>);

    // list_insert_elements
    static_assert(std::is_same_v<stdext::list_insert_elements<stdext::type_list<>, stdext::list_insert_elements_arg<0, int>, stdext::list_insert_elements_arg<0, float>>::type, stdext::type_list<int, float>>);
    static_assert(std::is_same_v<stdext::list_insert_elements<stdext::type_list<int, float>, stdext::list_insert_elements_arg<1, short>, stdext::list_insert_elements_arg<2, double>>::type, stdext::type_list<int, short, float, double>>);
    static_assert(std::is_same_v<stdext::list_insert_elements<stdext::value_list<>, stdext::list_insert_elements_arg<0, stdext::constant<1>>, stdext::list_insert_elements_arg<0, stdext::constant<2>>>::type, stdext::value_list<1, 2>>);
    static_assert(std::is_same_v<stdext::list_insert_elements<stdext::value_list<1, 2, 5>, stdext::list_insert_elements_arg<1, stdext::constant<1>>, stdext::list_insert_elements_arg<2, stdext::constant<3>>>::type, stdext::value_list<1, 1, 2, 3, 5>>);

    // list_insert_elements_t
    static_assert(std::is_same_v<stdext::list_insert_elements_t<stdext::type_list<>, stdext::list_insert_elements_arg<0, int>, stdext::list_insert_elements_arg<0, float>>, stdext::type_list<int, float>>);
    static_assert(std::is_same_v<stdext::list_insert_elements_t<stdext::type_list<int, float>, stdext::list_insert_elements_arg<1, short>, stdext::list_insert_elements_arg<2, double>>, stdext::type_list<int, short, float, double>>);
    static_assert(std::is_same_v<stdext::list_insert_elements_t<stdext::value_list<>, stdext::list_insert_elements_arg<0, stdext::constant<1>>, stdext::list_insert_elements_arg<0, stdext::constant<2>>>, stdext::value_list<1, 2>>);
    static_assert(std::is_same_v<stdext::list_insert_elements_t<stdext::value_list<1, 2, 5>, stdext::list_insert_elements_arg<1, stdext::constant<1>>, stdext::list_insert_elements_arg<2, stdext::constant<3>>>, stdext::value_list<1, 1, 2, 3, 5>>);

    // list_prepend_lists
    static_assert(std::is_same_v<stdext::list_prepend_lists<stdext::type_list<stdext::type_list<int, float>, stdext::type_list<char, double, double>>, stdext::type_list<short, float>>::type, stdext::type_list<stdext::type_list<short, int, float>, stdext::type_list<float, char, double, double>>>);
    static_assert(std::is_same_v<stdext::list_prepend_lists<stdext::type_list<stdext::value_list<25, 37>, stdext::value_list<0, 19, 34>>, stdext::value_list<101, 405>>::type, stdext::type_list<stdext::value_list<101, 25, 37>, stdext::value_list<405, 0, 19, 34>>>);

    // list_prepend_lists_t
    static_assert(std::is_same_v<stdext::list_prepend_lists_t<stdext::type_list<stdext::type_list<int, float>, stdext::type_list<char, double, double>>, stdext::type_list<short, float>>, stdext::type_list<stdext::type_list<short, int, float>, stdext::type_list<float, char, double, double>>>);
    static_assert(std::is_same_v<stdext::list_prepend_lists_t<stdext::type_list<stdext::value_list<25, 37>, stdext::value_list<0, 19, 34>>, stdext::value_list<101, 405>>, stdext::type_list<stdext::value_list<101, 25, 37>, stdext::value_list<405, 0, 19, 34>>>);

    // list_append_lists
    static_assert(std::is_same_v<stdext::list_append_lists<stdext::type_list<stdext::type_list<int, float>, stdext::type_list<char, double, double>>, stdext::type_list<short, float>>::type, stdext::type_list<stdext::type_list<int, float, short>, stdext::type_list<char, double, double, float>>>);
    static_assert(std::is_same_v<stdext::list_append_lists<stdext::type_list<stdext::value_list<25, 37>, stdext::value_list<0, 19, 34>>, stdext::value_list<101, 405>>::type, stdext::type_list<stdext::value_list<25, 37, 101>, stdext::value_list<0, 19, 34, 405>>>);

    // list_append_lists_t
    static_assert(std::is_same_v<stdext::list_append_lists_t<stdext::type_list<stdext::type_list<int, float>, stdext::type_list<char, double, double>>, stdext::type_list<short, float>>, stdext::type_list<stdext::type_list<int, float, short>, stdext::type_list<char, double, double, float>>>);
    static_assert(std::is_same_v<stdext::list_append_lists_t<stdext::type_list<stdext::value_list<25, 37>, stdext::value_list<0, 19, 34>>, stdext::value_list<101, 405>>, stdext::type_list<stdext::value_list<25, 37, 101>, stdext::value_list<0, 19, 34, 405>>>);

    // list_concat_lists
    static_assert(std::is_same_v<stdext::list_concat_lists<stdext::type_list<stdext::type_list<int, float>, stdext::type_list<short, double>>, stdext::type_list<stdext::type_list<bool, char>, stdext::type_list<wchar_t, int>>>::type, stdext::type_list<stdext::type_list<int, float, bool, char>, stdext::type_list<short, double, wchar_t, int>>>);
    static_assert(std::is_same_v<stdext::list_concat_lists<stdext::type_list<stdext::value_list<15, 52>, stdext::value_list<3842, 160>>, stdext::type_list<stdext::value_list<4892, 1694>, stdext::value_list<752, 5028>>>::type, stdext::type_list<stdext::value_list<15, 52, 4892, 1694>, stdext::value_list<3842, 160, 752, 5028>>>);
    static_assert(std::is_same_v<stdext::list_concat_lists<stdext::type_list<stdext::type_list<int, float>, stdext::value_list<3842, 160>>, stdext::type_list<stdext::type_list<bool, char>, stdext::value_list<752, 5028>>>::type, stdext::type_list<stdext::type_list<int, float, bool, char>, stdext::value_list<3842, 160, 752, 5028>>>);

    // list_concat_lists_t
    static_assert(std::is_same_v<stdext::list_concat_lists_t<stdext::type_list<stdext::type_list<int, float>, stdext::type_list<short, double>>, stdext::type_list<stdext::type_list<bool, char>, stdext::type_list<wchar_t, int>>>, stdext::type_list<stdext::type_list<int, float, bool, char>, stdext::type_list<short, double, wchar_t, int>>>);
    static_assert(std::is_same_v<stdext::list_concat_lists_t<stdext::type_list<stdext::value_list<15, 52>, stdext::value_list<3842, 160>>, stdext::type_list<stdext::value_list<4892, 1694>, stdext::value_list<752, 5028>>>, stdext::type_list<stdext::value_list<15, 52, 4892, 1694>, stdext::value_list<3842, 160, 752, 5028>>>);
    static_assert(std::is_same_v<stdext::list_concat_lists_t<stdext::type_list<stdext::type_list<int, float>, stdext::value_list<3842, 160>>, stdext::type_list<stdext::type_list<bool, char>, stdext::value_list<752, 5028>>>, stdext::type_list<stdext::type_list<int, float, bool, char>, stdext::value_list<3842, 160, 752, 5028>>>);

    // list_transpose
    static_assert(std::is_same_v<stdext::list_transpose<stdext::type_list<stdext::type_list<>>>::type, stdext::type_list<stdext::type_list<>>>);
    static_assert(std::is_same_v<stdext::list_transpose<stdext::type_list<stdext::type_list<int>>>::type, stdext::type_list<stdext::type_list<int>>>);
    static_assert(std::is_same_v<stdext::list_transpose<stdext::type_list<stdext::type_list<int, float>>>::type, stdext::type_list<stdext::type_list<int>, stdext::type_list<float>>>);
    static_assert(std::is_same_v<stdext::list_transpose<stdext::type_list<stdext::type_list<int>, stdext::type_list<float>>>::type, stdext::type_list<stdext::type_list<int, float>>>);
    static_assert(std::is_same_v<stdext::list_transpose<stdext::type_list<stdext::type_list<bool, char>, stdext::type_list<short, long>>>::type, stdext::type_list<stdext::type_list<bool, short>, stdext::type_list<char, long>>>);
    static_assert(std::is_same_v<stdext::list_transpose<stdext::type_list<stdext::value_list<>>>::type, stdext::type_list<stdext::value_list<>>>);
    static_assert(std::is_same_v<stdext::list_transpose<stdext::type_list<stdext::value_list<0>>>::type, stdext::type_list<stdext::value_list<0>>>);
    static_assert(std::is_same_v<stdext::list_transpose<stdext::type_list<stdext::value_list<0, 1>>>::type, stdext::type_list<stdext::value_list<0>, stdext::value_list<1>>>);
    static_assert(std::is_same_v<stdext::list_transpose<stdext::type_list<stdext::value_list<0>, stdext::value_list<1>>>::type, stdext::type_list<stdext::value_list<0, 1>>>);
    static_assert(std::is_same_v<stdext::list_transpose<stdext::type_list<stdext::value_list<0, 1>, stdext::value_list<2, 3>>>::type, stdext::type_list<stdext::value_list<0, 2>, stdext::value_list<1, 3>>>);

    // list_transpose_t
    static_assert(std::is_same_v<stdext::list_transpose_t<stdext::type_list<stdext::type_list<>>>, stdext::type_list<stdext::type_list<>>>);
    static_assert(std::is_same_v<stdext::list_transpose_t<stdext::type_list<stdext::type_list<int>>>, stdext::type_list<stdext::type_list<int>>>);
    static_assert(std::is_same_v<stdext::list_transpose_t<stdext::type_list<stdext::type_list<int, float>>>, stdext::type_list<stdext::type_list<int>, stdext::type_list<float>>>);
    static_assert(std::is_same_v<stdext::list_transpose_t<stdext::type_list<stdext::type_list<int>, stdext::type_list<float>>>, stdext::type_list<stdext::type_list<int, float>>>);
    static_assert(std::is_same_v<stdext::list_transpose_t<stdext::type_list<stdext::type_list<bool, char>, stdext::type_list<short, long>>>, stdext::type_list<stdext::type_list<bool, short>, stdext::type_list<char, long>>>);
    static_assert(std::is_same_v<stdext::list_transpose_t<stdext::type_list<stdext::value_list<>>>, stdext::type_list<stdext::value_list<>>>);
    static_assert(std::is_same_v<stdext::list_transpose_t<stdext::type_list<stdext::value_list<0>>>, stdext::type_list<stdext::value_list<0>>>);
    static_assert(std::is_same_v<stdext::list_transpose_t<stdext::type_list<stdext::value_list<0, 1>>>, stdext::type_list<stdext::value_list<0>, stdext::value_list<1>>>);
    static_assert(std::is_same_v<stdext::list_transpose_t<stdext::type_list<stdext::value_list<0>, stdext::value_list<1>>>, stdext::type_list<stdext::value_list<0, 1>>>);
    static_assert(std::is_same_v<stdext::list_transpose_t<stdext::type_list<stdext::value_list<0, 1>, stdext::value_list<2, 3>>>, stdext::type_list<stdext::value_list<0, 2>, stdext::value_list<1, 3>>>);

    // list_zip
    static_assert(std::is_same_v<stdext::list_zip<stdext::type_list<>>::type, stdext::type_list<stdext::type_list<>>>);
    static_assert(std::is_same_v<stdext::list_zip<stdext::type_list<int>>::type, stdext::type_list<stdext::type_list<int>>>);
    static_assert(std::is_same_v<stdext::list_zip<stdext::type_list<int, float>>::type, stdext::type_list<stdext::type_list<int>, stdext::type_list<float>>>);
    static_assert(std::is_same_v<stdext::list_zip<stdext::type_list<int>, stdext::type_list<float>>::type, stdext::type_list<stdext::type_list<int, float>>>);
    static_assert(std::is_same_v<stdext::list_zip<stdext::type_list<bool, char>, stdext::type_list<short, long>>::type, stdext::type_list<stdext::type_list<bool, short>, stdext::type_list<char, long>>>);
    static_assert(std::is_same_v<stdext::list_zip<stdext::value_list<>>::type, stdext::type_list<stdext::value_list<>>>);
    static_assert(std::is_same_v<stdext::list_zip<stdext::value_list<0>>::type, stdext::type_list<stdext::value_list<0>>>);
    static_assert(std::is_same_v<stdext::list_zip<stdext::value_list<0, 1>>::type, stdext::type_list<stdext::value_list<0>, stdext::value_list<1>>>);
    static_assert(std::is_same_v<stdext::list_zip<stdext::value_list<0>, stdext::value_list<1>>::type, stdext::type_list<stdext::value_list<0, 1>>>);
    static_assert(std::is_same_v<stdext::list_zip<stdext::value_list<0, 1>, stdext::value_list<2, 3>>::type, stdext::type_list<stdext::value_list<0, 2>, stdext::value_list<1, 3>>>);

    // list_zip_t
    static_assert(std::is_same_v<stdext::list_zip_t<stdext::type_list<>>, stdext::type_list<stdext::type_list<>>>);
    static_assert(std::is_same_v<stdext::list_zip_t<stdext::type_list<int>>, stdext::type_list<stdext::type_list<int>>>);
    static_assert(std::is_same_v<stdext::list_zip_t<stdext::type_list<int, float>>, stdext::type_list<stdext::type_list<int>, stdext::type_list<float>>>);
    static_assert(std::is_same_v<stdext::list_zip_t<stdext::type_list<int>, stdext::type_list<float>>, stdext::type_list<stdext::type_list<int, float>>>);
    static_assert(std::is_same_v<stdext::list_zip_t<stdext::type_list<bool, char>, stdext::type_list<short, long>>, stdext::type_list<stdext::type_list<bool, short>, stdext::type_list<char, long>>>);
    static_assert(std::is_same_v<stdext::list_zip_t<stdext::value_list<>>, stdext::type_list<stdext::value_list<>>>);
    static_assert(std::is_same_v<stdext::list_zip_t<stdext::value_list<0>>, stdext::type_list<stdext::value_list<0>>>);
    static_assert(std::is_same_v<stdext::list_zip_t<stdext::value_list<0, 1>>, stdext::type_list<stdext::value_list<0>, stdext::value_list<1>>>);
    static_assert(std::is_same_v<stdext::list_zip_t<stdext::value_list<0>, stdext::value_list<1>>, stdext::type_list<stdext::value_list<0, 1>>>);
    static_assert(std::is_same_v<stdext::list_zip_t<stdext::value_list<0, 1>, stdext::value_list<2, 3>>, stdext::type_list<stdext::value_list<0, 2>, stdext::value_list<1, 3>>>);

    namespace
    {
        template <typename T> struct one_type { };
        template <typename T0, typename T1> struct two_types { };
        template <typename... Ts> struct many_types { };
    }

    // list_apply
    static_assert(std::is_same_v<stdext::list_apply<one_type, stdext::type_list<int>>::type, one_type<int>>);
    static_assert(std::is_same_v<stdext::list_apply<two_types, stdext::type_list<signed char, unsigned char>>::type, two_types<signed char, unsigned char>>);
    static_assert(std::is_same_v<stdext::list_apply<many_types, stdext::type_list<short, float, double, long>>::type, many_types<short, float, double, long>>);

    // list_apply_t
    static_assert(std::is_same_v<stdext::list_apply_t<one_type, stdext::type_list<int>>, one_type<int>>);
    static_assert(std::is_same_v<stdext::list_apply_t<two_types, stdext::type_list<signed char, unsigned char>>, two_types<signed char, unsigned char>>);
    static_assert(std::is_same_v<stdext::list_apply_t<many_types, stdext::type_list<short, float, double, long>>, many_types<short, float, double, long>>);

    // list_conditional_apply
    static_assert(std::is_same_v<stdext::list_conditional_apply<false, one_type, stdext::type_list<int>, two_types, stdext::type_list<signed char, unsigned char>>::type, two_types<signed char, unsigned char>>);
    static_assert(std::is_same_v<stdext::list_conditional_apply<true, one_type, stdext::type_list<int>, two_types, stdext::type_list<signed char, unsigned char>>::type, one_type<int>>);
    static_assert(std::is_same_v<stdext::list_conditional_apply<false, many_types, stdext::type_list<short, float, double, long>, many_types, stdext::type_list<int, int, long>>::type, many_types<int, int, long>>);
    static_assert(std::is_same_v<stdext::list_conditional_apply<true, many_types, stdext::type_list<short, float, double, long>, many_types, stdext::type_list<int, int, long>>::type, many_types<short, float, double, long>>);

    // list_conditional_apply_t
    static_assert(std::is_same_v<stdext::list_conditional_apply_t<false, one_type, stdext::type_list<int>, two_types, stdext::type_list<signed char, unsigned char>>, two_types<signed char, unsigned char>>);
    static_assert(std::is_same_v<stdext::list_conditional_apply_t<true, one_type, stdext::type_list<int>, two_types, stdext::type_list<signed char, unsigned char>>, one_type<int>>);
    static_assert(std::is_same_v<stdext::list_conditional_apply_t<false, many_types, stdext::type_list<short, float, double, long>, many_types, stdext::type_list<int, int, long>>, many_types<int, int, long>>);
    static_assert(std::is_same_v<stdext::list_conditional_apply_t<true, many_types, stdext::type_list<short, float, double, long>, many_types, stdext::type_list<int, int, long>>, many_types<short, float, double, long>>);

    namespace
    {
        template <typename> struct always_false : std::false_type { };
        template <typename> struct always_true : std::true_type { };
    }

    // list_all_of
    static_assert(stdext::list_all_of<stdext::type_list<>, always_false>::value);
    static_assert(stdext::list_all_of<stdext::type_list<>, always_true>::value);
    static_assert(!stdext::list_all_of<stdext::type_list<int>, always_false>::value);
    static_assert(stdext::list_all_of<stdext::type_list<int>, always_true>::value);
    static_assert(!stdext::list_all_of<stdext::type_list<void, int*, float>, std::is_integral>::value);
    static_assert(!stdext::list_all_of<stdext::type_list<int, long, double>, std::is_integral>::value);
    static_assert(stdext::list_all_of<stdext::type_list<short, long, char>, std::is_integral>::value);

    // list_all_of_v
    static_assert(stdext::list_all_of_v<stdext::type_list<>, always_false>);
    static_assert(stdext::list_all_of_v<stdext::type_list<>, always_true>);
    static_assert(!stdext::list_all_of_v<stdext::type_list<int>, always_false>);
    static_assert(stdext::list_all_of_v<stdext::type_list<int>, always_true>);
    static_assert(!stdext::list_all_of_v<stdext::type_list<void, int*, float>, std::is_integral>);
    static_assert(!stdext::list_all_of_v<stdext::type_list<int, long, double>, std::is_integral>);
    static_assert(stdext::list_all_of_v<stdext::type_list<short, long, char>, std::is_integral>);

    // list_any_of
    static_assert(!stdext::list_any_of<stdext::type_list<>, always_false>::value);
    static_assert(!stdext::list_any_of<stdext::type_list<>, always_true>::value);
    static_assert(!stdext::list_any_of<stdext::type_list<int>, always_false>::value);
    static_assert(stdext::list_any_of<stdext::type_list<int>, always_true>::value);
    static_assert(!stdext::list_any_of<stdext::type_list<void, int*, float>, std::is_integral>::value);
    static_assert(stdext::list_any_of<stdext::type_list<int, long, double>, std::is_integral>::value);
    static_assert(stdext::list_any_of<stdext::type_list<short, long, char>, std::is_integral>::value);

    // list_any_of_v
    static_assert(!stdext::list_any_of_v<stdext::type_list<>, always_false>);
    static_assert(!stdext::list_any_of_v<stdext::type_list<>, always_true>);
    static_assert(!stdext::list_any_of_v<stdext::type_list<int>, always_false>);
    static_assert(stdext::list_any_of_v<stdext::type_list<int>, always_true>);
    static_assert(!stdext::list_any_of_v<stdext::type_list<void, int*, float>, std::is_integral>);
    static_assert(stdext::list_any_of_v<stdext::type_list<int, long, double>, std::is_integral>);
    static_assert(stdext::list_any_of_v<stdext::type_list<short, long, char>, std::is_integral>);

    // list_none_of
    static_assert(stdext::list_none_of<stdext::type_list<>, always_false>::value);
    static_assert(stdext::list_none_of<stdext::type_list<>, always_true>::value);
    static_assert(stdext::list_none_of<stdext::type_list<int>, always_false>::value);
    static_assert(!stdext::list_none_of<stdext::type_list<int>, always_true>::value);
    static_assert(stdext::list_none_of<stdext::type_list<void, int*, float>, std::is_integral>::value);
    static_assert(!stdext::list_none_of<stdext::type_list<int, long, double>, std::is_integral>::value);
    static_assert(!stdext::list_none_of<stdext::type_list<short, long, char>, std::is_integral>::value);

    // list_none_of_v
    static_assert(stdext::list_none_of_v<stdext::type_list<>, always_false>);
    static_assert(stdext::list_none_of_v<stdext::type_list<>, always_true>);
    static_assert(stdext::list_none_of_v<stdext::type_list<int>, always_false>);
    static_assert(!stdext::list_none_of_v<stdext::type_list<int>, always_true>);
    static_assert(stdext::list_none_of_v<stdext::type_list<void, int*, float>, std::is_integral>);
    static_assert(!stdext::list_none_of_v<stdext::type_list<int, long, double>, std::is_integral>);
    static_assert(!stdext::list_none_of_v<stdext::type_list<short, long, char>, std::is_integral>);

    // list_count_fulfilling
    static_assert(stdext::list_count_fulfilling<stdext::type_list<>, always_false>::value == 0);
    static_assert(stdext::list_count_fulfilling<stdext::type_list<>, always_true>::value == 0);
    static_assert(stdext::list_count_fulfilling<stdext::type_list<int>, always_false>::value == 0);
    static_assert(stdext::list_count_fulfilling<stdext::type_list<int>, always_true>::value == 1);
    static_assert(stdext::list_count_fulfilling<stdext::type_list<void, int*, float>, std::is_integral>::value == 0);
    static_assert(stdext::list_count_fulfilling<stdext::type_list<int, long, double>, std::is_integral>::value == 2);
    static_assert(stdext::list_count_fulfilling<stdext::type_list<short, long, char>, std::is_integral>::value == 3);

    // list_count_fulfilling_v
    static_assert(stdext::list_count_fulfilling_v<stdext::type_list<>, always_false> == 0);
    static_assert(stdext::list_count_fulfilling_v<stdext::type_list<>, always_true> == 0);
    static_assert(stdext::list_count_fulfilling_v<stdext::type_list<int>, always_false> == 0);
    static_assert(stdext::list_count_fulfilling_v<stdext::type_list<int>, always_true> == 1);
    static_assert(stdext::list_count_fulfilling_v<stdext::type_list<void, int*, float>, std::is_integral> == 0);
    static_assert(stdext::list_count_fulfilling_v<stdext::type_list<int, long, double>, std::is_integral> == 2);
    static_assert(stdext::list_count_fulfilling_v<stdext::type_list<short, long, char>, std::is_integral> == 3);

    namespace
    {
        STDEXT_DECLARE_HAS_INNER_TYPE(type);
    }

    // list_find_first_fulfilling
    static_assert(!STDEXT_HAS_INNER_TYPE_V((stdext::list_find_first_fulfilling<stdext::type_list<>, always_false>), type));
    static_assert(!STDEXT_HAS_INNER_TYPE_V((stdext::list_find_first_fulfilling<stdext::type_list<>, always_true>), type));
    static_assert(!STDEXT_HAS_INNER_TYPE_V((stdext::list_find_first_fulfilling<stdext::type_list<int>, always_false>), type));
    static_assert(std::is_same_v<stdext::list_find_first_fulfilling<stdext::type_list<int>, always_true>::type, int>);
    static_assert(!STDEXT_HAS_INNER_TYPE_V((stdext::list_find_first_fulfilling<stdext::type_list<void, int*, float>, std::is_integral>), type));
    static_assert(std::is_same_v<stdext::list_find_first_fulfilling<stdext::type_list<int, long, double>, std::is_integral>::type, int>);
    static_assert(std::is_same_v<stdext::list_find_first_fulfilling<stdext::type_list<float, short, long>, std::is_integral>::type, short>);

    // list_find_first_fulfilling_t
    static_assert(std::is_same_v<stdext::list_find_first_fulfilling_t<stdext::type_list<int>, always_true>, int>);
    static_assert(std::is_same_v<stdext::list_find_first_fulfilling_t<stdext::type_list<int, long, double>, std::is_integral>, int>);
    static_assert(std::is_same_v<stdext::list_find_first_fulfilling_t<stdext::type_list<float, short, long>, std::is_integral>, short>);

    // iota_list
    static_assert(std::is_same_v<stdext::iota_list<5>::type, stdext::value_list<0, 1, 2, 3, 4>>);
    static_assert(std::is_same_v<stdext::iota_list<5, unsigned>::type, stdext::value_list<0u, 1u, 2u, 3u, 4u>>);
    static_assert(std::is_same_v<stdext::iota_list<3, unsigned, 7>::type, stdext::value_list<7u, 8u, 9u>>);

    // iota_list_t
    static_assert(std::is_same_v<stdext::iota_list_t<5>, stdext::value_list<0, 1, 2, 3, 4>>);
    static_assert(std::is_same_v<stdext::iota_list_t<5, unsigned>, stdext::value_list<0u, 1u, 2u, 3u, 4u>>);
    static_assert(std::is_same_v<stdext::iota_list_t<3, unsigned, 7>, stdext::value_list<7u, 8u, 9u>>);

    // index_list
    static_assert(std::is_same_v<stdext::index_list<5>::type, stdext::value_list<std::size_t(0), std::size_t(1), std::size_t(2), std::size_t(3), std::size_t(4)>>);
    static_assert(std::is_same_v<stdext::index_list<3, 7>::type, stdext::value_list<std::size_t(7), std::size_t(8), std::size_t(9)>>);

    // index_list_t
    static_assert(std::is_same_v<stdext::index_list_t<5>, stdext::value_list<std::size_t(0), std::size_t(1), std::size_t(2), std::size_t(3), std::size_t(4)>>);
    static_assert(std::is_same_v<stdext::index_list_t<3, 7>, stdext::value_list<std::size_t(7), std::size_t(8), std::size_t(9)>>);
}
