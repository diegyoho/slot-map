#pragma once

#include <array>
#include <vector>

typedef unsigned long long ObjectId;

template <typename T, size_t N = 256>
class SlotMap
{
    struct Object
    {
        ObjectId id;
        T object;
    };

    const size_t _chunkSize;
    std::vector<std::array<Object, N>> _objectTable;
    std::vector<unsigned int> _freeIndices;

public:

    SlotMap() : _chunkSize{ N }, _objectTable{}, _freeIndices{}
    {
    }

    ~SlotMap() = default;

    ObjectId Insert(const T& object)
    {
        if (_freeIndices.empty())
        {
            _objectTable.push_back({});
            
            for (int i = _chunkSize - 1; i >= 0; --i) {
                _freeIndices.push_back((_objectTable.size() - 1) * _chunkSize + i);
            }
        }

        int free = _freeIndices.back();
        _freeIndices.pop_back();

        _objectTable[free / _chunkSize][free % _chunkSize].object = object;
        return _objectTable[free / _chunkSize][free % _chunkSize].id = free;
    }

    T* GetObject(const ObjectId& id) {
        Object obj = _objectTable[(id & 0xFFFFFFFF) / _chunkSize][((id & 0xFFFFFFFF) % _chunkSize)];
        return obj.id != id ? nullptr : &obj.object;
    }
};
