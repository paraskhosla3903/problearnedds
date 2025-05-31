#pragma once

#include <bitset>
#include <vector>
#include <functional>
#include <optional>
#include <iomanip>
#include <string>
#include <iostream>

#include "pds/core/common.h"
#include "simpleBloomFilterVisualiser.h"

namespace pds::bloomFilter
{
    template <typename T>
    class SimpleBloomFilter
    {
        friend class SimpleBloomFilterVisualiser<T>;

        public:
        SimpleBloomFilter();
        ~SimpleBloomFilter();

        void init(size_t numHashFunctions);

        void insert(const T& item);
        std::optional<float> query(const T& item) const;

        int32_t getLoadFactor() const;
        int32_t getSize() const;
        bool isEmpty() const;

        private:
        size_t _k; // Number of hash functions
        int32_t _count; // count of number of set bits in the bit array
        std::bitset<BIT_ARRAY_SIZE>* _bitArray;

        std::vector<std::function<size_t(const T&)>> _hashFunctions;

        SimpleBloomFilterVisualiser<T> _visualiser;

        private:

        float computeFalsePositiveProbability() const
        {
            if (_k == 0 || _count == 0)
                return 0.0f;

            float n = static_cast<float>(_count);              // Number of bits set
            float m = static_cast<float>(BIT_ARRAY_SIZE);      // Bit array size

            float exponent = -static_cast<float>(_k) * (n / m);
            float base = 1.0f - std::exp(exponent);

            // Clamp base to [0,1] to prevent domain errors in pow()
            if (base < 0.0f) base = 0.0f;
            if (base > 1.0f) base = 1.0f;

            float falsePositiveProb = std::pow(base, static_cast<float>(_k));

            return falsePositiveProb;
        }
    };
}

#include "simpleBloomFilterImpl.h"