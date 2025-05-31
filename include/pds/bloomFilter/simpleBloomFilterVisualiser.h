#pragma once

#include <iostream>
#include <optional>
#include <bitset>
#include <iomanip>

#include "pds/core/common.h"

namespace pds::bloomFilter {

    enum class VisualContext : uint8_t
    {
        INIT,
        INSERT,
        QUERY,
        UNKNOWN
    };

    template <typename T>
    class SimpleBloomFilter; // Forward declaration

    template <typename T>
    class SimpleBloomFilterVisualiser
    {
        public:
        void logState(const SimpleBloomFilter<T>& table,
                      std::optional<size_t> highlight = std::nullopt,
                      VisualContext ctx = VisualContext::UNKNOWN) const
        {
            constexpr size_t rowSize = 32;
            std::cout << "\nBit Array State:\n\n";

            for (size_t i = 0; i < BIT_ARRAY_SIZE; ++i)
            {
                bool isSet = table._bitArray->test(i);

                if (highlight.has_value() && highlight.value() == i)
                {
                    if (ctx == VisualContext::INSERT) std::cout << "\033[44m"; // Blue background
                    else if (ctx == VisualContext::QUERY) std::cout << "\033[43m"; // Yellow background
                    else std::cout << "\033[47m"; // White/gray
                }
                else
                {
                    std::cout << (isSet ? "\033[42m" : "\033[41m"); // Green or Red background
                }

                std::cout << "  ";
                std::cout << "\033[0m";

                if ((i + 1) % rowSize == 0)
                {
                    std::cout << "  <- [" << std::setw(3) << (i - rowSize + 1)
                              << " - " << std::setw(3) << i << "]\n";
                }
            }

            if (BIT_ARRAY_SIZE % rowSize != 0)
            {
                std::cout << "  <- [" << BIT_ARRAY_SIZE - (BIT_ARRAY_SIZE % rowSize)
                          << " - " << BIT_ARRAY_SIZE - 1 << "]\n";
            }

            std::cout << "\n";
        }

        void logAction(const std::string& action) const
        {
            std::cout << "[LOG] " << action << "\n";
        }
    };
} // namespace pds::bloomFilter