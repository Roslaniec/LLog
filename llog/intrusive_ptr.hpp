#ifndef LINKO_INTRUSIVE_PTR_HPP
#define LINKO_INTRUSIVE_PTR_HPP

// 
// Copyright (C) 2024 Karol Roslaniec <llog@roslaniec.net>
//
// This file is derived from boost/intrusive_ptr.hpp:
//  Copyright (c) 2001-2009, 2012 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <functional>
#include <cassert>
#include <iosfwd>

namespace linko
{

template<typename T>
class intrusive_ptr
{
private:
    typedef intrusive_ptr this_type;

public:
    typedef T element_type;

    constexpr intrusive_ptr() noexcept : px(nullptr) { }

    ~intrusive_ptr()
    {
        if (px != nullptr) intrusive_ptr_release(px);
    }

    intrusive_ptr(T *p, bool add_ref = true) : px(p)
    {
        if (px != nullptr && add_ref) intrusive_ptr_add_ref(px);
    }

    intrusive_ptr(intrusive_ptr const &rhs) : px(rhs.px)
    { 
        if (px != nullptr) intrusive_ptr_add_ref(px);
    }

    template<class U>
    intrusive_ptr(intrusive_ptr<U> const &rhs,
                  typename std::enable_if<
                  std::is_convertible<U, T>::value>::type* = nullptr)
        : px(rhs.get())
    {
        if (px != nullptr) intrusive_ptr_add_ref(px);
    }
    
    intrusive_ptr(intrusive_ptr &&rhs) noexcept : px(rhs.px)
    {
        rhs.px = nullptr;
    }

    template<class U> friend class intrusive_ptr;

    template<class U>
    intrusive_ptr(intrusive_ptr<U> &&rhs,
                  typename std::enable_if<
                  std::is_convertible<U, T>::value>::type* = nullptr)
        : px(rhs.px)
    {
        rhs.px = nullptr;
    }

    intrusive_ptr &operator=(T *rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    intrusive_ptr &operator=(intrusive_ptr const &rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    template<typename U>
    intrusive_ptr &operator=(intrusive_ptr<U> const &rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    intrusive_ptr &operator=(intrusive_ptr &&rhs) noexcept
    {
        this_type(static_cast<intrusive_ptr &&>(rhs)).swap(*this);
        return *this;
    }

    template<class U>
    intrusive_ptr &operator=(intrusive_ptr<U> &&rhs) noexcept
    {
        this_type(static_cast<intrusive_ptr<U> &&>(rhs)).swap(*this);
        return *this;
    }

    void reset()
    {
        this_type().swap(*this);
    }

    void reset(T *rhs)
    {
        this_type(rhs).swap(*this);
    }

    void reset(T *rhs, bool add_ref)
    {
        this_type(rhs, add_ref).swap(*this);
    }

    T *get() const noexcept
    {
        return px;
    }

    T *detach() noexcept
    {
        T * ret = px;
        px = nullptr;
        return ret;
    }
    
    T &operator*() const noexcept
    {
        assert(px != nullptr);
        return *px;
    }

    T *operator->() const noexcept
    {
        assert(px != nullptr);
        return px;
    }

    explicit operator bool() const noexcept
    {
        return px != nullptr;
    }

    bool operator!() const noexcept
    {
        return px == nullptr;
    }

    void swap(intrusive_ptr &rhs) noexcept
    {
        T *tmp = px;
        px = rhs.px;
        rhs.px = tmp;
    }

private:
    T *px;
};


template<class T, class U>
inline bool
operator==(intrusive_ptr<T> const &a, intrusive_ptr<U> const &b) noexcept
{
    return a.get() == b.get();
}

template<class T, class U>
inline bool
operator!=(intrusive_ptr<T> const &a, intrusive_ptr<U> const &b) noexcept
{
    return a.get() != b.get();
}

template<class T, class U>
inline bool
operator==(intrusive_ptr<T> const &a, U *b) noexcept
{
    return a.get() == b;
}

template<class T, class U>
inline bool
operator!=(intrusive_ptr<T> const &a, U *b) noexcept
{
    return a.get() != b;
}

template<class T, class U>
inline bool
operator==(T *a, intrusive_ptr<U> const &b) noexcept
{
    return a == b.get();
}

template<class T, class U>
inline bool
operator!=(T *a, intrusive_ptr<U> const &b) noexcept
{
    return a != b.get();
}

template<class T>
inline bool
operator==(intrusive_ptr<T> const &p, std::nullptr_t) noexcept
{
    return p.get() == nullptr;
}

template<class T>
inline bool
operator==(std::nullptr_t, intrusive_ptr<T> const &p) noexcept
{
    return p.get() == nullptr;
}

template<class T>
inline bool
operator!=(intrusive_ptr<T> const & p, std::nullptr_t) noexcept
{
    return p.get() != nullptr;
}

template<class T>
inline bool
operator!=(std::nullptr_t, intrusive_ptr<T> const &p) noexcept
{
    return p.get() != nullptr;
}

template<class T>
inline bool
operator<(intrusive_ptr<T> const &a, intrusive_ptr<T> const &b) noexcept
{
    return std::less<T *>()(a.get(), b.get());
}

template<class T>
void
swap(intrusive_ptr<T> &lhs, intrusive_ptr<T> &rhs) noexcept
{
    lhs.swap(rhs);
}

template<class T>
T *
get_pointer(intrusive_ptr<T> const &p) noexcept
{
    return p.get();
}

template<class T, class U>
intrusive_ptr<T>
static_pointer_cast(intrusive_ptr<U> const &p)
{
    return static_cast<T *>(p.get());
}

template<class T, class U>
intrusive_ptr<T>
const_pointer_cast(intrusive_ptr<U> const &p)
{
    return const_cast<T *>(p.get());
}

template<class T, class U>
intrusive_ptr<T>
dynamic_pointer_cast(intrusive_ptr<U> const &p)
{
    return dynamic_cast<T *>(p.get());
}

template<class T, class U>
intrusive_ptr<T>
static_pointer_cast(intrusive_ptr<U> && p) noexcept
{
    return intrusive_ptr<T>(static_cast<T*>(p.detach()), false);
}

template<class T, class U>
intrusive_ptr<T>
const_pointer_cast(intrusive_ptr<U> &&p) noexcept
{
    return intrusive_ptr<T>(const_cast<T*>(p.detach()), false);
}

template<class T, class U>
intrusive_ptr<T>
dynamic_pointer_cast(intrusive_ptr<U> &&p) noexcept
{
    T * p2 = dynamic_cast<T*>(p.get());
    intrusive_ptr<T> r(p2, false);
    if (p2) p.detach();
    return r;
}

// template<class Y> std::ostream &
// operator<<(std::ostream &os, intrusive_ptr<Y> const &p)
// {
//     os << p.get();
//     return os;
// }

template<class E, class T, class Y>
std::basic_ostream<E, T> &
operator<<(std::basic_ostream<E, T> &os, intrusive_ptr<Y> const &p)
{
    os << p.get();
    return os;
}


// template<class T> struct hash;

template<class T>
std::size_t
hash_value(intrusive_ptr<T> const &p) noexcept
{
    // return ::std::hash<T *>()(p.get());
    return hash_value(p.get());
}

} // namespace linko


namespace std
{

template<class T>
struct hash<::linko::intrusive_ptr<T> >
{
    std::size_t operator()(::linko::intrusive_ptr<T> const & p) const noexcept
    {
        return std::hash<T *>()(p.get());
    }
};

} // namespace std

#endif
