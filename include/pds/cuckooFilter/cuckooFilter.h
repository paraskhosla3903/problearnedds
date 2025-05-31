#pragma once

#include <vector>
#include <optional>
#include <functional>
#include <string>

#include "pds/core/common.h"
#include "cuckooFilterVisualiser.h"

namespace pds::bloomFilter {

    template <typename T>
    class CuckooFilter {
        friend class CuckooFilterVisualiser<T>;

        public:
        CuckooFilter();
        ~CuckooFilter();

        void init(size_t numBuckets = 1024, size_t entriesPerBucket = 4, size_t fingerprintSize = 8, size_t maxKicks = 500);

        bool insert(const T& item);
        std::optional<bool> query(const T& item) const;
        bool erase(const T& item);

        int32_t getSize() const;
        bool isEmpty() const;

        private:
        using Bucket = std::vector<uint8_t>;

        size_t _numBuckets;
        size_t _entriesPerBucket;
        size_t _fingerprintSize;
        size_t _maxKicks;
        size_t _count;

        std::vector<Bucket>* _table;
        CuckooFilterVisualiser<T> _visualiser;

        uint8_t fingerprint(const T& item) const;
        size_t hash1(const T& item) const;
        size_t hash2(size_t h1, uint8_t fp) const;

        std::hash<T> _hasher;
    };

}

#include "cuckooFilterImpl.h"