#pragma once

#include <type_traits>
#include <utility>
#include <cstddef>
#include <string>

template <typename T, size_t I, bool ebo = std::is_empty_v<T> && !std::is_final_v<T>>
struct CompressedPairElement {
    CompressedPairElement() noexcept {};

    template<typename U>
    CompressedPairElement(U&& value) noexcept : value_(std::forward<U>(value)) {
    }

    T& Get() noexcept {
        return value_;
    }
    const T& Get() const noexcept {
        return value_;
    }
    T value_{};
};

template <typename T, size_t I>
struct CompressedPairElement<T, I, true> : public T {
    CompressedPairElement() noexcept {};

    CompressedPairElement(T&&) noexcept {
    }

    T& Get() noexcept {
        return *this;
    }

    const T& Get() const noexcept {
        return *this;
    }
};

template <typename F, typename S>
class CompressedPair : private CompressedPairElement<F, 0>, private CompressedPairElement<S, 1> {
public:
    CompressedPair() noexcept {};

    template<typename U, typename R>
    CompressedPair(U&& first, R&& second) noexcept
        : CompressedPairElement<F, 0>(std::forward<U>(first)),
          CompressedPairElement<S, 1>(std::forward<R>(second)) {
    }

    F& GetFirst() noexcept {
        return CompressedPairElement<F, 0>::Get();
    }

    const F& GetFirst() const noexcept {
        return CompressedPairElement<F, 0>::Get();
    }

    S& GetSecond() noexcept {
        return CompressedPairElement<S, 1>::Get();
    }

    const S& GetSecond() const noexcept {
        return CompressedPairElement<S, 1>::Get();
    }
};
