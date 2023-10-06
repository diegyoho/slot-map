#pragma once

#include <array>
#include <vector>

template <typename T, size_t N = 256>
class SlotMap
{
    typedef unsigned long long ObjectId;

public:

    SlotMap() : _chunkSize{ N }, _objectTable{}, _freeIndices{}
    {
    }

    ~SlotMap() = default;

private:
    struct Object
    {
        ObjectId id;
        T object;
    };

    const size_t _chunkSize;
    std::vector<std::array<Object, N>> _objectTable;
    std::vector<unsigned int> _freeIndices;
};
