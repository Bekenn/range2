#ifndef IOLIB_GENERATOR_INCLUDED
#define IOLIB_GENERATOR_INCLUDED
#pragma once

#include "optional.h"
#include <iterator>


// A generator is a function object whose call operator takes zero arguments
// and whose return type is not void.  Generators are Destructible, CopyConstructible,
// and CopyAssignable.

// A bounded generator is a generator that produces a finite sequence of values and
// whose return type is optional<T>.  The end of the sequence is reached when !v.has_value()
// for a returned value v.

namespace iolib
{
	template <class Iterator>
	class iterator_generator
	{
	public:
		iterator_generator() noexcept
			: i() { }
		explicit iterator_generator(Iterator i) noexcept
			: i(i) { }

	public:
		auto operator () ()
		{
			return *i++;
		}

	private:
		Iterator i;
	};

	template <class Generator>
	class generator_iterator
		: public ::std::iterator<::std::input_iterator_tag, const decltype(::std::declval<Generator>()())>
	{
	private:
		using base_type = ::std::iterator<::std::input_iterator_tag, const decltype(::std::declval<Generator>()())>;
	public:
		using value_type = typename base_type::value_type;
		using pointer = typename base_type::pointer;
		using reference = typename base_type::reference;
		struct proxy
		{
			value_type value;
			value_type operator * () { return ::std::move(value); }
		};

	public:
		generator_iterator() noexcept
			: g(nullptr) { }
		explicit generator_iterator(Generator& g) noexcept
			: g(::std::addressof(g)) { ++*this; }

		friend bool operator == (const generator_iterator& a, const generator_iterator& b) noexcept
		{
			return a.g == nullptr && b.g == nullptr ? true
				: a.g == nullptr || b.g == nullptr ? false
				: a.value == b.value;
		}
		friend bool operator != (const generator_iterator& a, const generator_iterator& b) noexcept
		{
			return !(a == b);
		}

	public:
		reference operator * () noexcept { return value; }
		pointer operator -> () noexcept { return &value; }
		generator_iterator& operator ++ () { value = (*g)(); }
		proxy operator ++ (int)
		{
			proxy result = { ::std::move(value) };
			++*this;
			return result;
		}

	private:
		Generator* g;
		value_type value;
	};

	template <class Iterator>
	class bounded_iterator_generator
	{
	public:
		using result_type = optional<typename ::std::iterator_traits<Iterator>::value_type>;
	public:
		bounded_iterator_generator() noexcept
			: f(), l() { }
		bounded_iterator_generator(Iterator i, Iterator j) noexcept
			: f(i), l(j) { }

	public:
		result_type operator () ()
		{
			if (f == l)
				return nullopt;
			return result_type(*f++);
		}

	private:
		Iterator f, l;
	};

	namespace detail
	{
		template <class Optional> struct optional_result_type { };
		template <class T>
		struct optional_result_type<optional<T>>
		{
			using type = T;
		};

		template <class Optional>
		using optional_result_type_t = typename optional_result_type<Optional>::type;
	};

	template <class Generator>
	class bounded_generator_iterator
		: public ::std::iterator<::std::input_iterator_tag, const detail::optional_result_type_t<decltype(::std::declval<Generator>()())>>
	{
	private:
		using base_type = ::std::iterator<::std::input_iterator_tag, const detail::optional_result_type_t<decltype(::std::declval<Generator>()())>>;
	public:
		using value_type = typename base_type::value_type;
		using pointer = typename base_type::pointer;
		using reference = typename base_type::reference;
		struct proxy
		{
			value_type value;
			value_type operator * () { return ::std::move(value); }
		};

	public:
		bounded_generator_iterator() noexcept
			: g(nullptr) { }
		explicit bounded_generator_iterator(Generator& g) noexcept
			: g(::std::addressof(g)) { ++*this; }

		friend bool operator == (const bounded_generator_iterator& a, const bounded_generator_iterator& b) noexcept
		{
			return a.g == nullptr && b.g == nullptr ? true
				: a.g == nullptr || b.g == nullptr ? false
				: a.value == b.value;
		}
		friend bool operator != (const bounded_generator_iterator& a, const bounded_generator_iterator& b) noexcept
		{
			return !(a == b);
		}

	public:
		reference operator * () noexcept { return value.value(); }
		pointer operator -> () noexcept { return &value.value(); }
		bounded_generator_iterator& operator ++ ()
		{
			value = (*g)();
			if (!value.has_value())
				g = nullptr;
		}
		proxy operator ++ (int)
		{
			proxy result = { ::std::move(value.value()) };
			++*this;
			return result;
		}

	private:
		Generator* g;
		optional<value_type> value;
	};
}

#endif
