#ifndef DVS_IMAGE_C1_H_
#define DVS_IMAGE_C1_H_

#include <assert.h>
#include <stdint.h>
#include <cstring>

#include <iostream>

namespace dvs
{

template <typename T> class ImageC1
{
private:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;
    bool is_allocated_;

public:
    ImageC1();
    ImageC1(const size_t num_rows, const size_t num_cols);
    ~ImageC1();

    T& operator()(const size_t r, const size_t c);
    const T& operator()(const size_t r, const size_t c) const;

    bool isAllocated() const;
    T min() const;
    T max() const;
    size_t rows() const;
    size_t cols() const;
    size_t numBytes() const;
    void fillBufferWithData(uint8_t* const buffer) const;
    void setInternalData(T* const input_ptr, const size_t num_rows, const size_t num_cols);

    void reallocate(const size_t num_rows_, const size_t num_cols_);

    void scale(T factor);
    void addTo(T term);
    void fill(T fill_val);
    void mapBetween(T image_min, T image_max, T min_value, T max_value);
    size_t numElements() const;
    size_t size() const;
    T* getDataPointer() const;
};

template <typename T> ImageC1<T>::~ImageC1()
{
    if (is_allocated_)
    {
        delete[] data_;
    }
}

template <typename T> ImageC1<T>::ImageC1() : num_rows_(0), num_cols_(0), is_allocated_(false) {}

template <typename T> ImageC1<T>::ImageC1(const size_t num_rows, const size_t num_cols)
{
    data_ = new T[num_rows * num_cols];
    num_rows_ = num_rows;
    num_cols_ = num_cols;
    is_allocated_ = true;
}

template <typename T>
void ImageC1<T>::setInternalData(T* const input_ptr, const size_t num_rows, const size_t num_cols)
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
}

template <typename T>
void ImageC1<T>::fillBufferWithData(uint8_t* const buffer) const
{
    const uint8_t* const internal_ptr = reinterpret_cast<uint8_t*>(data_);
    const size_t num_bytes = num_rows_ * num_cols_ * sizeof(T);
    std::memcpy(buffer, internal_ptr, num_bytes);
}

template <typename T> 
size_t ImageC1<T>::numBytes() const
{
    return num_rows_ * num_cols_ * sizeof(T);
}

template <typename T> void ImageC1<T>::reallocate(const size_t num_rows, const size_t num_cols)
{
    if (is_allocated_)
    {
        delete[] data_;
        is_allocated_ = false;
        num_rows_ = 0;
        num_cols_ = 0;
    }

    data_ = new T[num_rows * num_cols];
    num_rows_ = num_rows;
    num_cols_ = num_cols;
    is_allocated_ = true;
}

template <typename T>
void ImageC1<T>::mapBetween(T image_min, T image_max, T min_value, T max_value)
{
    const T factor = (max_value - min_value) / (image_max - image_min);
    for (int idx = 0; idx < num_rows_ * num_cols_; idx++)
    {
        data_[idx] = data_[idx] * factor + min_value;
    }
}

template <typename T> size_t ImageC1<T>::rows() const
{
    return num_rows_;
}

template <typename T> size_t ImageC1<T>::cols() const
{
    return num_cols_;
}

template <typename T> size_t ImageC1<T>::size() const
{
    return num_rows_ * num_cols_;
}

template <typename T> size_t ImageC1<T>::numElements() const
{
    return num_rows_ * num_cols_;
}

template <typename T> T* ImageC1<T>::getDataPointer() const
{
    return data_;
}

template <typename T> void ImageC1<T>::scale(T factor)
{
    for (size_t idx = 0; idx < num_rows_ * num_cols_; idx++)
    {
        data_[idx] = data_[idx] * factor;
    }
}

template <typename T> void ImageC1<T>::fill(T fill_val)
{
    for (size_t idx = 0; idx < num_rows_ * num_cols_; idx++)
    {
        data_[idx] = fill_val;
    }
}

template <typename T> void ImageC1<T>::addTo(T term)
{
    for (size_t idx = 0; idx < num_rows_ * num_cols_; idx++)
    {
        data_[idx] = data_[idx] + term;
    }
}

template <typename T> T& ImageC1<T>::operator()(const size_t r, const size_t c)
{
    assert(r < num_rows_ && "Row index is larger than num_rows_-1!");
    assert(c < num_cols_ && "Column index is larger than num_cols_-1!");

    return data_[r * num_cols_ + c];
}

template <typename T> const T& ImageC1<T>::operator()(const size_t r, const size_t c) const
{
    assert(r < num_rows_ && "Row index is larger than num_rows_-1!");
    assert(c < num_cols_ && "Column index is larger than num_cols_-1!");

    return data_[r * num_cols_ + c];
}

template <typename T> bool ImageC1<T>::isAllocated() const
{
    return is_allocated_;
}

template <typename T> T ImageC1<T>::min() const
{
    assert(is_allocated_ && "Image not allocated!");
    T min_value = data_[0];

    // TODO: Crashes without "-1" in less than condition
    for (size_t idx = 1; idx < num_rows_ * num_cols_ - 1; idx++)
    {
        min_value = data_[idx] < min_value ? data_[idx] : min_value;
    }

    return min_value;
}

template <typename T> T ImageC1<T>::max() const
{
    // TODO: Crashes without "-1" in less than condition
    assert(is_allocated_ && "Image not allocated!");
    T max_value = data_[0];
    for (size_t idx = 1; idx < num_rows_ * num_cols_ - 1; idx++)
    {
        max_value = data_[idx] > max_value ? data_[idx] : max_value;
    }

    return max_value;
}

}

#endif
