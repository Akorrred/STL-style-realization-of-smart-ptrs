#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <string>
#include <compressed_pair.h>
#include <tuple>


template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
private:
    CompressedPair<T*, Deleter> pair_;

    template<typename U, typename P>
    friend class UniquePtr;

public:
    explicit UniquePtr(T* new_ptr = nullptr) noexcept {
        pair_.GetFirst() = new_ptr;
    }

    UniquePtr(UniquePtr& other) = delete;


    UniquePtr(T* new_ptr, Deleter deleter) noexcept : pair_(new_ptr, std::forward<Deleter>(deleter)){}

    template<typename U, typename P>
    UniquePtr(UniquePtr<U, P>&& other) noexcept :
          pair_(other.pair_.GetFirst(), std::forward<P>(other.pair_.GetSecond())) {
        other.pair_.GetFirst() = nullptr;
    }

    UniquePtr& operator=(std::nullptr_t) noexcept {
        auto tmp = pair_.GetFirst();
        pair_.GetFirst() = nullptr;
        GetDeleter()(tmp);
        return *this;
    }

    UniquePtr& operator=(UniquePtr& other) = delete;

    UniquePtr& operator=(UniquePtr&& other_ptr) noexcept {
        if (other_ptr.pair_.GetFirst() == pair_.GetFirst()) {
            return *this;
        }
        auto tmp = pair_.GetFirst();
        pair_.GetFirst() = other_ptr.pair_.GetFirst();
        GetDeleter()(tmp);
        other_ptr.pair_.GetFirst() = nullptr;
        pair_.GetSecond() = std::forward<Deleter>(other_ptr.pair_.GetSecond());
        return *this;
    }

    ~UniquePtr() {
        GetDeleter()(GetPtr());
    }

    std::add_lvalue_reference_t<T> operator*() const {
        return *pair_.GetFirst();
    }

    T* operator->() const noexcept {
        return pair_.GetFirst();
    }

    T* Release() noexcept {
        T* temp_ptr = pair_.GetFirst();
        pair_.GetFirst() = nullptr;
        return temp_ptr;
    }

    void Reset(T* other_ptr = nullptr) noexcept {
        auto tmp = pair_.GetFirst();
        pair_.GetFirst() = other_ptr;
        GetDeleter()(tmp);
    }

    void Swap(UniquePtr& other_ptr) noexcept {
        T* tmp = pair_.GetFirst();
        pair_.GetFirst() = other_ptr.pair_.GetFirst();
        other_ptr.pair_.GetFirst() = tmp;

        std::swap(pair_.GetSecond(), other_ptr.pair_.GetSecond());

        /*auto tmp1 = pair_.GetSecond();
        pair_.GetSecond() = other_ptr.pair_.GetSecond();
        other_ptr.pair_.GetSecond() = tmp1;*/
    }

    T* Get() const {
        return pair_.GetFirst();
    }

    explicit operator bool() const noexcept {
        return !(pair_.GetFirst() == nullptr);
    }

    Deleter& GetDeleter() noexcept {
        return pair_.GetSecond();
    }

    T* &GetPtr() {
        return pair_.GetFirst();
    }

    const Deleter& GetDeleter() const {
        return pair_.GetSecond();
    }

    const T* GetPtr() const {
        return pair_.GetFirst();
    }

};

template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
private:
    CompressedPair<T*, Deleter> pair_;

    template<typename U, typename P>
    friend class UniquePtr;

public:
    explicit UniquePtr(T* new_ptr = nullptr) noexcept {
        pair_.GetFirst() = new_ptr;
    }

    UniquePtr(UniquePtr& other) = delete;


    UniquePtr(T* new_ptr, Deleter deleter) noexcept : pair_(new_ptr, std::forward<Deleter>(deleter)){}


    template<typename U, typename P>
    UniquePtr(UniquePtr<U, P>&& other) noexcept :
          pair_(other.pair_.GetFirst(), std::forward<P>(other.pair_.GetSecond())) {
        other.pair_.GetFirst() = nullptr;
    }

    UniquePtr& operator=(std::nullptr_t) noexcept {
        auto tmp = pair_.GetFirst();
        pair_.GetFirst() = nullptr;
        GetDeleter()(tmp);
        return *this;
    }

    UniquePtr& operator=(UniquePtr& other) = delete;

    UniquePtr& operator=(UniquePtr&& other_ptr) noexcept {
        if (other_ptr.pair_.GetFirst() == pair_.GetFirst()) {
            return *this;
        }
        auto tmp = pair_.GetFirst();
        pair_.GetFirst() = other_ptr.pair_.GetFirst();
        GetDeleter()(tmp);
        other_ptr.pair_.GetFirst() = nullptr;
        pair_.GetSecond() = std::forward<Deleter>(other_ptr.pair_.GetSecond());
        return *this;
    }

    ~UniquePtr() {
        GetDeleter()(GetPtr());
    }


    T* Release() noexcept {
        T* temp_ptr = pair_.GetFirst();
        pair_.GetFirst() = nullptr;
        return temp_ptr;
    }

    void Reset(T* other_ptr = nullptr) noexcept {
        auto tmp = pair_.GetFirst();
        pair_.GetFirst() = other_ptr;
        GetDeleter()(tmp);
    }

    void Swap(UniquePtr& other_ptr) noexcept {
        T* tmp = pair_.GetFirst();
        pair_.GetFirst() = other_ptr.pair_.GetFirst();
        other_ptr.pair_.GetFirst() = tmp;

        std::swap(pair_.GetSecond(), other_ptr.pair_.GetSecond());
        /*auto tmp1 = pair_.GetSecond();
        pair_.GetSecond() = other_ptr.pair_.GetSecond();
        other_ptr.pair_.GetSecond() = tmp1;*/
    }

    T* Get() const {
        return pair_.GetFirst();
    }

    explicit operator bool() const noexcept {
        return !(pair_.GetPtr() == nullptr);
    }

    Deleter& GetDeleter() noexcept {
        return pair_.GetSecond();
    }

    T* &GetPtr() {
        return pair_.GetFirst();
    }

    const Deleter& GetDeleter() const noexcept {
        return pair_.GetSecond();
    }

    T* GetPtr() const noexcept {
        return pair_.GetFirst();
    }

    T& operator[](size_t i) noexcept {
        return pair_.GetFirst()[i];
    }

    const T& operator[](size_t i) const noexcept {
        return pair_.GetFirst()[i];
    }
};
