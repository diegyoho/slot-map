#pragma once

#include <array>
#include <vector>

/**
 * @namespace SM
 * @brief Namespace containing classes and types related to SlotMap.
 */
namespace SM
{
    /**
     * @brief Type representing an object identifier in the SlotMap.
     */
    typedef unsigned long long ObjectId;

    /**
     * @brief A data structure for managing objects using a SlotMap mechanism.
     *
     * @tparam T The type of objects to be stored in the SlotMap.
     * @tparam N The chunk size for allocating objects (default is 256).
     */
    template <typename T, size_t N = 256>
    class SlotMap
    {
        /**
         * @brief Represents an object in the SlotMap.
         *
         * This struct defines an object within the SlotMap data structure. It consists of
         * a unique identifier and a value associated with the object.
         */
        struct Object
        {
            ObjectId id{}; ///< The unique identifier of the object.
            T value{};     ///< The value stored in the object.
        };

        const size_t _chunkSize;
        std::vector<std::array<Object, N>> _objectTable;
        std::vector<unsigned int> _freeIndices;

        /**
         * @brief Add a new chunk to the SlotMap for storing objects.
         */
        void AddChunck()
        {
            _objectTable.push_back({});

            for (int i = _chunkSize - 1; i >= 0; --i) {
                _freeIndices.push_back((_objectTable.size() - 1) * _chunkSize + i);
            }
        }

    public:
        /**
         * @brief Constructs a SlotMap with an optional initial chunk.
         *
         * @param initializeWithChunk If true, initializes the SlotMap with an initial chunk.
         */
        SlotMap(const bool& initializeWithChunk = false) : _chunkSize{ N }, _objectTable{}, _freeIndices{}
        {
            if (initializeWithChunk)
            {
                AddChunck();
            }
        }

        /**
         * @brief Destructor for the SlotMap.
         */
        ~SlotMap() = default;

        /**
         * @brief Inserts an object into the SlotMap.
         *
         * @param value The value to be inserted.
         * @return The unique identifier (ObjectId) assigned to the inserted object.
         */
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

        /**
         * @brief Retrieves a constant pointer to the object associated with the given ObjectId.
         *
         * @param id The unique identifier of the object to retrieve.
         * @return A constant pointer to the object, or nullptr if the object does not exist.
         */
        const Object* GetObject(const ObjectId& id) const {
            const Object& obj = _objectTable[(id & 0xFFFFFFFF) / _chunkSize][((id & 0xFFFFFFFF) % _chunkSize)];
            return obj.id != id ? nullptr : &obj;
        }

        /**
         * @brief Destroys the object associated with the given ObjectId.
         *
         * @param id The unique identifier of the object to destroy.
         */
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
