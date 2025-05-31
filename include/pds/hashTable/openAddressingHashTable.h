#pragma once

#include <vector>
#include <optional>
#include <bitset>
#include <utility>

#include "pds/core/common.h"
#include "openAddressingHashTableVisualiser.h"

namespace pds::hashTable
{
    template<typename Key, typename Value>
    class OpenAddressingHashTable
    {
        friend class OpenAddressingHashTableVisualiser<Key, Value>;

        public:
        OpenAddressingHashTable();
        void init(size_t capacity);
        void insert(const Key& key, const Value& value);
        std::optional<Value> query(const Key& key) const;
        bool contains(const Key& key) const;
        void erase(const Key& key);
        void clear();

        int32_t getLoadFactor() const;
        int32_t getSize() const;
        bool isEmpty() const;

    private:
        size_t hash(const Key& key) const;
        size_t probe(size_t index) const;

        size_t _capacity;
        size_t _size;
        std::vector<std::pair<Key, Value>>* _table;
        std::bitset<BIT_ARRAY_SIZE>* _bitArray;
        OpenAddressingHashTableVisualiser<Key, Value> _visualiser;
    };
}

#include "openAddressingHashTableImpl.h"