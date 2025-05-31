#pragma once

namespace pds::bloomFilter
{
    /**
     * @brief Construct a new Simple Bloom Filter< T>:: Simple Bloom Filter object
     *
     * @tparam T
     */
    template <typename T>
    SimpleBloomFilter<T>::SimpleBloomFilter()
        : _k(0), _count(0), _bitArray(new std::bitset<BIT_ARRAY_SIZE>()) {}


    /**
     * @brief Destroy the Simple Bloom Filter< T>:: Simple Bloom Filter object
     *
     * @tparam T
     */
    template <typename T>
    SimpleBloomFilter<T>::~SimpleBloomFilter()
    {
        delete _bitArray;
    }

    /**
     * @brief Initialise the Bloom Filter and set number of hash functions, k
     *
     * @tparam T
     * @param numHashFunctions
     */
    template <typename T>
    void SimpleBloomFilter<T>::init(size_t numHashFunctions)
    {
        _k = numHashFunctions;
        _count = 0;
        _bitArray->reset();
        _hashFunctions.clear();

        for (size_t i = 0; i < _k; ++i)
        {
            _hashFunctions.emplace_back([i](const T& item) {
                return std::hash<T>{}(item) ^ (i * 0x9e3779b9);
            });
        }

        _visualiser.logAction("[Init] Bloom Filter initialized with " + std::to_string(_k) + " hash functions");
        _visualiser.logState(*this, std::nullopt, VisualContext::INIT);
    }

    /**
     * @brief Insert an item into the Bloom Filter
     *
     * @tparam T
     * @param item
     */
    template <typename T>
    void SimpleBloomFilter<T>::insert(const T& item)
    {
        for (const auto& hashFunc : _hashFunctions)
        {
            size_t idx = hashFunc(item) % BIT_ARRAY_SIZE;
            if (!_bitArray->test(idx))
            {
                ++_count;
                _bitArray->set(idx);
            }

            _items.insert(item); // Track inserted items

            _visualiser.logAction("[Insert] " + item + " -> Hash index: " + std::to_string(idx));
            _visualiser.logState(*this, idx, VisualContext::INSERT);
        }
    }

    /**
     * @brief Query if an item is possibly in the Bloom Filter
     *
     * @tparam T 
     * @param item 
     * @return std::optional<float>
     */
    template <typename T>
    std::optional<float> SimpleBloomFilter<T>::query(const T& item) const
    {
        for (const auto& hashFunc : _hashFunctions)
        {
            size_t idx = hashFunc(item) % BIT_ARRAY_SIZE;
            if (!_bitArray->test(idx))
            {
                _visualiser.logAction("\033[31m[Query Miss]\033[0m " + item + " at index " + std::to_string(idx));
                _visualiser.logState(*this, idx, VisualContext::QUERY);
                return std::nullopt;
            }
        }

        if(_items.find(item) == _items.end())
        {
            _visualiser.logAction("\033[33m[Query False Positive]\033[0m " + item);
        }
        else
        {
            _visualiser.logAction("\033[34m[Query Hit]\033[0m " + item);
        }

        _visualiser.logState(*this, std::nullopt, VisualContext::QUERY);

        const auto falsePositiveProbability = computeFalsePositiveProbability();
        return std::make_optional<float>(falsePositiveProbability);
    }

    /**
     * @brief Gets the load factor of the Bloom Filter as a percentage
     * which represents the ratio of set bits to total bits
     *
     * @tparam T 
     * @return int32_t 
     */
    template <typename T>
    int32_t SimpleBloomFilter<T>::getLoadFactor() const
    {
        return (_count * 100) / BIT_ARRAY_SIZE;
    }

    /**
     * @brief Total number of set bits in the Bloom Filter
     *
     * @tparam T 
     * @return int32_t 
     */
    template <typename T>
    int32_t SimpleBloomFilter<T>::getSize() const
    {
        return (_count);
    }

    /**
     * @brief Checks if the Bloom Filter is empty
     *
     * @tparam T 
     * @return true 
     * @return false 
     */
    template <typename T>
    bool SimpleBloomFilter<T>::isEmpty() const
    {
        return _count == 0;
    }
}