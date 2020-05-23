/**
 * @file array_of.hpp
 * @brief ArrayOf implementation c++
 */

#ifndef _ARRAY_OF_H_
#define _ARRAY_OF_H_

#include <algorithm>  // for equal, swap_ranges
#include <cstddef>    // for ptrdiff_t, size_t
#include <iterator>   // for reverse_iterator
#include <memory>     // for allocator, uninitialized_fill_n
#include <stdexcept>  // for out_of_range

template <typename T, typename Alloc = std::allocator<T>>
class ArrayOf {
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

  explicit ArrayOf(size_type length, const value_type& v = value_type{},
                   const allocator_type& alloc = allocator_type{})
      : length_(length), d_(alloc) {
    ptr_ = d_.alloc.allocate(length_);
    std::uninitialized_fill_n(ptr_, length_, v);
  }
  ArrayOf(size_type length, const allocator_type& alloc)
      : ArrayOf(length_, value_type{}, alloc) {}
  ArrayOf(size_type length, const value_type& v)
      : ArrayOf(length, v, std::allocator<T>()) {}

  ArrayOf(const ArrayOf&) = delete;
  ArrayOf& operator=(const ArrayOf&) = delete;
  ArrayOf(ArrayOf&& other) {
    std::swap(length_, other.length_);
    std::swap(ptr_, other.ptr_);
    d_ = Deleter(other.d_.alloc);
  }
  ArrayOf& operator=(ArrayOf&& other) {
    std::swap(length_, other.length_);
    std::swap(ptr_, other.ptr_);
    d_ = Deleter(other.d_.alloc);
    return *this;
  }
  ~ArrayOf() { d_(ptr_, length_); }

  // Element access

  reference at(size_type n) {
    RangeCheck(n);
    return ptr_[n];
  }
  const_reference at(size_type n) const {
    RangeCheck(n);
    return ptr_[n];
  }
  reference operator[](size_type n) { return ptr_[n]; }
  const_reference operator[](size_type n) const { return ptr_[n]; }
  reference front() { return ptr_[0]; }
  const_reference front() const { return ptr_[0]; }
  reference back() { return ptr_[length_ - 1]; }
  const_reference back() const { return ptr_[length_ - 1]; }
  pointer data() { return ptr_; }
  const_pointer data() const { return ptr_; }

  // Iterators

  iterator begin() { return iterator(ptr_); }
  const_iterator begin() const { return const_iterator(ptr_); }
  iterator end() { return iterator(ptr_ + length_); }
  const_iterator end() const { return const_iterator(ptr_ + length_); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return reverse_iterator(begin()); }

  // Capacity

  size_type length() const { return length_; }

  //  Operations
  void fill(const value_type& v) {
    std::uninitialized_fill_n(ptr_, length_, v);
  }
  void swap(ArrayOf&& other) {
    std::swap_ranges(begin(), end(), other.begin());
  }

 private:
  void RangeCheck(size_type n) const {
    if (n >= length_) {
      throw std::out_of_range("ArrayOf::RangeCheck");
    }
  }
  struct Deleter {
    explicit Deleter(const allocator_type& alloc) : alloc(alloc) {}
    void operator()(T* ptr, size_type n) {
      if (ptr) alloc.deallocate(ptr, n);
    }
    Alloc alloc{};
  };

  size_type length_{};
  T* ptr_{};
  Deleter d_;
};

template <typename T, typename Alloc>
static inline bool operator==(const ArrayOf<T, Alloc>& lhs,
                              const ArrayOf<T, Alloc>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename T, typename Alloc>
static inline bool operator!=(const ArrayOf<T, Alloc>& lhs,
                              const ArrayOf<T, Alloc>& rhs) {
  return !(lhs == rhs);
}

#endif