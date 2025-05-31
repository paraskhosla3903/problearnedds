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
        /**
         * @brief Logs the current state of the Bloom Filter by printing the bit array
         *
         * @param table The Bloom Filter to log
         * @param highlight Optional index to highlight in the bit array
         * @param ctx Context of the operation (INIT, INSERT, QUERY)
         */
        void logState(const SimpleBloomFilter<T>& table,
                      std::optional<size_t> highlight = std::nullopt,
                      VisualContext ctx = VisualContext::UNKNOWN) const
        {
            constexpr size_t rowSize = 32;
            std::cout << "\nBit Array State:\n\n";

            if(ctx == VisualContext::QUERY)
            {
                std::cout << "\033[1;33m"; // Yellow text for QUERY context
                std::cout << "FP Probability: "
                    << std::fixed << std::setprecision(4)
                    << table.computeFalsePositiveProbability() * 100.0f
                    << "%\033[0m";
                std::cout << '\n';
            }
            

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

        /**
         * @brief Logs the string describing an action taken on the bloom filter
         *
         * @param action 
         */
        void logAction(const std::string& action) const
        {
            std::cout << "[LOG] " << action << "\n";
        }
    };
}