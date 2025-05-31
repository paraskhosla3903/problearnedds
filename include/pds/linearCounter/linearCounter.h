#pragma once

#include <vector>
#include <bitset>
#include <cmath>
#include <optional>
#include <string>

#include "pds/core/common.h"
#include "linearCounterVisualiser.h"

namespace pds::cardinality
{
    template <typename T>
    class LinearCounter
    {
        friend class LinearCounterVisualiser<T>;

        public:
        LinearCounter();
        ~LinearCounter();

        void init(size_t bitmapSize = BIT_ARRAY_SIZE);

        void insert(const T& item);
        std::optional<float> estimate() const;

        int32_t getSize() const;
        bool isEmpty() const;

        private:
        size_t _m; // Bitmap size
        size_t _count;
        std::bitset<BIT_ARRAY_SIZE>* _bitArray;
        std::hash<T> _hasher;

        LinearCounterVisualiser<T> _visualiser;
    };
}

#include "linearCounterImpl.h"