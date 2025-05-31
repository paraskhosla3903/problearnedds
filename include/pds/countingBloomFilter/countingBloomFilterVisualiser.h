#pragma once

#include <iostream>
#include <iomanip>
#include <optional>
#include <string>
#include <vector>

#include "pds/core/common.h"

namespace pds::bloomFilter
{
    enum class VisualContext : uint8_t
    {
        INIT,
        INSERT,
        QUERY,
        ERASE,
        UNKNOWN
    };

    inline std::string toString(VisualContext ctx)
    {
        switch (ctx)
        {
            case VisualContext::INIT: return "INIT";
            case VisualContext::INSERT: return "INSERT";
            case VisualContext::QUERY: return "QUERY";
            case VisualContext::ERASE: return "ERASE";
            default: return "UNKNOWN";
        }
    }

    template <typename T>
    class CountingBloomFilter; // Forward declaration

    template <typename T>
    class CountingBloomFilterVisualiser
    {
    public:
        void logAction(const std::string& action) const
        {
            std::cout << action << "\n";
        }

        void logState(const CountingBloomFilter<T>& table,
                      std::optional<size_t> highlight = std::nullopt,
                      VisualContext ctx = VisualContext::UNKNOWN) const
        {
            constexpr size_t rowSize = 32;
            std::cout << "\n[Counting Bloom Filter State] Context: " << toString(ctx) << "\n\n";

            // Print Counter Array
            for (size_t i = 0; i < BIT_ARRAY_SIZE; ++i)
            {
                if (i % rowSize == 0)
                {
                    std::cout << std::setw(4) << i << ": ";
                }

                bool isHighlighted = highlight.has_value() && highlight.value() == i;
                int32_t count = (*table._counterArray)[i];

                if (isHighlighted)
                {
                    switch (ctx)
                    {
                        case VisualContext::INSERT: std::cout << "\033[44m"; break; // Blue
                        case VisualContext::QUERY:  std::cout << "\033[43m"; break; // Yellow
                        case VisualContext::ERASE:  std::cout << "\033[41m"; break; // Red
                        default:                    std::cout << "\033[47m"; break; // White
                    }
                }
                else
                {
                    std::cout << (count > 0 ? "\033[42m" : "\033[41m"); // Green if active, Red if 0
                }

                std::cout << std::setw(3) << count << " ";
                std::cout << "\033[0m";

                if ((i + 1) % rowSize == 0)
                {
                    std::cout << '\n';
                }
            }

            if (BIT_ARRAY_SIZE % rowSize != 0)
            {
                size_t start = BIT_ARRAY_SIZE - (BIT_ARRAY_SIZE % rowSize);
                std::cout << "  <- [" << start << " - " << BIT_ARRAY_SIZE - 1 << "]\n";
            }

            std::cout << "\n";
        }
    };
}