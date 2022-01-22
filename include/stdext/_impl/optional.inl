namespace stdext
{
    template <typename T>
    template <typename... Args>
    T& _private::optional_storage_base<T, false>::emplace(Args&&... args)
    {
        assert(!has_value);

        auto p = new ((void*)&value) T(stdext::forward<Args>(args)...);
        has_value = true;
        return *p;
    }

    template <typename T>
    void _private::optional_storage_base<T, false>::reset() noexcept
    {
        if (has_value)
            value.~T();

        has_value = false;
    }

    template <typename T>
    template <typename... Args>
    T& _private::optional_storage_base<T, true>::emplace(Args&&... args)
    {
        assert(!has_value);

        auto p = new ((void*)&value) T(stdext::forward<Args>(args)...);
        has_value = true;
        return *p;
    }

    template <typename T>
    void _private::optional_storage_base<T, true>::reset() noexcept
    {
        has_value = false;
    }

    template <typename T>
    auto _private::optional_storage<T>::operator = (const optional_storage& rhs) -> optional_storage&
    {
        if (this->has_value && rhs.has_value)
            this->value = rhs.value;
        else if (rhs.has_value)
            this->emplace(rhs.value);
        else if (this->has_value)
            this->reset();

        return *this;
    }

    template <typename T>
    auto _private::optional_storage<T>::operator = (optional_storage&& rhs)
        noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
        -> optional_storage&
    {
        if (this->has_value && rhs.has_value)
            this->value = stdext::move(rhs.value);
        else if (rhs.has_value)
            this->emplace(stdext::move(rhs.value));
        else if (this->has_value)
            this->reset();

        return *this;
    }

    template <typename T>
    template <typename U,
        STDEXT_REQUIRED(!std::is_same_v<optional<T>, remove_cvref_t<U>>
                        && (!std::is_scalar_v<T> || !std::is_same_v<T, std::decay_t<U>>)
                        && std::is_constructible_v<T, U>
                        && std::is_assignable_v<T&, U>)>
    auto optional<T>::operator=(U&& v) -> optional&
    {
        if (_storage.has_value)
            _storage.value = stdext::forward<U>(v);
        else
            _storage.emplace(stdext::forward<U>(v));

        return *this;
    }

    template <typename T>
    template <typename U,
        STDEXT_REQUIRED(std::is_constructible_v<T, const U&>
                        && std::is_assignable_v<T&, const U&>
                        && !_private::is_convertible_from_optional_v<T, U>
                        && !_private::is_assignable_from_optional_v<T, U>)>
    auto optional<T>::operator=(const optional<U>& rhs) -> optional&
    {
        if (_storage.has_value && rhs.has_value())
            _storage.value = rhs.value();
        else if (rhs.has_value())
            _storage.emplace(rhs.value());
        else if (_storage.has_value)
            _storage.reset();

        return *this;
    }

    template <typename T>
    template <typename U,
        STDEXT_REQUIRED(std::is_constructible_v<T, U>
                        && std::is_assignable_v<T&, U>
                        && !_private::is_convertible_from_optional_v<T, U>
                        && !_private::is_assignable_from_optional_v<T, U>)>
    auto optional<T>::operator=(optional<U>&& rhs) -> optional&
    {
        if (_storage.has_value && rhs.has_value())
            _storage.value = stdext::move(rhs.value());
        else if (rhs.has_value())
            _storage.emplace(stdext::move(rhs.value()));
        else if (_storage.has_value)
            _storage.reset();

        return *this;
    }

    template <typename T>
    template <typename... Args>
    T& optional<T>::emplace(Args&&... args)
    {
        if (_storage.has_value)
            _storage.reset();

        return _storage.emplace(stdext::forward<Args>(args)...);
    }

    template <typename T>
    template <typename U, typename... Args,
        STDEXT_REQUIRED(std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>)>
    T& optional<T>::emplace(std::initializer_list<U> il, Args&&... args)
    {
        if (_storage.has_value)
            _storage.reset();

        return _storage.emplace(il, stdext::forward<Args>(args)...);
    }

    template <typename T>
    void optional<T>::swap(optional& rhs) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_swappable_v<T>)
    {
        if (_storage.has_value && rhs._storage.has_value)
            stdext::swap(_storage.value, rhs._storage.value);
        else if (rhs._storage.has_value)
        {
            _storage.emplace(stdext::move(rhs._storage.value));
            rhs._storage.reset();
        }
        else if (_storage.has_value)
        {
            rhs._storage.emplace(stdext::move(_storage.value));
            _storage.reset();
        }
    }

    template <typename T>
    template <typename U,
        STDEXT_REQUIRED(std::is_move_constructible_v<U>)>
    T optional<T>::exchange(nullopt_t)
    {
        if (!_storage.has_value)
            throw bad_optional_access();

        T old_value = stdext::move(_storage.value);
        _storage.reset();
        return old_value;
    }

    template <typename T>
    template <typename... Args,
        STDEXT_REQUIRED(std::is_constructible_v<T, Args...>
                        && std::is_move_constructible_v<T>)>
    T optional<T>::exchange(in_place_t, Args&&... args)
    {
        if (!_storage.has_value)
            throw bad_optional_access();

        T old_value(stdext::move(_storage.value));
        _storage.reset();
        _storage.emplace(stdext::forward<Args>(args)...);
        return old_value;
    }

    template <typename T>
    template <typename U, typename... Args,
        STDEXT_REQUIRED(std::is_constructible_v<T, std::initializer_list<U>&, Args...>
                        && std::is_move_constructible_v<T>)>
    T optional<T>::exchange(in_place_t, std::initializer_list<U> il, Args&&... args)
    {
        if (!_storage.has_value)
            throw bad_optional_access();

        T old_value(stdext::move(_storage.value));
        _storage.reset();
        _storage.emplace(il, stdext::forward<Args>(args)...);
        return old_value;
    }

    template <typename T>
    template <typename U,
        STDEXT_REQUIRED(!std::is_same_v<optional<T>, remove_cvref_t<U>>
                        && !std::is_same_v<remove_cvref_t<U>, in_place_t>
                        && std::is_move_constructible_v<T>
                        && std::is_assignable_v<T&, U>)>
    T stdext::optional<T>::exchange(U&& v)
    {
        if (!_storage.has_value)
            throw bad_optional_access();

        T old_value = stdext::move(_storage.value);
        _storage.value = stdext::forward<U>(v);
        return old_value;
    }

    template <typename T>
    template <typename U,
        STDEXT_REQUIRED(std::is_assignable_v<T&, const U&>
                        && std::is_move_constructible_v<T>
                        && !_private::is_convertible_from_optional_v<T, U>
                        && !_private::is_assignable_from_optional_v<T, U>)>
    T optional<T>::exchange(const optional<U>& other)
    {
        if (!_storage.has_value)
            throw bad_optional_access();

        T old_value = stdext::move(_storage.value);
        if (other.has_value())
            _storage.value = other.value();
        else
            _storage.reset();

        return old_value;
    }

    template <typename T>
    template <typename U,
        STDEXT_REQUIRED(std::is_assignable_v<T&, U>
                        && std::is_move_constructible_v<T>
                        && !_private::is_convertible_from_optional_v<T, U>
                        && !_private::is_assignable_from_optional_v<T, U>)>
    T optional<T>::exchange(optional<U>&& other)
    {
        if (!_storage.has_value)
            throw bad_optional_access();

        T old_value = stdext::move(_storage.value);
        if (other.has_value())
            _storage.value = stdext::move(other.value());
        else
            _storage.reset();

        return old_value;
    }

    template <typename T>
    template <typename U>
    constexpr T optional<T>::value_or(U&& v) const &
    {
        static_assert(std::is_copy_constructible_v<T> && std::is_convertible_v<U&&, T>);
        return _storage.has_value ? _storage.value : static_cast<T>(stdext::forward<U>(v));
    }

    template <typename T>
    template <typename U>
    constexpr T optional<T>::value_or(U&& v) &&
    {
        static_assert(std::is_move_constructible_v<T> && std::is_convertible_v<U&&, T>);
        return _storage.has_value ? stdext::move(_storage.value) : static_cast<T>(stdext::forward<U>(v));
    }
}
