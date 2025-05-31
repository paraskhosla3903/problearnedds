#pragma once

namespace pds::hashTable
{
    /**
     * @brief Construct a new Open Addressing Hash Table< Key, Value>:: Open Addressing Hash Table object
     * 
     * @tparam Key 
     * @tparam Value 
     */
    template<typename Key, typename Value>
    OpenAddressingHashTable<Key, Value>::OpenAddressingHashTable()
        : _capacity(0), _size(0)
    {
        _table = new std::vector<std::pair<Key, Value>>(BIT_ARRAY_SIZE);
        _bitArray = new std::bitset<BIT_ARRAY_SIZE>();
    }

    /**
     * @brief Initialise the hash table with a given capacity
     * 
     * @tparam Key 
     * @tparam Value 
     * @param capacity 
     */
    template<typename Key, typename Value>
    void OpenAddressingHashTable<Key, Value>::init(size_t capacity)
    {
        _capacity = capacity;
        _size = 0;
        _visualiser.logAction("Initialized table");
        _visualiser.log(*this, std::nullopt, VisualContext::INIT);
    }

    /**
     * @brief Insert a key-value pair into the hash table
     * 
     * @tparam Key 
     * @tparam Value 
     * @param key 
     * @param value 
     */
    template<typename Key, typename Value>
    void OpenAddressingHashTable<Key, Value>::insert(const Key& key, const Value& value)
    {
        size_t index = hash(key);
        while (_bitArray->test(index)) {
            index = probe(index);
        }
        (*_table)[index] = {key, value};
        _bitArray->set(index);
        _size++;
        _visualiser.logAction("Inserted key: " + key);
        _visualiser.log(*this, index, VisualContext::INSERT);
    }

    /**
     * @brief Queries the hash table for a value associated with a key
     * 
     * @tparam Key 
     * @tparam Value 
     * @param key 
     * @return std::optional<Value> 
     */
    template<typename Key, typename Value>
    std::optional<Value> OpenAddressingHashTable<Key, Value>::query(const Key& key) const
    {
        size_t index = hash(key);
        while (_bitArray->test(index)) {
            if ((*_table)[index].first == key) {
                _visualiser.logAction("Query hit for key: " + key);
                _visualiser.log(*this, index, VisualContext::QUERY);
                return (*_table)[index].second;
            }
            index = probe(index);
        }
        _visualiser.logAction("Query miss for key: " + key);
        return std::nullopt;
    }

    /**
     * @brief Checks if the hash table contains a key
     * 
     * @tparam Key 
     * @tparam Value 
     * @param key 
     * @return true 
     * @return false 
     */
    template<typename Key, typename Value>
    bool OpenAddressingHashTable<Key, Value>::contains(const Key& key) const
    {
        return query(key).has_value();
    }

    /**
     * @brief Erases a key-value pair from the hash table
     * 
     * @tparam Key 
     * @tparam Value 
     * @param key 
     */
    template<typename Key, typename Value>
    void OpenAddressingHashTable<Key, Value>::erase(const Key& key)
    {
        size_t index = hash(key);
        while (_bitArray->test(index))
        {
            if ((*_table)[index].first == key)
            {
                _bitArray->reset(index);
                (*_table)[index] = {};
                _size--;
                _visualiser.logAction("Erased key: " + key);
                _visualiser.log(*this, index, VisualContext::ERASE);
                return;
            }
            index = probe(index);
        }
        _visualiser.logAction("Erase failed: key not found - " + key);
    }

    /**
     * @brief Clears the hash table
     * 
     * @tparam Key 
     * @tparam Value 
     */
    template<typename Key, typename Value>
    void OpenAddressingHashTable<Key, Value>::clear()
    {
        _bitArray->reset();
        _table->clear();
        _table->resize(BIT_ARRAY_SIZE);
        _size = 0;
        _visualiser.logAction("Cleared table");
    }

    /**
     * @brief Gets the load factor of the hash table as a percentage
     * 
     * @tparam Key 
     * @tparam Value 
     * @return int32_t 
     */
    template<typename Key, typename Value>
    int32_t OpenAddressingHashTable<Key, Value>::getLoadFactor() const
    {
        return static_cast<int32_t>(_size * 100 / _capacity);
    }

    /**
     * @brief Gets the current size of the hash table
     * 
     * @tparam Key 
     * @tparam Value 
     * @return int32_t 
     */
    template<typename Key, typename Value>
    int32_t OpenAddressingHashTable<Key, Value>::getSize() const
    {
        return static_cast<int32_t>(_size);
    }

    /**
     * @brief Checks if the hash table is empty
     * 
     * @tparam Key 
     * @tparam Value 
     * @return true 
     * @return false 
     */
    template<typename Key, typename Value>
    bool OpenAddressingHashTable<Key, Value>::isEmpty() const
    {
        return _size == 0;
    }

    /**
     * @brief Hash function for the key
     * 
     * @tparam Key 
     * @tparam Value 
     * @param key 
     * @return size_t 
     */
    template<typename Key, typename Value>
    size_t OpenAddressingHashTable<Key, Value>::hash(const Key& key) const
    {
        return std::hash<Key>{}(key) % _capacity;
    }

    /**
     * @brief Linear probing function to find the next index
     * 
     * @tparam Key 
     * @tparam Value 
     * @param index 
     * @return size_t 
     */
    template<typename Key, typename Value>
    size_t OpenAddressingHashTable<Key, Value>::probe(size_t index) const
    {
        return (index + 1) % _capacity;
    }

}
