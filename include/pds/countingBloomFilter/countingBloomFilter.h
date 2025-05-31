
#pragma once

#include <vector>
#include <unordered_set>
#include <functional>
#include <optional>
#include <cmath>

#include "pds/core/common.h"
#include "countingBloomFilterVisualiser.h"

namespace pds::bloomFilter
{

    template <typename T>
    class CountingBloomFilter
    {
        friend class CountingBloomFilterVisualiser<T>;

        public:
        CountingBloomFilter();
        ~CountingBloomFilter();

        void init(size_t numHashFunctions);
        void insert(const T& item);
        std::optional<float> query(const T& item) const;
        void erase(const T& item);

        int32_t getLoadFactor() const;
        int32_t getSize() const;
        bool isEmpty() const;

        private:
        size_t _k; // Number of hash functions
        int32_t _count; // Count of number of set bits in the bit array
        std::bitset<BIT_ARRAY_SIZE>* _bitArray; // Using vector to allow counting
        std::vector<uint8_t>* _counterArray; // counter for enabling deletions

        std::vector<std::function<size_t(const T&)>> _hashFunctions;

        std::unordered_set<T> _items; // To track inserted items
        float computeFalsePositiveProbability() const
        {
            if (_k == 0 || _count == 0)
                return 0.0f;

            float n = static_cast<float>(_count);              // Number of bits set
            float m = static_cast<float>(_bitArray->size());   // Bit array size

            float exponent = -static_cast<float>(_k) * (n / m);
            float base = 1.0f - std::exp(exponent);

            // Clamp base to [0,1] to prevent domain errors in pow()
            if (base < 0.0f) base = 0.0f;
            if (base > 1.0f) base = 1.0f;

            float falsePositiveProb = std::pow(base, static_cast<float>(_k));

            return falsePositiveProb;
        }

        CountingBloomFilterVisualiser<T> _visualiser;
    };
}

#include "countingBloomFilterImpl.h"