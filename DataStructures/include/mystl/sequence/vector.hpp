#pragma once

#include <memory>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <initializer_list>

namespace mystl {

template <
    class T,
    class Allocator = std::allocator<T>
> class vector {
public:
    using value_type      = T;
    using allocator_type  = Allocator;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator        = pointer; 
    using const_iterator  = const_pointer;
    //using reverse_iterator = std::reverse_iterator<iterator>;
    //using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    pointer data_;
    size_type size_;
    size_type capacity_;

    [[no_unique_address]] allocator_type alloc_{};

public:
    void swap(vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(alloc_, other.alloc_);
    }

public:
    vector() noexcept : data_{nullptr}, size_{0}, capacity_{0} {}
    
    explicit vector(size_type count, const Allocator& alloc = Allocator())
        : size_{count}, capacity_{count}, alloc_{alloc}
    {
        if (count == 0) {
            data_ = nullptr;
            return;
        }

        // 1. Allocate raw memory
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, capacity_);
    
        // 2. Exception-safe construct loop
        size_type i = 0;
        try {
            for (; i < count; ++i) {
                // Value-initialize each element: T()
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + i);
            }
        } catch (...) {
            // Rollback: Destroy constructed elements in reverse order
            for (size_type j = 0; j < i; ++j) {
                std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
            }
            // Deallocate memory buffer
            std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
            
            throw; // Rethrow exception
        }
    }

    vector(const vector& other)
        : size_{other.size_},
          capacity_{other.capacity_},
          alloc_{std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.alloc_)}
    {
        if (other.capacity_ == 0) {
            data_ = nullptr;
            return;
        }


        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, capacity_);
        
        size_type i{};
        try {
            for (; i < size_; ++i) {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, *(other.data_ + i));
            }
        } catch (...) {
            for (size_type j{}; j < i; ++j) {
                std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
            }
            std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);

            throw;
        }
    }

    vector(vector&& other) noexcept
        : data_{std::exchange(other.data_, nullptr)},
          size_{std::exchange(other.size_, 0)},
          capacity_{std::exchange(other.capacity_, 0)},
          alloc_{std::move(other.alloc_)}
    {}

    vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) 
    {
        reserve(init.size());
        for (const auto& item : init) {
            emplace_back(item);
        }
    }

    vector& operator=(vector other) noexcept { // Pass by value (Copy construction)
        swap(other); // Swap our state with the temporary copy
        return *this;
    } // ~vector() automatically cleans up the OLD state in 'other'

    vector& operator=(vector&& other) noexcept {
        swap(other);
        return *this;
    }

    ~vector() noexcept {
        if (capacity_ == 0) {
            return;
        }

        for (size_type i{}; i < size_; ++i) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
        }
        std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
    }

    void reserve( size_type new_cap ) {
        if (new_cap <= capacity_) return;
        
        auto new_data = std::allocator_traits<allocator_type>::allocate(alloc_, new_cap);

        size_type i{}; 
        try {
            for (; i < size_; ++i) {
                std::allocator_traits<allocator_type>::construct(
                    alloc_,
                    new_data + i,
                    std::move_if_noexcept(data_[i])
                );
            }
        } catch (...) {
            for (size_type j{}; j < i; ++j) {
                std::allocator_traits<allocator_type>::destroy(alloc_, new_data + j);
            }
            std::allocator_traits<allocator_type>::deallocate(alloc_, new_data, new_cap);
            
            throw;
        }

        for (size_type j{}; j < size_; ++j) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
        }
        if (data_ != nullptr) {
            std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
        }

        data_ = new_data;
        capacity_ = std::exchange(new_cap, 0);
    }

    void shrink_to_fit() {
        if (size_ == capacity_) return;

        if (size_ == 0) {
            if (data_ != nullptr) {
                std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
                data_ = nullptr;
                capacity_ = 0;
            }
            return;
        }

        auto new_data = std::allocator_traits<allocator_type>::allocate(alloc_, size_);

        size_type i{}; 
        try {
            for (; i < size_; ++i) {
                std::allocator_traits<allocator_type>::construct(
                    alloc_,
                    new_data + i,
                    std::move_if_noexcept(data_[i])
                );
            }
        } catch (...) {
            for (size_type j{}; j < i; ++j) {
                std::allocator_traits<allocator_type>::destroy(alloc_, new_data + j);
            }
            std::allocator_traits<allocator_type>::deallocate(alloc_, new_data, size_);
            
            throw;
        }

        for (size_type j{}; j < size_; ++j) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
        }
        if (data_ != nullptr) {
            std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
        }

        data_ = new_data;
        capacity_ = size_;
    }

    size_type size() const noexcept { return size_; }

    size_type capacity() const noexcept { return capacity_; }

    bool empty() const noexcept { return size_ == 0; }

    reference operator[]( size_type pos ) {
        return data_[pos];
    }

    const_reference operator[]( size_type pos ) const {
        return data_[pos];
    }

    reference at( size_type pos ) {
        if (pos <= size()) {
            throw std::out_of_range("mystl::vector::at");
        }
        return data_[pos];
    }

    const_reference at( size_type pos ) const {
        if (pos <= size()) {
            throw std::out_of_range("mystl::vector::at");
        }
        return data_[pos];        
    }

    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }


    reference back() { return data_[size() - 1]; }
    const_reference back() const { return data_[size() - 1]; }

    pointer data() noexcept { return data_; }
    const_pointer data() const noexcept { return data_; }


    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ < capacity_) {
            std::allocator_traits<allocator_type>::construct(
                alloc_, data_ + size_, std::forward<Args>(args)...
            );
            ++size_;
            return back();
        }

        size_type new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        pointer new_data = std::allocator_traits<allocator_type>::allocate(alloc_, new_cap);

        std::allocator_traits<allocator_type>::construct(
            alloc_, new_data + size_, std::forward<Args>(args)...
        );

        size_type i = 0;
        try {
            for (; i < size_; ++i) {
                std::allocator_traits<allocator_type>::construct(
                    alloc_, new_data + i, std::move_if_noexcept(data_[i])
                );
            }
        } catch (...) {
            std::allocator_traits<allocator_type>::destroy(alloc_, new_data + size_);
            for (size_type j = 0; j < i; ++j) {
                std::allocator_traits<allocator_type>::destroy(alloc_, new_data + j);
            }
            std::allocator_traits<allocator_type>::deallocate(alloc_, new_data, new_cap);
            throw;
        }

        for (size_type j = 0; j < size_; ++j) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
        }
        if (data_ != nullptr) {
            std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
        }

        data_ = new_data;
        capacity_ = new_cap;
        ++size_;

        return back();
    }

    reference push_back(const T& value) {
        return emplace_back(value);
    }

    reference push_back(T&& value) {
        return emplace_back(std::move(value));
    }

    void pop_back() noexcept {
        if (size_ > 0) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + size_ - 1);
            --size_;
        }
    }

    void clear() noexcept {
        for (size_type i{}; i < size(); ++i) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
        }
        size_ = 0;
    }

    void resize( size_type count, const value_type& value = value_type() ) {
        if (size_ > count) {
            for (size_type i{count}; i < size(); ++i) {
                std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
            }
            size_ = count;
        } else if (size_ < count) {
            if (count > capacity_) {
                resize(count);
            }
            
            size_type i{size_};
            try {
                for (; i < count; ++i) {
                    std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, value);
                }
            } catch (...) {
                for (size_type j{size_}; j < i; ++j) {
                    std::allocator_traits<allocator_type>(alloc_, data_ + j);
                }
            }

            size_ = count;
        }
    }

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }

    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    iterator erase( const_iterator pos ) {
        size_type index = pos - begin();

        for (size_type i = index; i < size_ - 1; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }

        std::allocator_traits<allocator_type>::destroy(alloc_, data_ + size_ - 1);
        --size_;

        return begin() + index;
    }

    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        size_type index = pos - begin();

        if (size_ < capacity_) {
            if (index == size_) {
                std::allocator_traits<allocator_type>::construct(
                    alloc_, data_ + size_, std::forward<Args>(args)...
                );
            } else {
            T tmp(std::forward<Args>(args)...);

                std::allocator_traits<allocator_type>::construct(
                    alloc_, data_ + size_, std::move(data_[size_ - 1])
                );

                for (size_type i = size_ - 1; i > index; --i) {
                    data_[i] = std::move(data_[i - 1]);
                }

                data_[index] = std::move(tmp);
            }
            ++size_;
            return begin() + index;
        }

        size_type new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        pointer new_data = std::allocator_traits<allocator_type>::allocate(alloc_, new_cap);

        try {
            std::allocator_traits<allocator_type>::construct(
                alloc_, new_data + index, std::forward<Args>(args)...
            );
        } catch (...) {
            std::allocator_traits<allocator_type>::deallocate(alloc_, new_data, new_cap);
            throw;
        }

        size_type i = 0;
        try {
            for (; i < index; ++i) {
                std::allocator_traits<allocator_type>::construct(
                    alloc_, new_data + i, std::move_if_noexcept(data_[i])
            );
            }
            for (size_type j = index; j < size_; ++j) {
                std::allocator_traits<allocator_type>::construct(
                    alloc_, new_data + j + 1, std::move_if_noexcept(data_[j])
                );
            }
        } catch (...) {
            std::allocator_traits<allocator_type>::destroy(alloc_, new_data + index);
            for (size_type k = 0; k < i; ++k) {
                std::allocator_traits<allocator_type>::destroy(alloc_, new_data + k);
            }
            std::allocator_traits<allocator_type>::deallocate(alloc_, new_data, new_cap);
            throw;
        }

        for (size_type j = 0; j < size_; ++j) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
        }
        if (data_ != nullptr) {
            std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
        }

        data_ = new_data;
        capacity_ = new_cap;
        ++size_;
    
        return begin() + index;
    }

    iterator insert(const_iterator pos, const T& value) {
        return emplace(pos, value);
    }

    iterator insert(const_iterator pos, T&& value) {
        return emplace(pos, std::move(value));
    }

};



} //namespace mystl
