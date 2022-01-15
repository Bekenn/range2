#include <iterator>

#include <cstddef>


namespace test
{
    class fixed_input_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = int;
        using pointer = const int*;
        using reference = int;
        using iterator_category = std::input_iterator_tag;

    public:
        static inline int value;

    public:
        friend bool operator == (const fixed_input_iterator& a, const fixed_input_iterator& b) noexcept { return true; }
        friend bool operator != (const fixed_input_iterator& a, const fixed_input_iterator& b) noexcept { return false; }

    public:
        reference operator * () const noexcept { return value; }
        pointer operator -> () const noexcept { return &value; }
        fixed_input_iterator& operator ++ () noexcept { ++value; return *this; }
        stdext::iterator_proxy<fixed_input_iterator> operator ++ (int) noexcept { return value++; }
    };

    class fixed_output_iterator
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
        fixed_output_iterator& operator ++ () noexcept { return *this; }
        const fixed_output_iterator& operator ++ (int) noexcept { return *this; }
    };

    class input_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = int;
        using pointer = const int*;
        using reference = int;
        using iterator_category = std::input_iterator_tag;

    public:
        explicit input_iterator(const int* p) noexcept : _p(p) { }
        friend bool operator == (const input_iterator& a, const input_iterator& b) noexcept { return a._p == b._p; }
        friend bool operator != (const input_iterator& a, const input_iterator& b) noexcept { return a._p != b._p; }

    public:
        reference operator * () const noexcept { return *_p; }
        pointer operator -> () const noexcept { return _p; }
        input_iterator& operator ++ () noexcept { ++_p; return *this; }
        const int* operator ++ (int) noexcept { return _p++; }

    private:
        const int* _p = nullptr;
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
        explicit output_iterator(int* p) noexcept : _p(p) { }

    public:
        int& operator * () const noexcept { return *_p; }
        output_iterator& operator ++ () noexcept { ++_p; return *this; }
        output_iterator operator ++ (int) noexcept { return output_iterator(_p++); }

    private:
        int* _p;
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
        forward_iterator operator ++ (int) noexcept { return forward_iterator(_p++); }

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
        bidirectional_iterator operator ++ (int) noexcept { return bidirectional_iterator(_p++); }
        bidirectional_iterator& operator -- () noexcept { --_p; return *this; }
        bidirectional_iterator operator -- (int) noexcept { return bidirectional_iterator(_p--); }

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
        random_access_iterator operator ++ (int) noexcept { return random_access_iterator(_p++); }
        random_access_iterator& operator -- () noexcept { --_p; return *this; }
        random_access_iterator operator -- (int) noexcept { return random_access_iterator(_p--); }
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
