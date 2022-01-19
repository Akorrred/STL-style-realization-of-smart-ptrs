#pragma once

#include "sw_fwd.h"

#include <cstddef>


class ControlBlockBase {
public:
    size_t cnt_{};

    virtual ~ControlBlockBase() = default;
};

template <typename U>
class ControlBlockPointer : public ControlBlockBase {
public:
    ControlBlockPointer(U* ptr) noexcept : ptr_(ptr) {
    }

    U* ptr_;

    ~ControlBlockPointer() {
        delete ptr_;
    }
};

template <typename U>
class ControlBlockHolder : public ControlBlockBase {
public:
    template <typename... Args>
    ControlBlockHolder(Args&&... args) {
        new (&storage_) U(std::forward<Args>(args)...);
        ++cnt_;
    }

    U* GetRawPtr() {
        return reinterpret_cast<U*>(&storage_);
    }

    ~ControlBlockHolder() {
        reinterpret_cast<U*>(&storage_)->~U();
    }

    std::aligned_storage<sizeof(U), alignof(U)> storage_[sizeof(U)];
};

template <typename T>
class SharedPtr {
private:
    T* ptr_{};
    ControlBlockBase* block_;

    template <typename Q, typename... Args>
    friend SharedPtr<Q> MakeShared(Args&&... args);

    template <typename U>
    friend class SharedPtr;

public:
    void Removeblock() {
        if (ptr_ != nullptr) {
            if (block_->cnt_ == 1) {
                delete block_;
            } else {
                --block_->cnt_;
            }
        }
    }

    void Addblock() {
        if (ptr_ != nullptr) {
            if (block_ != nullptr) {
                ++block_->cnt_;
            }
        }
    }


    SharedPtr() noexcept {
        ptr_ = nullptr;
        block_ = nullptr;
    };

    SharedPtr(std::nullptr_t) noexcept : ptr_(nullptr), block_(nullptr) {
    }

    template <typename U>
    explicit SharedPtr(U* ptr) noexcept : ptr_(ptr), block_(new ControlBlockPointer<U>(ptr)) {
        block_->cnt_ = 1;
    }

    SharedPtr(const SharedPtr& other) noexcept : ptr_(other.ptr_), block_(other.block_) {
        Addblock();
    }

    template <typename U>
    SharedPtr(const SharedPtr<U>& other) noexcept : ptr_(other.ptr_), block_(other.block_) {
        Addblock();
    }

    SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), block_(other.block_) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    template <typename U>
    SharedPtr(SharedPtr<U>&& other) noexcept : ptr_(other.ptr_), block_(other.block_) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (other.ptr_ != ptr_) {
            Removeblock();
            ptr_ = other.ptr_;
            block_ = other.block_;
            Addblock();
        }
        return *this;
    }

    template <typename U>
    SharedPtr& operator=(const SharedPtr<U>& other) noexcept {
        if (other.ptr_ != ptr_) {
            Removeblock();
            ptr_ = other.ptr_;
            block_ = other.block_;
            Addblock();
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (other.ptr_ != ptr_) {
            Removeblock();
            ptr_ = other.ptr_;
            block_ = other.block_;
            other.ptr_ = nullptr;
            other.block_ = nullptr;
        }
        return *this;
    }

    template <typename U>
    SharedPtr& operator=(SharedPtr<U>&& other) noexcept {
        if (other.ptr_ != ptr_) {
            Removeblock();
            ptr_ = other.ptr_;
            block_ = other.block_;
            other.ptr_ = nullptr;
            other.block_ = nullptr;
        }
        return *this;
    }

    ~SharedPtr() noexcept {
        Removeblock();
    }

    void Reset() noexcept {
        Removeblock();
        ptr_ = nullptr;
        block_ = nullptr;
    }

    template <typename U>
    void Reset(U* ptr) noexcept {
        Removeblock();
        ptr_ = ptr;
        block_ = new ControlBlockPointer<U>(ptr);
        block_->cnt_ = 1;
    }

    void Swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(block_, other.block_);
    }

    T* Get() const noexcept{
        return ptr_;
    }
    T& operator*() const noexcept{
        return *ptr_;
    }
    T* operator->() const noexcept {
        return ptr_;
    }

    size_t UseCount() const noexcept {
        if (block_ == nullptr) {
            return 0;
        }
        return block_->cnt_;
    }
    explicit operator bool() const {
        return ptr_ != nullptr;
    }
};

template <typename T, typename U>
inline bool operator==(const SharedPtr<T>& left, const SharedPtr<U>& right) {
    return (*left == *right);
}

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    SharedPtr<T> sp;

    auto block = new ControlBlockHolder<T>(std::forward<Args>(args)...);
    sp.block_ = block;
    sp.ptr_ = block->GetRawPtr();

    return sp;
}

