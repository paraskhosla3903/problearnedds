#pragma once

#include <iostream>
#include <iomanip>
#include <optional>
#include <string>
#include <vector>

#include "pds/core/common.h"

namespace pds::bloomFilter
{
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
                      pds::VisualContext ctx = pds::VisualContext::UNKNOWN) const
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
                        case pds::VisualContext::INSERT: std::cout << "\033[44m"; break; // Blue
                        case pds::VisualContext::QUERY:  std::cout << "\033[43m"; break; // Yellow
                        case pds::VisualContext::ERASE:  std::cout << "\033[41m"; break; // Red
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

            // add printing of the items in the set _items in the form of a table with column header items
            std::cout << "Items in the Counting Bloom Filter:\n";
            if (table._items.empty())
            {
                std::cout << "  No items inserted.\n";
            }
            else
            {
                std::cout << "  " << std::setw(20) << "Item" << "\n";
                std::cout << "  " << std::string(20, '-') << "\n";
                for (const auto& item : table._items)
                {
                    std::cout << "  " << std::setw(20) << item << "\n";
                }
            }
            std::cout << '\n';
        }
    };
}