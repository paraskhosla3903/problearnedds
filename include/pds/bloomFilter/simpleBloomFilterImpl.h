#pragma once

namespace pds::bloomFilter
{
    template <typename T>
    SimpleBloomFilter<T>::SimpleBloomFilter()
        : _k(0), _count(0), _bitArray(new std::bitset<BIT_ARRAY_SIZE>()) {}

    template <typename T>
    SimpleBloomFilter<T>::~SimpleBloomFilter()
    {
        delete _bitArray;
    }

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

            _visualiser.logAction("[Insert] " + item + " -> Hash index: " + std::to_string(idx));
            _visualiser.logState(*this, idx, VisualContext::INSERT);
        }
    }

    template <typename T>
    bool SimpleBloomFilter<T>::query(const T& item) const
    {
        for (const auto& hashFunc : _hashFunctions)
        {
            size_t idx = hashFunc(item) % BIT_ARRAY_SIZE;
            if (!_bitArray->test(idx))
            {
                _visualiser.logAction("\033[31m[Query Miss]\033[0m " + item + " at index " + std::to_string(idx));
                _visualiser.logState(*this, idx, VisualContext::QUERY);
                return false;
            }
        }
        _visualiser.logAction("\033[32m[Query Hit]\033[0m " + item);
        _visualiser.logState(*this, std::nullopt, VisualContext::QUERY);
        return true;
    }

    template <typename T>
    int32_t SimpleBloomFilter<T>::getLoadFactor() const
    {
        return (_count * 100) / BIT_ARRAY_SIZE;
    }

    template <typename T>
    int32_t SimpleBloomFilter<T>::getSize() const
    {
        return (_count);
    }

    template <typename T>
    bool SimpleBloomFilter<T>::isEmpty() const
    {
        return _count == 0;
    }
}