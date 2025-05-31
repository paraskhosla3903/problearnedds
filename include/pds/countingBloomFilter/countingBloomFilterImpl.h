#pragma once

#include <algorithm>
#include <iostream>

namespace pds::bloomFilter
{
    template <typename T>
    CountingBloomFilter<T>::CountingBloomFilter()
        : _k(0), _count(0),
          _bitArray(new std::bitset<BIT_ARRAY_SIZE>()),
          _counterArray(new std::vector<uint8_t>(BIT_ARRAY_SIZE, 0)) {}

    template <typename T>
    CountingBloomFilter<T>::~CountingBloomFilter()
    {
        delete _bitArray;
        delete _counterArray;
    }

    template <typename T>
    void CountingBloomFilter<T>::init(size_t numHashFunctions)
    {
        _k = numHashFunctions;
        _count = 0;
        _bitArray->reset();
        _counterArray->assign(BIT_ARRAY_SIZE, 0);
        _hashFunctions.clear();

        for (size_t i = 0; i < _k; ++i)
        {
            _hashFunctions.emplace_back([i](const T& item) {
                return std::hash<T>{}(item) ^ (i * 0x9e3779b9);
            });
        }

        _visualiser.logAction("[Init] Counting Bloom Filter initialized with " + std::to_string(_k) + " hash functions");
        _visualiser.logState(*this, std::nullopt, VisualContext::INIT);
    }

    template <typename T>
    void CountingBloomFilter<T>::insert(const T& item)
    {
        for (const auto& hashFunc : _hashFunctions)
        {
            size_t idx = hashFunc(item) % BIT_ARRAY_SIZE;
            if ((*_counterArray)[idx] == 0)
            {
                _bitArray->set(idx);
            }
            ++(*_counterArray)[idx];
        }

        _items.insert(item);
        ++_count;

        _visualiser.logAction("\033[32m[Insert]\033[0m " + item);
        _visualiser.logState(*this, std::nullopt, VisualContext::INSERT);
    }

    template <typename T>
    std::optional<float> CountingBloomFilter<T>::query(const T& item) const
    {
        for (const auto& hashFunc : _hashFunctions)
        {
            size_t idx = hashFunc(item) % BIT_ARRAY_SIZE;
            if ((*_counterArray)[idx] == 0)
            {
                _visualiser.logAction("\033[31m[Query Miss]\033[0m " + item + " at index " + std::to_string(idx));
                _visualiser.logState(*this, idx, VisualContext::QUERY);
                return std::nullopt;
            }
        }

        if (_items.find(item) == _items.end())
        {
            _visualiser.logAction("\033[33m[Query False Positive]\033[0m " + item);
        }
        else
        {
            _visualiser.logAction("\033[34m[Query Hit]\033[0m " + item);
        }

        _visualiser.logState(*this, std::nullopt, VisualContext::QUERY);

        return computeFalsePositiveProbability();
    }

    template <typename T>
    void CountingBloomFilter<T>::erase(const T& item)
    {
        if (_items.find(item) == _items.end())
        {
            _visualiser.logAction("\033[31m[Erase Miss]\033[0m " + item);
            return;
        }

        for (const auto& hashFunc : _hashFunctions)
        {
            size_t idx = hashFunc(item) % BIT_ARRAY_SIZE;
            if ((*_counterArray)[idx] > 0)
            {
                --(*_counterArray)[idx];
                if ((*_counterArray)[idx] == 0)
                {
                    _bitArray->reset(idx);
                }
            }
        }

        _items.erase(item);
        --_count;
        _visualiser.logAction("\033[32m[Erase]\033[0m " + item);
        _visualiser.logState(*this, std::nullopt, VisualContext::ERASE);
    }

    template <typename T>
    int32_t CountingBloomFilter<T>::getLoadFactor() const
    {
        return (_count * 100) / BIT_ARRAY_SIZE;
    }

    template <typename T>
    int32_t CountingBloomFilter<T>::getSize() const
    {
        return _count;
    }

    template <typename T>
    bool CountingBloomFilter<T>::isEmpty() const
    {
        return _count == 0;
    }
}