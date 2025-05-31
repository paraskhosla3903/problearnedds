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
        bool query(const T& item) const;

        int32_t getLoadFactor() const;
        int32_t getSize() const;
        bool isEmpty() const;

        private:
        size_t _k; // Number of hash functions
        int32_t _count; // count of number of set bits in the bit array
        std::bitset<BIT_ARRAY_SIZE>* _bitArray;

        std::vector<std::function<size_t(const T&)>> _hashFunctions;

        SimpleBloomFilterVisualiser<T> _visualiser;
    };
}

#include "simpleBloomFilterImpl.h"