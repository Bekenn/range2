#include <stdext/iterator.h>

#include "iterators.h"

#include <catch2/catch.hpp>


namespace test
{
    // is_iterator
    static_assert(!stdext::is_iterator<void>::value);
    static_assert(!stdext::is_iterator<void*>::value);
    static_assert(!stdext::is_iterator<const void*>::value);
    static_assert(!stdext::is_iterator<volatile void*>::value);
    static_assert(!stdext::is_iterator<const volatile void*>::value);
    static_assert(!stdext::is_iterator<int>::value);
    static_assert(!stdext::is_iterator<int&>::value);
    static_assert(stdext::is_iterator<int*>::value);
    static_assert(!stdext::is_iterator<int*&>::value);
    static_assert(!stdext::is_iterator<int[]>::value);
    static_assert(stdext::is_iterator<int (*)[]>::value);
    static_assert(stdext::is_iterator<int (*)[5]>::value);
    static_assert(!stdext::is_iterator<void ()>::value);
    static_assert(!stdext::is_iterator<void (*)()>::value);
    static_assert(stdext::is_iterator<int**>::value);
    static_assert(stdext::is_iterator<const int*>::value);
    static_assert(stdext::is_iterator<int* const>::value);
    static_assert(stdext::is_iterator<volatile int*>::value);
    static_assert(stdext::is_iterator<int* volatile>::value);
    static_assert(stdext::is_iterator<fixed_input_iterator>::value);
    static_assert(stdext::is_iterator<fixed_output_iterator>::value);
    static_assert(stdext::is_iterator<input_iterator>::value);
    static_assert(stdext::is_iterator<output_iterator>::value);
    static_assert(stdext::is_iterator<forward_iterator>::value);
    static_assert(stdext::is_iterator<bidirectional_iterator>::value);
    static_assert(stdext::is_iterator<random_access_iterator>::value);

    // is_iterator_v
    static_assert(!stdext::is_iterator_v<void>);
    static_assert(!stdext::is_iterator_v<void*>);
    static_assert(!stdext::is_iterator_v<const void*>);
    static_assert(!stdext::is_iterator_v<volatile void*>);
    static_assert(!stdext::is_iterator_v<const volatile void*>);
    static_assert(!stdext::is_iterator_v<int>);
    static_assert(!stdext::is_iterator_v<int&>);
    static_assert(stdext::is_iterator_v<int*>);
    static_assert(!stdext::is_iterator_v<int*&>);
    static_assert(!stdext::is_iterator_v<int[]>);
    static_assert(stdext::is_iterator_v<int (*)[]>);
    static_assert(stdext::is_iterator_v<int (*)[5]>);
    static_assert(!stdext::is_iterator_v<void ()>);
    static_assert(!stdext::is_iterator_v<void (*)()>);
    static_assert(stdext::is_iterator_v<int**>);
    static_assert(stdext::is_iterator_v<const int*>);
    static_assert(stdext::is_iterator_v<int* const>);
    static_assert(stdext::is_iterator_v<volatile int*>);
    static_assert(stdext::is_iterator_v<int* volatile>);
    static_assert(stdext::is_iterator_v<fixed_input_iterator>);
    static_assert(stdext::is_iterator_v<fixed_output_iterator>);
    static_assert(stdext::is_iterator_v<input_iterator>);
    static_assert(stdext::is_iterator_v<output_iterator>);
    static_assert(stdext::is_iterator_v<forward_iterator>);
    static_assert(stdext::is_iterator_v<bidirectional_iterator>);
    static_assert(stdext::is_iterator_v<random_access_iterator>);

    // is_input_iterator
    static_assert(!stdext::is_input_iterator<void>::value);
    static_assert(!stdext::is_input_iterator<void*>::value);
    static_assert(!stdext::is_input_iterator<const void*>::value);
    static_assert(!stdext::is_input_iterator<volatile void*>::value);
    static_assert(!stdext::is_input_iterator<const volatile void*>::value);
    static_assert(!stdext::is_input_iterator<int>::value);
    static_assert(!stdext::is_input_iterator<int&>::value);
    static_assert(stdext::is_input_iterator<int*>::value);
    static_assert(!stdext::is_input_iterator<int*&>::value);
    static_assert(!stdext::is_input_iterator<int[]>::value);
    static_assert(stdext::is_input_iterator<int (*)[]>::value);
    static_assert(stdext::is_input_iterator<int (*)[5]>::value);
    static_assert(!stdext::is_input_iterator<void ()>::value);
    static_assert(!stdext::is_input_iterator<void (*)()>::value);
    static_assert(stdext::is_input_iterator<int**>::value);
    static_assert(stdext::is_input_iterator<const int*>::value);
    static_assert(stdext::is_input_iterator<int* const>::value);
    static_assert(stdext::is_input_iterator<volatile int*>::value);
    static_assert(stdext::is_input_iterator<int* volatile>::value);
    static_assert(stdext::is_input_iterator<fixed_input_iterator>::value);
    static_assert(!stdext::is_input_iterator<fixed_output_iterator>::value);
    static_assert(stdext::is_input_iterator<input_iterator>::value);
    static_assert(!stdext::is_input_iterator<output_iterator>::value);
    static_assert(stdext::is_input_iterator<forward_iterator>::value);
    static_assert(stdext::is_input_iterator<bidirectional_iterator>::value);
    static_assert(stdext::is_input_iterator<random_access_iterator>::value);

    // is_input_iterator_v
    static_assert(!stdext::is_input_iterator_v<void>);
    static_assert(!stdext::is_input_iterator_v<void*>);
    static_assert(!stdext::is_input_iterator_v<const void*>);
    static_assert(!stdext::is_input_iterator_v<volatile void*>);
    static_assert(!stdext::is_input_iterator_v<const volatile void*>);
    static_assert(!stdext::is_input_iterator_v<int>);
    static_assert(!stdext::is_input_iterator_v<int&>);
    static_assert(stdext::is_input_iterator_v<int*>);
    static_assert(!stdext::is_input_iterator_v<int*&>);
    static_assert(!stdext::is_input_iterator_v<int[]>);
    static_assert(stdext::is_input_iterator_v<int (*)[]>);
    static_assert(stdext::is_input_iterator_v<int (*)[5]>);
    static_assert(!stdext::is_input_iterator_v<void ()>);
    static_assert(!stdext::is_input_iterator_v<void (*)()>);
    static_assert(stdext::is_input_iterator_v<int**>);
    static_assert(stdext::is_input_iterator_v<const int*>);
    static_assert(stdext::is_input_iterator_v<int* const>);
    static_assert(stdext::is_input_iterator_v<volatile int*>);
    static_assert(stdext::is_input_iterator_v<int* volatile>);
    static_assert(stdext::is_input_iterator_v<fixed_input_iterator>);
    static_assert(!stdext::is_input_iterator_v<fixed_output_iterator>);
    static_assert(stdext::is_input_iterator_v<input_iterator>);
    static_assert(!stdext::is_input_iterator_v<output_iterator>);
    static_assert(stdext::is_input_iterator_v<forward_iterator>);
    static_assert(stdext::is_input_iterator_v<bidirectional_iterator>);
    static_assert(stdext::is_input_iterator_v<random_access_iterator>);

    // is_forward_iterator
    static_assert(!stdext::is_forward_iterator<void>::value);
    static_assert(!stdext::is_forward_iterator<void*>::value);
    static_assert(!stdext::is_forward_iterator<const void*>::value);
    static_assert(!stdext::is_forward_iterator<volatile void*>::value);
    static_assert(!stdext::is_forward_iterator<const volatile void*>::value);
    static_assert(!stdext::is_forward_iterator<int>::value);
    static_assert(!stdext::is_forward_iterator<int&>::value);
    static_assert(stdext::is_forward_iterator<int*>::value);
    static_assert(!stdext::is_forward_iterator<int*&>::value);
    static_assert(!stdext::is_forward_iterator<int[]>::value);
    static_assert(stdext::is_forward_iterator<int (*)[]>::value);
    static_assert(stdext::is_forward_iterator<int (*)[5]>::value);
    static_assert(!stdext::is_forward_iterator<void ()>::value);
    static_assert(!stdext::is_forward_iterator<void (*)()>::value);
    static_assert(stdext::is_forward_iterator<int**>::value);
    static_assert(stdext::is_forward_iterator<const int*>::value);
    static_assert(stdext::is_forward_iterator<int* const>::value);
    static_assert(stdext::is_forward_iterator<volatile int*>::value);
    static_assert(stdext::is_forward_iterator<int* volatile>::value);
    static_assert(!stdext::is_forward_iterator<fixed_input_iterator>::value);
    static_assert(!stdext::is_forward_iterator<fixed_output_iterator>::value);
    static_assert(!stdext::is_forward_iterator<input_iterator>::value);
    static_assert(!stdext::is_forward_iterator<output_iterator>::value);
    static_assert(stdext::is_forward_iterator<forward_iterator>::value);
    static_assert(stdext::is_forward_iterator<bidirectional_iterator>::value);
    static_assert(stdext::is_forward_iterator<random_access_iterator>::value);

    // is_forward_iterator_v
    static_assert(!stdext::is_forward_iterator_v<void>);
    static_assert(!stdext::is_forward_iterator_v<void*>);
    static_assert(!stdext::is_forward_iterator_v<const void*>);
    static_assert(!stdext::is_forward_iterator_v<volatile void*>);
    static_assert(!stdext::is_forward_iterator_v<const volatile void*>);
    static_assert(!stdext::is_forward_iterator_v<int>);
    static_assert(!stdext::is_forward_iterator_v<int&>);
    static_assert(stdext::is_forward_iterator_v<int*>);
    static_assert(!stdext::is_forward_iterator_v<int*&>);
    static_assert(!stdext::is_forward_iterator_v<int[]>);
    static_assert(stdext::is_forward_iterator_v<int (*)[]>);
    static_assert(stdext::is_forward_iterator_v<int (*)[5]>);
    static_assert(!stdext::is_forward_iterator_v<void ()>);
    static_assert(!stdext::is_forward_iterator_v<void (*)()>);
    static_assert(stdext::is_forward_iterator_v<int**>);
    static_assert(stdext::is_forward_iterator_v<const int*>);
    static_assert(stdext::is_forward_iterator_v<int* const>);
    static_assert(stdext::is_forward_iterator_v<volatile int*>);
    static_assert(stdext::is_forward_iterator_v<int* volatile>);
    static_assert(!stdext::is_forward_iterator_v<fixed_input_iterator>);
    static_assert(!stdext::is_forward_iterator_v<fixed_output_iterator>);
    static_assert(!stdext::is_forward_iterator_v<input_iterator>);
    static_assert(!stdext::is_forward_iterator_v<output_iterator>);
    static_assert(stdext::is_forward_iterator_v<forward_iterator>);
    static_assert(stdext::is_forward_iterator_v<bidirectional_iterator>);
    static_assert(stdext::is_forward_iterator_v<random_access_iterator>);

    // is_bidirectional_iterator
    static_assert(!stdext::is_bidirectional_iterator<void>::value);
    static_assert(!stdext::is_bidirectional_iterator<void*>::value);
    static_assert(!stdext::is_bidirectional_iterator<const void*>::value);
    static_assert(!stdext::is_bidirectional_iterator<volatile void*>::value);
    static_assert(!stdext::is_bidirectional_iterator<const volatile void*>::value);
    static_assert(!stdext::is_bidirectional_iterator<int>::value);
    static_assert(!stdext::is_bidirectional_iterator<int&>::value);
    static_assert(stdext::is_bidirectional_iterator<int*>::value);
    static_assert(!stdext::is_bidirectional_iterator<int*&>::value);
    static_assert(!stdext::is_bidirectional_iterator<int[]>::value);
    static_assert(stdext::is_bidirectional_iterator<int (*)[]>::value);
    static_assert(stdext::is_bidirectional_iterator<int (*)[5]>::value);
    static_assert(!stdext::is_bidirectional_iterator<void ()>::value);
    static_assert(!stdext::is_bidirectional_iterator<void (*)()>::value);
    static_assert(stdext::is_bidirectional_iterator<int**>::value);
    static_assert(stdext::is_bidirectional_iterator<const int*>::value);
    static_assert(stdext::is_bidirectional_iterator<int* const>::value);
    static_assert(stdext::is_bidirectional_iterator<volatile int*>::value);
    static_assert(stdext::is_bidirectional_iterator<int* volatile>::value);
    static_assert(!stdext::is_bidirectional_iterator<fixed_input_iterator>::value);
    static_assert(!stdext::is_bidirectional_iterator<fixed_output_iterator>::value);
    static_assert(!stdext::is_bidirectional_iterator<input_iterator>::value);
    static_assert(!stdext::is_bidirectional_iterator<output_iterator>::value);
    static_assert(!stdext::is_bidirectional_iterator<forward_iterator>::value);
    static_assert(stdext::is_bidirectional_iterator<bidirectional_iterator>::value);
    static_assert(stdext::is_bidirectional_iterator<random_access_iterator>::value);

    // is_bidirectional_iterator_v
    static_assert(!stdext::is_bidirectional_iterator_v<void>);
    static_assert(!stdext::is_bidirectional_iterator_v<void*>);
    static_assert(!stdext::is_bidirectional_iterator_v<const void*>);
    static_assert(!stdext::is_bidirectional_iterator_v<volatile void*>);
    static_assert(!stdext::is_bidirectional_iterator_v<const volatile void*>);
    static_assert(!stdext::is_bidirectional_iterator_v<int>);
    static_assert(!stdext::is_bidirectional_iterator_v<int&>);
    static_assert(stdext::is_bidirectional_iterator_v<int*>);
    static_assert(!stdext::is_bidirectional_iterator_v<int*&>);
    static_assert(!stdext::is_bidirectional_iterator_v<int[]>);
    static_assert(stdext::is_bidirectional_iterator_v<int (*)[]>);
    static_assert(stdext::is_bidirectional_iterator_v<int (*)[5]>);
    static_assert(!stdext::is_bidirectional_iterator_v<void ()>);
    static_assert(!stdext::is_bidirectional_iterator_v<void (*)()>);
    static_assert(stdext::is_bidirectional_iterator_v<int**>);
    static_assert(stdext::is_bidirectional_iterator_v<const int*>);
    static_assert(stdext::is_bidirectional_iterator_v<int* const>);
    static_assert(stdext::is_bidirectional_iterator_v<volatile int*>);
    static_assert(stdext::is_bidirectional_iterator_v<int* volatile>);
    static_assert(!stdext::is_bidirectional_iterator_v<fixed_input_iterator>);
    static_assert(!stdext::is_bidirectional_iterator_v<fixed_output_iterator>);
    static_assert(!stdext::is_bidirectional_iterator_v<input_iterator>);
    static_assert(!stdext::is_bidirectional_iterator_v<output_iterator>);
    static_assert(!stdext::is_bidirectional_iterator_v<forward_iterator>);
    static_assert(stdext::is_bidirectional_iterator_v<bidirectional_iterator>);
    static_assert(stdext::is_bidirectional_iterator_v<random_access_iterator>);

    // is_random_access_iterator
    static_assert(!stdext::is_random_access_iterator<void>::value);
    static_assert(!stdext::is_random_access_iterator<void*>::value);
    static_assert(!stdext::is_random_access_iterator<const void*>::value);
    static_assert(!stdext::is_random_access_iterator<volatile void*>::value);
    static_assert(!stdext::is_random_access_iterator<const volatile void*>::value);
    static_assert(!stdext::is_random_access_iterator<int>::value);
    static_assert(!stdext::is_random_access_iterator<int&>::value);
    static_assert(stdext::is_random_access_iterator<int*>::value);
    static_assert(!stdext::is_random_access_iterator<int*&>::value);
    static_assert(!stdext::is_random_access_iterator<int[]>::value);
    static_assert(stdext::is_random_access_iterator<int (*)[]>::value);
    static_assert(stdext::is_random_access_iterator<int (*)[5]>::value);
    static_assert(!stdext::is_random_access_iterator<void ()>::value);
    static_assert(!stdext::is_random_access_iterator<void (*)()>::value);
    static_assert(stdext::is_random_access_iterator<int**>::value);
    static_assert(stdext::is_random_access_iterator<const int*>::value);
    static_assert(stdext::is_random_access_iterator<int* const>::value);
    static_assert(stdext::is_random_access_iterator<volatile int*>::value);
    static_assert(stdext::is_random_access_iterator<int* volatile>::value);
    static_assert(!stdext::is_random_access_iterator<fixed_input_iterator>::value);
    static_assert(!stdext::is_random_access_iterator<fixed_output_iterator>::value);
    static_assert(!stdext::is_random_access_iterator<input_iterator>::value);
    static_assert(!stdext::is_random_access_iterator<output_iterator>::value);
    static_assert(!stdext::is_random_access_iterator<forward_iterator>::value);
    static_assert(!stdext::is_random_access_iterator<bidirectional_iterator>::value);
    static_assert(stdext::is_random_access_iterator<random_access_iterator>::value);

    // is_random_access_iterator_v
    static_assert(!stdext::is_random_access_iterator_v<void>);
    static_assert(!stdext::is_random_access_iterator_v<void*>);
    static_assert(!stdext::is_random_access_iterator_v<const void*>);
    static_assert(!stdext::is_random_access_iterator_v<volatile void*>);
    static_assert(!stdext::is_random_access_iterator_v<const volatile void*>);
    static_assert(!stdext::is_random_access_iterator_v<int>);
    static_assert(!stdext::is_random_access_iterator_v<int&>);
    static_assert(stdext::is_random_access_iterator_v<int*>);
    static_assert(!stdext::is_random_access_iterator_v<int*&>);
    static_assert(!stdext::is_random_access_iterator_v<int[]>);
    static_assert(stdext::is_random_access_iterator_v<int (*)[]>);
    static_assert(stdext::is_random_access_iterator_v<int (*)[5]>);
    static_assert(!stdext::is_random_access_iterator_v<void ()>);
    static_assert(!stdext::is_random_access_iterator_v<void (*)()>);
    static_assert(stdext::is_random_access_iterator_v<int**>);
    static_assert(stdext::is_random_access_iterator_v<const int*>);
    static_assert(stdext::is_random_access_iterator_v<int* const>);
    static_assert(stdext::is_random_access_iterator_v<volatile int*>);
    static_assert(stdext::is_random_access_iterator_v<int* volatile>);
    static_assert(!stdext::is_random_access_iterator_v<fixed_input_iterator>);
    static_assert(!stdext::is_random_access_iterator_v<fixed_output_iterator>);
    static_assert(!stdext::is_random_access_iterator_v<input_iterator>);
    static_assert(!stdext::is_random_access_iterator_v<output_iterator>);
    static_assert(!stdext::is_random_access_iterator_v<forward_iterator>);
    static_assert(!stdext::is_random_access_iterator_v<bidirectional_iterator>);
    static_assert(stdext::is_random_access_iterator_v<random_access_iterator>);

    // is_std_range
    static_assert(!stdext::is_std_range<void>::value);
    static_assert(!stdext::is_std_range<const void>::value);
    static_assert(!stdext::is_std_range<volatile void>::value);
    static_assert(!stdext::is_std_range<const volatile void>::value);
    static_assert(!stdext::is_std_range<void*>::value);
    static_assert(!stdext::is_std_range<int>::value);
    static_assert(!stdext::is_std_range<int&>::value);
    static_assert(!stdext::is_std_range<int*>::value);
    static_assert(!stdext::is_std_range<int*&>::value);
    static_assert(!stdext::is_std_range<int[]>::value);
    static_assert(!stdext::is_std_range<int (&)[]>::value);
    static_assert(!stdext::is_std_range<int (*)[]>::value);
    static_assert(!stdext::is_std_range<int[5]>::value);
    static_assert(stdext::is_std_range<int (&)[5]>::value);
    static_assert(!stdext::is_std_range<int (*)[5]>::value);
    static_assert(stdext::is_std_range<const int (&)[5]>::value);

    // iterator_category
    static_assert(std::is_same_v<stdext::iterator_category<int*>, std::random_access_iterator_tag>);
    static_assert(std::is_same_v<stdext::iterator_category<fixed_input_iterator>, std::input_iterator_tag>);
    static_assert(std::is_same_v<stdext::iterator_category<fixed_output_iterator>, std::output_iterator_tag>);
    static_assert(std::is_same_v<stdext::iterator_category<input_iterator>, std::input_iterator_tag>);
    static_assert(std::is_same_v<stdext::iterator_category<output_iterator>, std::output_iterator_tag>);
    static_assert(std::is_same_v<stdext::iterator_category<forward_iterator>, std::forward_iterator_tag>);
    static_assert(std::is_same_v<stdext::iterator_category<bidirectional_iterator>, std::bidirectional_iterator_tag>);
    static_assert(std::is_same_v<stdext::iterator_category<random_access_iterator>, std::random_access_iterator_tag>);

    // value_type
    static_assert(std::is_same_v<stdext::value_type<int*, stdext::is_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::value_type<const int*, stdext::is_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::value_type<float*, stdext::is_iterator>::type, float>);
    static_assert(std::is_same_v<stdext::value_type<const float*, stdext::is_iterator>::type, float>);
    static_assert(std::is_same_v<stdext::value_type<fixed_input_iterator, stdext::is_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::value_type<fixed_output_iterator, stdext::is_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::value_type<input_iterator, stdext::is_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::value_type<output_iterator, stdext::is_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::value_type<forward_iterator, stdext::is_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::value_type<bidirectional_iterator, stdext::is_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::value_type<random_access_iterator, stdext::is_iterator>::type, int>);

    // value_type_t
    static_assert(std::is_same_v<stdext::value_type_t<int*, stdext::is_iterator>, int>);
    static_assert(std::is_same_v<stdext::value_type_t<const int*, stdext::is_iterator>, int>);
    static_assert(std::is_same_v<stdext::value_type_t<float*, stdext::is_iterator>, float>);
    static_assert(std::is_same_v<stdext::value_type_t<const float*, stdext::is_iterator>, float>);
    static_assert(std::is_same_v<stdext::value_type_t<fixed_input_iterator, stdext::is_iterator>, int>);
    static_assert(std::is_same_v<stdext::value_type_t<fixed_output_iterator, stdext::is_iterator>, void>);
    static_assert(std::is_same_v<stdext::value_type_t<input_iterator, stdext::is_iterator>, int>);
    static_assert(std::is_same_v<stdext::value_type_t<output_iterator, stdext::is_iterator>, void>);
    static_assert(std::is_same_v<stdext::value_type_t<forward_iterator, stdext::is_iterator>, int>);
    static_assert(std::is_same_v<stdext::value_type_t<bidirectional_iterator, stdext::is_iterator>, int>);
    static_assert(std::is_same_v<stdext::value_type_t<random_access_iterator, stdext::is_iterator>, int>);

    // iterator_value_type
    static_assert(std::is_same_v<stdext::iterator_value_type<int*>::type, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<const int*>::type, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<float*>::type, float>);
    static_assert(std::is_same_v<stdext::iterator_value_type<const float*>::type, float>);
    static_assert(std::is_same_v<stdext::iterator_value_type<fixed_input_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<fixed_output_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::iterator_value_type<input_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<output_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::iterator_value_type<forward_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<bidirectional_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<random_access_iterator>::type, int>);

    // iterator_value_type_t
    static_assert(std::is_same_v<stdext::iterator_value_type_t<int*>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type_t<const int*>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type_t<float*>, float>);
    static_assert(std::is_same_v<stdext::iterator_value_type_t<const float*>, float>);
    static_assert(std::is_same_v<stdext::iterator_value_type_t<fixed_input_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type_t<fixed_output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_value_type_t<input_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type_t<output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_value_type_t<forward_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type_t<bidirectional_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type_t<random_access_iterator>, int>);

    // difference_type
    static_assert(std::is_same_v<stdext::difference_type<int*, stdext::is_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type<const int*, stdext::is_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type<float*, stdext::is_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type<const float*, stdext::is_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type<fixed_input_iterator, stdext::is_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type<fixed_output_iterator, stdext::is_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::difference_type<input_iterator, stdext::is_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type<output_iterator, stdext::is_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::difference_type<forward_iterator, stdext::is_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type<bidirectional_iterator, stdext::is_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type<random_access_iterator, stdext::is_iterator>::type, std::ptrdiff_t>);

    // difference_type_t
    static_assert(std::is_same_v<stdext::difference_type_t<int*, stdext::is_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type_t<const int*, stdext::is_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type_t<float*, stdext::is_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type_t<const float*, stdext::is_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type_t<fixed_input_iterator, stdext::is_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type_t<fixed_output_iterator, stdext::is_iterator>, void>);
    static_assert(std::is_same_v<stdext::difference_type_t<input_iterator, stdext::is_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type_t<output_iterator, stdext::is_iterator>, void>);
    static_assert(std::is_same_v<stdext::difference_type_t<forward_iterator, stdext::is_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type_t<bidirectional_iterator, stdext::is_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::difference_type_t<random_access_iterator, stdext::is_iterator>, std::ptrdiff_t>);

    // iterator_difference_type
    static_assert(std::is_same_v<stdext::iterator_difference_type<int*>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<const int*>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<float*>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<const float*>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<fixed_input_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<fixed_output_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<input_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<output_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<forward_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<bidirectional_iterator>::type, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<random_access_iterator>::type, std::ptrdiff_t>);

    // iterator_difference_type_t
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<int*>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<const int*>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<float*>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<const float*>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<fixed_input_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<fixed_output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<input_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<forward_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<bidirectional_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type_t<random_access_iterator>, std::ptrdiff_t>);

    // pointer_type
    static_assert(std::is_same_v<stdext::pointer_type<int*, stdext::is_iterator>::type, int*>);
    static_assert(std::is_same_v<stdext::pointer_type<const int*, stdext::is_iterator>::type, const int*>);
    static_assert(std::is_same_v<stdext::pointer_type<float*, stdext::is_iterator>::type, float*>);
    static_assert(std::is_same_v<stdext::pointer_type<const float*, stdext::is_iterator>::type, const float*>);
    static_assert(std::is_same_v<stdext::pointer_type<fixed_input_iterator, stdext::is_iterator>::type, const int*>);
    static_assert(std::is_same_v<stdext::pointer_type<fixed_output_iterator, stdext::is_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::pointer_type<input_iterator, stdext::is_iterator>::type, const int*>);
    static_assert(std::is_same_v<stdext::pointer_type<output_iterator, stdext::is_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::pointer_type<forward_iterator, stdext::is_iterator>::type, const int*>);
    static_assert(std::is_same_v<stdext::pointer_type<bidirectional_iterator, stdext::is_iterator>::type, const int*>);
    static_assert(std::is_same_v<stdext::pointer_type<random_access_iterator, stdext::is_iterator>::type, const int*>);

    // pointer_type_t
    static_assert(std::is_same_v<stdext::pointer_type_t<int*, stdext::is_iterator>, int*>);
    static_assert(std::is_same_v<stdext::pointer_type_t<const int*, stdext::is_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::pointer_type_t<float*, stdext::is_iterator>, float*>);
    static_assert(std::is_same_v<stdext::pointer_type_t<const float*, stdext::is_iterator>, const float*>);
    static_assert(std::is_same_v<stdext::pointer_type_t<fixed_input_iterator, stdext::is_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::pointer_type_t<fixed_output_iterator, stdext::is_iterator>, void>);
    static_assert(std::is_same_v<stdext::pointer_type_t<input_iterator, stdext::is_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::pointer_type_t<output_iterator, stdext::is_iterator>, void>);
    static_assert(std::is_same_v<stdext::pointer_type_t<forward_iterator, stdext::is_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::pointer_type_t<bidirectional_iterator, stdext::is_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::pointer_type_t<random_access_iterator, stdext::is_iterator>, const int*>);

    // iterator_pointer_type
    static_assert(std::is_same_v<stdext::iterator_pointer_type<int*>::type, int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<const int*>::type, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<float*>::type, float*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<const float*>::type, const float*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<fixed_input_iterator>::type, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<fixed_output_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<input_iterator>::type, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<output_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<forward_iterator>::type, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<bidirectional_iterator>::type, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<random_access_iterator>::type, const int*>);

    // iterator_pointer_type_t
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<int*>, int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<const int*>, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<float*>, float*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<const float*>, const float*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<fixed_input_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<fixed_output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<input_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<forward_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<bidirectional_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type_t<random_access_iterator>, const int*>);

    // reference_type
    static_assert(std::is_same_v<stdext::reference_type<int*, stdext::is_iterator>::type, int&>);
    static_assert(std::is_same_v<stdext::reference_type<const int*, stdext::is_iterator>::type, const int&>);
    static_assert(std::is_same_v<stdext::reference_type<float*, stdext::is_iterator>::type, float&>);
    static_assert(std::is_same_v<stdext::reference_type<const float*, stdext::is_iterator>::type, const float&>);
    static_assert(std::is_same_v<stdext::reference_type<fixed_input_iterator, stdext::is_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::reference_type<fixed_output_iterator, stdext::is_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::reference_type<input_iterator, stdext::is_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::reference_type<output_iterator, stdext::is_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::reference_type<forward_iterator, stdext::is_iterator>::type, const int&>);
    static_assert(std::is_same_v<stdext::reference_type<bidirectional_iterator, stdext::is_iterator>::type, const int&>);
    static_assert(std::is_same_v<stdext::reference_type<random_access_iterator, stdext::is_iterator>::type, const int&>);

    // reference_type_t
    static_assert(std::is_same_v<stdext::reference_type_t<int*, stdext::is_iterator>, int&>);
    static_assert(std::is_same_v<stdext::reference_type_t<const int*, stdext::is_iterator>, const int&>);
    static_assert(std::is_same_v<stdext::reference_type_t<float*, stdext::is_iterator>, float&>);
    static_assert(std::is_same_v<stdext::reference_type_t<const float*, stdext::is_iterator>, const float&>);
    static_assert(std::is_same_v<stdext::reference_type_t<fixed_input_iterator, stdext::is_iterator>, int>);
    static_assert(std::is_same_v<stdext::reference_type_t<fixed_output_iterator, stdext::is_iterator>, void>);
    static_assert(std::is_same_v<stdext::reference_type_t<input_iterator, stdext::is_iterator>, int>);
    static_assert(std::is_same_v<stdext::reference_type_t<output_iterator, stdext::is_iterator>, void>);
    static_assert(std::is_same_v<stdext::reference_type_t<forward_iterator, stdext::is_iterator>, const int&>);
    static_assert(std::is_same_v<stdext::reference_type_t<bidirectional_iterator, stdext::is_iterator>, const int&>);
    static_assert(std::is_same_v<stdext::reference_type_t<random_access_iterator, stdext::is_iterator>, const int&>);

    // iterator_reference_type
    static_assert(std::is_same_v<stdext::iterator_reference_type<int*>::type, int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<const int*>::type, const int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<float*>::type, float&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<const float*>::type, const float&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<fixed_input_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<fixed_output_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<input_iterator>::type, int>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<output_iterator>::type, void>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<forward_iterator>::type, const int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<bidirectional_iterator>::type, const int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<random_access_iterator>::type, const int&>);

    // iterator_reference_type_t
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<int*>, int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<const int*>, const int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<float*>, float&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<const float*>, const float&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<fixed_input_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<fixed_output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<input_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<forward_iterator>, const int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<bidirectional_iterator>, const int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type_t<random_access_iterator>, const int&>);

    namespace { using uptrdiff_t = std::make_unsigned_t<std::ptrdiff_t>; }

    // size_type
    static_assert(std::is_same_v<stdext::size_type<int*, stdext::is_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type<const int*, stdext::is_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type<float*, stdext::is_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type<const float*, stdext::is_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type<fixed_input_iterator, stdext::is_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type<input_iterator, stdext::is_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type<forward_iterator, stdext::is_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type<bidirectional_iterator, stdext::is_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type<random_access_iterator, stdext::is_iterator>::type, uptrdiff_t>);

    // size_type_t
    static_assert(std::is_same_v<stdext::size_type_t<int*, stdext::is_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type_t<const int*, stdext::is_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type_t<float*, stdext::is_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type_t<const float*, stdext::is_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type_t<fixed_input_iterator, stdext::is_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type_t<input_iterator, stdext::is_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type_t<forward_iterator, stdext::is_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type_t<bidirectional_iterator, stdext::is_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::size_type_t<random_access_iterator, stdext::is_iterator>, uptrdiff_t>);

    // iterator_size_type
    static_assert(std::is_same_v<stdext::iterator_size_type<int*>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<const int*>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<float*>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<const float*>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<fixed_input_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<input_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<forward_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<bidirectional_iterator>::type, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<random_access_iterator>::type, uptrdiff_t>);

    // iterator_size_type_t
    static_assert(std::is_same_v<stdext::iterator_size_type_t<int*>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type_t<const int*>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type_t<float*>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type_t<const float*>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type_t<fixed_input_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type_t<input_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type_t<forward_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type_t<bidirectional_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type_t<random_access_iterator>, uptrdiff_t>);

    // iterator_proxy
    static_assert(std::is_same_v<stdext::iterator_proxy<int*>::value_type, int>);
    static_assert(std::is_same_v<stdext::iterator_proxy<const int*>::value_type, int>);
    static_assert(std::is_same_v<stdext::iterator_proxy<float*>::value_type, float>);
    static_assert(std::is_same_v<stdext::iterator_proxy<const float*>::value_type, float>);
    static_assert(std::is_same_v<stdext::iterator_proxy<fixed_input_iterator>::value_type, int>);
    static_assert(std::is_same_v<stdext::iterator_proxy<input_iterator>::value_type, int>);
    static_assert(std::is_same_v<stdext::iterator_proxy<forward_iterator>::value_type, int>);
    static_assert(std::is_same_v<stdext::iterator_proxy<bidirectional_iterator>::value_type, int>);
    static_assert(std::is_same_v<stdext::iterator_proxy<random_access_iterator>::value_type, int>);

    TEST_CASE("iterator_proxy", "[iterator]")
    {
        int x = 5;
        CHECK(*stdext::iterator_proxy<int*>(x) == 5);
        CHECK(*stdext::iterator_proxy<int*>(21) == 21);
    }
}
