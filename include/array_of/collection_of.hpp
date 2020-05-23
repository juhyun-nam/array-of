/**
 * @file array_of.hpp
 * @brief CollectionOf implementation c++
 */

#ifndef _COLLECTION_OF_H_
#define _COLLECTION_OF_H_

#include <algorithm>
#include <iterator>
#include <memory>

template <typename T, typename Alloc = std::allocator<T>>
class CollectionOf {
 public:
  using value_type = T;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = value_type*;
  using const_iterator = const value_type*;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using allocator_type = Alloc;

  explicit CollectionOf(size_type length,
                        const allocator_type& alloc = allocator_type{})
      : length_(length), d_(alloc) {
    ptr_ = d_.alloc.allocate(length_);
  }

  CollectionOf(const CollectionOf&) = delete;
  CollectionOf& operator=(const CollectionOf&) = delete;
  ~CollectionOf() {
    for (size_type i = 0; i < n; ++i) {
      ptr[i].~T();
    }
    d_(ptr_, length_);
  }

  // Element access

  reference at(size_type n) {
    RangeCheck(n);
    return ptr_[n];
  }
  const_reference at(size_type n) const {
    RangeCheck(n);
    return ptr_[n];
  }
  reference operator[](size_type n) {
    return ptr_[n];
  }
  const_reference operator[](size_type n) const {
    return ptr_[n];
  }
  reference front() {
    return ptr_[0];
  }
  const_reference front() const {
    return ptr_[0];
  }
  reference back() {
    return ptr_[length_ - 1];
  }
  const_reference back() const {
    return ptr_[length_ - 1];
  }
  pointer data() {
    return ptr_;
  }
  const_pointer data() const {
    return ptr_;
  }

  // Iterators

  iterator begin() {
    return iterator(ptr_);
  }
  const_iterator begin() const {
    return const_iterator(ptr_);
  }
  iterator end() {
    return iterator(ptr_ + length_);
  }
  const_iterator end() const {
    return const_iterator(ptr_ + length_);
  }
  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }
  const_reverse_iterator rbegin() const {
    return reverse_iterator(end());
  }
  reverse_iterator rend() {
    return reverse_iterator(begin());

  }
  const_reverse_iterator rend() const {
    return reverse_iterator(begin());
  }

  // Capacity

  size_type length() const {
    return length_;
  }

  //  Operations
  template <typename... Args>
  void construct(Args&&... args) {
    for (size_type i = 0; i < length_; ++i) {
      ::new (ptr_ + i) T(std::forward<Args>(args)...);
    }
  }

 private:
  void RangeCheck(size_type n) const {
    if (n >= length_) {
      throw std::out_of_range("CollectionOf::RangeCheck");
    }
  }
  struct Deleter {
    explicit Deleter(const allocator_type& alloc) : alloc(alloc) {}
    void operator()(T* ptr, size_type n) {
      alloc.deallocate(ptr, n);
    }
    Alloc alloc{};
  };

  size_type length_;
  T* ptr_{};
  Deleter d_;
};

template <typename T, typename Alloc>
static inline bool operator==(const CollectionOf<T, Alloc>& lhs,
                              const CollectionOf<T, Alloc>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename T, typename Alloc>
static inline bool operator!=(const CollectionOf<T, Alloc>& lhs,
                              const CollectionOf<T, Alloc>& rhs) {
  return !(lhs == rhs);
}

#endif