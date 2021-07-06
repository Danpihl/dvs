#ifndef DVS_IMAGE_C3_H_
#define DVS_IMAGE_C3_H_

#include <assert.h>
#include <stdint.h>
#include <cstring>

#include <iostream>

namespace dvs
{

template <typename T> class ImageC3
{
private:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;
    bool is_allocated_;
    size_t num_element_per_channel_;

public:
    ImageC3();
    ImageC3(const size_t num_rows, const size_t num_cols);
    ~ImageC3();

    T& operator()(const size_t r, const size_t c, const size_t ch);
    const T& operator()(const size_t r, const size_t c, const size_t ch) const;

    bool isAllocated() const;
    size_t rows() const;
    size_t cols() const;
    size_t numBytes() const;
    void fillBufferWithData(uint8_t* const buffer) const;
    void setInternalData(T* const input_ptr, const size_t num_rows, const size_t num_cols);

    void scale(T factor);
    void addTo(T term);
    void fill(T fill_val);
    size_t numElements() const;
    size_t size() const;
    T* getDataPointer() const;
};

template <typename T> ImageC3<T>::~ImageC3()
{
    if (is_allocated_)
    {
        delete[] data_;
    }
}

template <typename T> ImageC3<T>::ImageC3() : num_rows_(0), num_cols_(0), is_allocated_(false), num_element_per_channel_(0) {}

template <typename T> ImageC3<T>::ImageC3(const size_t num_rows, const size_t num_cols)
{
    data_ = new T[num_rows * num_cols];
    num_rows_ = num_rows;
    num_cols_ = num_cols;
    num_element_per_channel_ = num_rows_ * num_cols_;
    is_allocated_ = true;
}

template <typename T>
void ImageC3<T>::setInternalData(T* const input_ptr, const size_t num_rows, const size_t num_cols)
{
    if (input_ptr == nullptr)
    {
        is_allocated_ = false;
    }
    else
    {
        is_allocated_ = true;
    }

    data_ = input_ptr;
    num_rows_ = num_rows;
    num_cols_ = num_cols;
    num_element_per_channel_ = num_rows_ * num_cols_;
}

template <typename T>
void ImageC3<T>::fillBufferWithData(uint8_t* const buffer) const
{
    const uint8_t* const internal_ptr = reinterpret_cast<uint8_t*>(data_);
    const size_t num_bytes = num_rows_ * num_cols_ * 3 * sizeof(T);
    std::memcpy(buffer, internal_ptr, num_bytes);
}

template <typename T> 
size_t ImageC3<T>::numBytes() const
{
    return 3 * num_rows_ * num_cols_ * sizeof(T);
}

template <typename T> size_t ImageC3<T>::rows() const
{
    return num_rows_;
}

template <typename T> size_t ImageC3<T>::cols() const
{
    return num_cols_;
}

template <typename T> size_t ImageC3<T>::size() const
{
    return num_rows_ * num_cols_ * 3;
}

template <typename T> size_t ImageC3<T>::numElements() const
{
    return num_rows_ * num_cols_ * 3;
}

template <typename T> T* ImageC3<T>::getDataPointer() const
{
    return data_;
}

template <typename T> void ImageC3<T>::scale(T factor)
{
    for (size_t idx = 0; idx < num_rows_ * num_cols_ * 3; idx++)
    {
        data_[idx] = data_[idx] * factor;
    }
}

template <typename T> void ImageC3<T>::fill(T fill_val)
{
    for (size_t idx = 0; idx < num_rows_ * num_cols_ * 3; idx++)
    {
        data_[idx] = fill_val;
    }
}

template <typename T> void ImageC3<T>::addTo(T term)
{
    for (size_t idx = 0; idx < num_rows_ * num_cols_ * 3; idx++)
    {
        data_[idx] = data_[idx] + term;
    }
}

template <typename T> T& ImageC3<T>::operator()(const size_t r, const size_t c, const size_t ch)
{
    assert(r < num_rows_ && "Row index is larger than num_rows_-1!");
    assert(c < num_cols_ && "Column index is larger than num_cols_-1!");

    return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
}

template <typename T> const T& ImageC3<T>::operator()(const size_t r, const size_t c, const size_t ch) const
{
    assert(r < num_rows_ && "Row index is larger than num_rows_-1!");
    assert(c < num_cols_ && "Column index is larger than num_cols_-1!");

    return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
}

template <typename T> bool ImageC3<T>::isAllocated() const
{
    return is_allocated_;
}


}

#endif
