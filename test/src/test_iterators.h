#include <iterator>

#include <cstddef>


namespace test
{
    class input_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = int;
        using pointer = const int*;
        using reference = int;
        using iterator_category = std::input_iterator_tag;

    public:
        friend bool operator == (const input_iterator& a, const input_iterator& b) noexcept { return &a == &b; }
        friend bool operator != (const input_iterator& a, const input_iterator& b) noexcept { return &a != &b; }

    public:
        reference operator * () const noexcept { return _value; }
        pointer operator -> () const noexcept { return &_value; }
        input_iterator& operator ++ () noexcept { ++_value; return *this; }
        stdext::iterator_proxy<input_iterator> operator ++ (int) noexcept { return _value++; }

    private:
        static inline int _value;
    };

    class output_iterator
    {
    public:
        using difference_type = void;
        using value_type = void;
        using pointer = void;
        using reference = void;
        using iterator_category = std::output_iterator_tag;

    public:
        static inline int value;

    public:
        int& operator * () const noexcept { return value; }
        output_iterator& operator ++ () noexcept { return *this; }
        const output_iterator& operator ++ (int) noexcept { return *this; }
    };

    class forward_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = int;
        using pointer = const int*;
        using reference = const int&;
        using iterator_category = std::forward_iterator_tag;

    public:
        forward_iterator() = default;
        explicit forward_iterator(const int* p) noexcept : _p(p) { }

        friend bool operator == (const forward_iterator& a, const forward_iterator& b) noexcept { return a._p == b._p; }
        friend bool operator != (const forward_iterator& a, const forward_iterator& b) noexcept { return a._p != b._p; }

    public:
        reference operator * () const noexcept { return *_p; }
        pointer operator -> () const noexcept { return _p; }
        forward_iterator& operator ++ () noexcept { ++_p; return *this; }
        forward_iterator operator ++ (int) noexcept { auto tmp = *this; ++_p; return tmp; }

    private:
        const int* _p = nullptr;
    };

    class bidirectional_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = int;
        using pointer = const int*;
        using reference = const int&;
        using iterator_category = std::bidirectional_iterator_tag;

    public:
        bidirectional_iterator() = default;
        explicit bidirectional_iterator(const int* p) noexcept : _p(p) { }

        friend bool operator == (const bidirectional_iterator& a, const bidirectional_iterator& b) noexcept { return a._p == b._p; }
        friend bool operator != (const bidirectional_iterator& a, const bidirectional_iterator& b) noexcept { return a._p != b._p; }

    public:
        reference operator * () const noexcept { return *_p; }
        pointer operator -> () const noexcept { return _p; }
        bidirectional_iterator& operator ++ () noexcept { ++_p; return *this; }
        bidirectional_iterator operator ++ (int) noexcept { auto tmp = *this; ++_p; return tmp; }
        bidirectional_iterator& operator -- () noexcept { --_p; return *this; }
        bidirectional_iterator operator -- (int) noexcept { auto tmp = *this; --_p; return tmp; }

    private:
        const int* _p = nullptr;
    };

    class random_access_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = int;
        using pointer = const int*;
        using reference = const int&;
        using iterator_category = std::random_access_iterator_tag;

    public:
        random_access_iterator() = default;
        explicit random_access_iterator(const int* p) noexcept : _p(p) { }

        friend bool operator == (const random_access_iterator& a, const random_access_iterator& b) noexcept { return a._p == b._p; }
        friend bool operator != (const random_access_iterator& a, const random_access_iterator& b) noexcept { return a._p != b._p; }
        friend bool operator <  (const random_access_iterator& a, const random_access_iterator& b) noexcept { return a._p < b._p; }
        friend bool operator >  (const random_access_iterator& a, const random_access_iterator& b) noexcept { return a._p > b._p; }
        friend bool operator <= (const random_access_iterator& a, const random_access_iterator& b) noexcept { return a._p <= b._p; }
        friend bool operator >= (const random_access_iterator& a, const random_access_iterator& b) noexcept { return a._p >= b._p; }

    public:
        reference operator * () const noexcept { return *_p; }
        pointer operator -> () const noexcept { return _p; }
        random_access_iterator& operator ++ () noexcept { ++_p; return *this; }
        random_access_iterator operator ++ (int) noexcept { auto tmp = *this; ++_p; return tmp; }
        random_access_iterator& operator -- () noexcept { --_p; return *this; }
        random_access_iterator operator -- (int) noexcept { auto tmp = *this; --_p; return tmp; }
        random_access_iterator& operator += (difference_type n) noexcept { _p += n; return *this; }
        random_access_iterator& operator -= (difference_type n) noexcept { _p += n; return *this; }
        reference operator [] (difference_type n) const noexcept { return _p[n]; }

        friend random_access_iterator operator + (const random_access_iterator& a, difference_type n) noexcept { return stdext::copy(a) += n; }
        friend random_access_iterator operator + (difference_type n, const random_access_iterator& a) noexcept { return stdext::copy(a) += n; }
        friend random_access_iterator operator - (const random_access_iterator& a, difference_type n) noexcept { return stdext::copy(a) -= n; }
        friend difference_type operator - (const random_access_iterator& a, const random_access_iterator& b) noexcept { return a._p - b._p; }

    private:
        const int* _p = nullptr;
    };
}

