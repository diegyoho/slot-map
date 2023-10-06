#pragma once

#include <array>
#include <vector>

namespace SM
{
    typedef unsigned long long ObjectId;

    template <typename T, size_t N = 256>
    class SlotMap
    {
        struct Object
        {
            ObjectId id{};
            T value{};
        };

        const size_t _chunkSize;
        std::vector<std::array<Object, N>> _objectTable;
        std::vector<unsigned int> _freeIndices;

        void AddChunck()
        {
            _objectTable.push_back({});

            for (int i = _chunkSize - 1; i >= 0; --i) {
                _freeIndices.push_back((_objectTable.size() - 1) * _chunkSize + i);
            }
        }

    public:
        SlotMap(const bool& initializeWithChunk = false) : _chunkSize{ N }, _objectTable{}, _freeIndices{}
        {
            if (initializeWithChunk)
            {
                AddChunck();
            }
        }

        ~SlotMap() = default;

        ObjectId Insert(const T& value)
        {
            if (_freeIndices.empty())
            {
                AddChunck();
            }

            int free = _freeIndices.back();
            _freeIndices.pop_back();

            _objectTable[free / _chunkSize][free % _chunkSize].value = value;
            return _objectTable[free / _chunkSize][free % _chunkSize].id = free;
        }

        const Object* GetObject(const ObjectId& id) const {
            const Object& obj = _objectTable[(id & 0xFFFFFFFF) / _chunkSize][((id & 0xFFFFFFFF) % _chunkSize)];
            return obj.id != id ? nullptr : &obj;
        }

        void DestroyObject(const ObjectId& id) {
            if (const Object* objPtr = GetObject(id)) {
                Object& obj = const_cast<Object&>(*objPtr);
                obj.id = (obj.id & 0xFFFFFFFF) | (((obj.id >> 32) + 1) << 32);
                obj.value = T{};

                _freeIndices.push_back(id & 0xFFFFFFFF);
            }
        }
    };
}
