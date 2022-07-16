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

    // iterator_value_type
    static_assert(std::is_same_v<stdext::iterator_value_type<int*>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<const int*>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<float*>, float>);
    static_assert(std::is_same_v<stdext::iterator_value_type<const float*>, float>);
    static_assert(std::is_same_v<stdext::iterator_value_type<fixed_input_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<fixed_output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_value_type<input_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_value_type<forward_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<bidirectional_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_value_type<random_access_iterator>, int>);

    // iterator_difference_type
    static_assert(std::is_same_v<stdext::iterator_difference_type<int*>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<const int*>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<float*>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<const float*>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<fixed_input_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<fixed_output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<input_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<forward_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<bidirectional_iterator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_difference_type<random_access_iterator>, std::ptrdiff_t>);

    // iterator_pointer_type
    static_assert(std::is_same_v<stdext::iterator_pointer_type<int*>, int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<const int*>, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<float*>, float*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<const float*>, const float*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<fixed_input_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<fixed_output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<input_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<forward_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<bidirectional_iterator>, const int*>);
    static_assert(std::is_same_v<stdext::iterator_pointer_type<random_access_iterator>, const int*>);

    // iterator_reference_type
    static_assert(std::is_same_v<stdext::iterator_reference_type<int*>, int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<const int*>, const int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<float*>, float&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<const float*>, const float&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<fixed_input_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<fixed_output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<input_iterator>, int>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<output_iterator>, void>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<forward_iterator>, const int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<bidirectional_iterator>, const int&>);
    static_assert(std::is_same_v<stdext::iterator_reference_type<random_access_iterator>, const int&>);

    namespace { using uptrdiff_t = std::make_unsigned_t<std::ptrdiff_t>; }

    // iterator_size_type
    static_assert(std::is_same_v<stdext::iterator_size_type<int*>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<const int*>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<float*>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<const float*>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<fixed_input_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<input_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<forward_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<bidirectional_iterator>, uptrdiff_t>);
    static_assert(std::is_same_v<stdext::iterator_size_type<random_access_iterator>, uptrdiff_t>);

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
