# stride_vector
A byte (unsigned char) based vector that retains a stride when instantiated from other typed vectors
Also includes a metho for interleaving stride_vectors

# Usage

`stride_vector(size_t size)` creates a new `stride_vector` with `size` memory **reserved**

`operator[](size_t idx)` returns a `vector<unsigned char>` starting from `idx * stride` and `stride` chars long

`stride_vector::create(vector<T>)` creates a new `stride_vector` from a given typed vector. This `stride_vector`'s `stride` property is the `sizeof(T) / sizeof(unsigned char)`

`stride_vector = vector<T>` is equiavlent to `stride_vector::create(vector<T>)`

`stride_vector::interleave({stride_vector, stride_vector, ...})` creates a new `stride_vector` by interleaving the given `stride_vector`s together. This interleaving takes each `stride_vector`'s `stride` into account

`get<T>()` returns a pointer to the memory casted to `T*`

# Example

```
vector<uint32_t> vec1;
vec1.push_back(100);
vec1.push_back(300);

vector<uint16_t> vec2;
vec2.push_back(5999);
vec2.push_back(4444);

stride_vector& vec3 = stride_vector::interleave({stride_vector::create(asdf), stride_vector::create(vec2)});
```

`vec3` now contains memory equivalent to `{(uint32_t) 100, (uint16_t) 5999, (uint32_t) 300, (uint16_t) 4444}`

# License

stride_vector can be used under the MIT License or the Unlicense