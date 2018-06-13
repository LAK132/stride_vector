/*  
    MIT License

    Copyright (c) 2018 Lucas Kleiss (LAK132)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <vector>
using std::vector;

#ifndef STRIDE_VECTOR_H
#define STRIDE_VECTOR_H

struct stride_vector
{
    size_t stride = 1;
    vector<unsigned char> data;
    stride_vector();
    stride_vector(size_t size);
    stride_vector(size_t str, size_t size);
    stride_vector(const stride_vector& other);
    stride_vector(stride_vector&& other);
    void init(size_t size);
    void init(size_t str, size_t size);
    stride_vector& operator=(const stride_vector& other);
    stride_vector& operator=(stride_vector&& other);
    vector<unsigned char> operator[](size_t idx) const;
    template <typename T> stride_vector& operator=(const vector<T>& other)
    {
        stride = sizeof(T) / sizeof(unsigned char);
        data.resize(sizeof(T) * other.size());
        memcpy(&(data[0]), &(other[0]), data.size());
        return *this;
    }
    template <typename T> stride_vector& operator=(vector<T>&& other)
    {
        stride = sizeof(T) / sizeof(unsigned char);
        data.resize(sizeof(T) * other.size());
        memcpy(&(data[0]), &(other[0]), data.size());
        return *this;
    }
    template <typename T> inline T* get()
    {
        return (T*)&(data[0]);
    }
    template <typename T> static stride_vector create(const vector<T> other)
    {
        stride_vector rtn;
        return rtn = other;
    }
    template <typename T> static stride_vector create(vector<T>&& other)
    {
        stride_vector rtn;
        return rtn = other;
    }
    static stride_vector interleave(const vector<stride_vector*>& vecs);
    static stride_vector interleave(vector<stride_vector*>&& vecs);
};

#endif // STRIDE_VECTOR_H

#ifdef STRIDE_VECTOR_IMPLEMENTATION

stride_vector::stride_vector(){}

stride_vector::stride_vector(size_t size)
{
    init(size);
}

stride_vector::stride_vector(size_t str, size_t size)
{
    init(str, size);
}

stride_vector::stride_vector(const stride_vector& other)
{
    stride = other.stride;
    data = other.data;
}

stride_vector::stride_vector(stride_vector&& other)
{
    stride = other.stride;
    data = other.data;
}

void stride_vector::init(size_t size)
{
    data.reserve(size);
}

void stride_vector::init(size_t str, size_t size)
{
    stride = str;
    data.resize(size);
}

stride_vector& stride_vector::operator=(const stride_vector& other)
{
    stride = other.stride;
    data = other.data;
    return *this;
}

stride_vector& stride_vector::operator=(stride_vector&& other)
{
    stride = other.stride;
    data = other.data;
    return *this;
}

vector<unsigned char> stride_vector::operator[](size_t idx) const
{
    if(stride * idx > data.size()) return vector<unsigned char>(0);
    vector<unsigned char> rtn(stride);
    memcpy(&(rtn[0]), &(data[stride * idx]), stride);
    return rtn;
}

stride_vector stride_vector::interleave(const vector<stride_vector*>& vecs)
{
    size_t size = 0;
    size_t maxlen = 0;
    for(auto it = vecs.begin(); it != vecs.end(); it++)
    {
        size += (*it)->data.size();
        if((*it)->data.size() > maxlen) maxlen = (*it)->data.size() / (*it)->stride;
    }
    stride_vector rtn(size);
    for(size_t i = 0; i < maxlen; i++)
    {
        for(auto it = vecs.begin(); it != vecs.end(); it++)
        {
            vector<unsigned char>& vec = (**it)[i];
            for(auto it2 = vec.begin(); it2 != vec.end(); it2++)
            {
                rtn.data.push_back(*it2);
            }
        }
    }
    return rtn;
}

stride_vector stride_vector::interleave(vector<stride_vector*>&& vecs)
{
    stride_vector& rtn = interleave(vecs);
    return rtn;
}

#endif // STRIDE_VECTOR_IMPLEMENTATION