#include "stride_vector.h"

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