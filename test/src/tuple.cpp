#include <stdext/tuple.h>


// tuple_element_list
static_assert(std::is_same_v<stdext::tuple_element_list<std::tuple<>>::type, stdext::type_list<>>);
static_assert(std::is_same_v<stdext::tuple_element_list<std::tuple<int>>::type, stdext::type_list<int>>);
static_assert(std::is_same_v<stdext::tuple_element_list<std::tuple<int, float>>::type, stdext::type_list<int, float>>);
static_assert(std::is_same_v<stdext::tuple_element_list<std::tuple<int, float, void*>>::type, stdext::type_list<int, float, void*>>);

// tuple_element_list_t
static_assert(std::is_same_v<stdext::tuple_element_list_t<std::tuple<>>, stdext::type_list<>>);
static_assert(std::is_same_v<stdext::tuple_element_list_t<std::tuple<int>>, stdext::type_list<int>>);
static_assert(std::is_same_v<stdext::tuple_element_list_t<std::tuple<int, float>>, stdext::type_list<int, float>>);
static_assert(std::is_same_v<stdext::tuple_element_list_t<std::tuple<int, float, void*>>, stdext::type_list<int, float, void*>>);

// zip
static_assert(stdext::zip(std::make_tuple(1, 2, 5), std::make_tuple(1, 3, 8)) == std::make_tuple(std::make_tuple(1, 1), std::make_tuple(2, 3), std::make_tuple(5, 8)));
