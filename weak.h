#pragma once

#include "sw_fwd.h"
#include "shared.h"

template <typename T>
class WeakPtr {
private:
    T* ptr_{};
    ControlBlockBase* block_;

    template <typename U>
    friend class WeakPtr;

    template <typename P>
    friend class SharedPtr;

public:
    void Removeweak() {
        if (ptr_ != nullptr) {
            --block_->weak_cnt_;
            if (block_->weak_cnt_ == 0) {
                if (block_->shared_cnt_ == 0) {
                    delete block_;
                    ptr_ = nullptr;
                }
            } 
        }
    }

    void Addweak() {
        if (ptr_ != nullptr) {
            if (block_ != nullptr) {
                ++block_->weak_cnt_;
            }
        }
    }


    WeakPtr() {
        ptr_ = nullptr;
        block_ = nullptr;
    }

    WeakPtr(const WeakPtr& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        Addweak();
    }

    template <typename U>
    WeakPtr(const WeakPtr<U>& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        Addweak();
    }

    WeakPtr(WeakPtr&& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    template <typename U>
    WeakPtr(WeakPtr<U>&& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    WeakPtr(const SharedPtr<T>& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        Addweak();
    }

    template <typename U>
    WeakPtr(const SharedPtr<U>& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        Addweak();
    }


    WeakPtr& operator=(const WeakPtr& other) {
        Removeweak();
        ptr_ = other.ptr_;
        block_ = other.block_;
        Addweak();
        return *this;
    }

    template <typename U>
    WeakPtr& operator=(const WeakPtr<U>& other) {
        Removeweak();
        ptr_ = other.ptr_;
        block_ = other.block_;
        Addweak();
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& other) {
        Removeweak();
        ptr_ = other.ptr_;
        block_ = other.block_;
        other.ptr_ = nullptr;
        other.block_ = nullptr;
        return *this;
    }

    template <typename U>
    WeakPtr& operator=(WeakPtr<U>&& other) {
        Removeweak();
        ptr_ = other.ptr_;
        block_ = other.block_;
        other.ptr_ = nullptr;
        other.block_ = nullptr;
        return *this;
    }


    ~WeakPtr() {
        Removeweak();
    }

    void Reset() {
        Removeweak();
        ptr_ = nullptr;
        block_ = nullptr;
    }
    void Swap(WeakPtr& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(block_, other.block_);
    }


    size_t UseCount() const {
        if (block_ == nullptr) {
            return 0;
        }
        return block_->shared_cnt_;
    }

    bool Expired() const {
        return UseCount() == 0;
    }
    SharedPtr<T> Lock() const {
        if (!Expired()) {
            return SharedPtr<T>(*this);
        } else {
            return SharedPtr<T>();
        }
    }
};
