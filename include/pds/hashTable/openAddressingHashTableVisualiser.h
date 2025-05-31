#pragma once

#include <iostream>
#include <optional>
#include <bitset>
#include <iomanip>

#include "pds/core/common.h"

namespace pds::hashTable
{
    template <typename Key, typename Value>
    class OpenAddressingHashTable; // Forward declaration

    template <typename Key, typename Value>
    class OpenAddressingHashTableVisualiser {
    public:
    
        /**
         * @brief Logs the current state of the hash table
         * 
         * @param table The hash table to log
         * @param highlight Optional index to highlight in the bit array
         * @param ctx Context of the operation (INIT, INSERT, QUERY, ERASE)
         */
        void log(const OpenAddressingHashTable<Key, Value>& table,
             std::optional<size_t> highlight = std::nullopt,
             pds::VisualContext ctx = pds::VisualContext::UNKNOWN) const
        {
            constexpr size_t rowSize = 32;
            std::cout << "\nBit Array State:\n\n";

            for (size_t i = 0; i < BIT_ARRAY_SIZE; ++i) {
                const bool isSet = table._bitArray->test(i);

                if (highlight.has_value() && highlight.value() == i)
                {
                    if (ctx == pds::VisualContext::INSERT) std::cout << "\033[44m"; // Blue background
                    else if (ctx == pds::VisualContext::QUERY) std::cout << "\033[43m"; // Yellow background
                    else std::cout << "\033[47m"; // Default white
                }
                else
                {
                    std::cout << (isSet ? "\033[42m" : "\033[41m"); // Green or Red background
                }

                std::cout << "  "; // 2 space block to appear square-like
                std::cout << "\033[0m"; // Reset

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

            std::cout << "\nHash Table Contents:\n\n";
            std::cout << std::left
                      << std::setw(12) << "Hash Index" << " | "
                      << std::setw(20) << "Key" << " | "
                      << std::setw(20) << "Value"
                      << "\n";

            std::cout << std::string(12, '-') << "-+-"
                      << std::string(20, '-') << "-+-"
                      << std::string(20, '-') << "\n";

            // Table rows
            for (size_t i = 0; i < table._capacity; ++i)
            {
                const auto &entry = table._table->at(i);
                if (entry.first != Key{})
                {
                    std::cout << std::left
                              << std::setw(12) << i << " | "
                              << std::setw(20) << entry.first << " | "
                              << std::setw(20) << entry.second
                              << "\n";
                }
            }
            std::cout << "\n";
        }


        /**
         * @brief Logs the string describing an action taken on the hash table
         * 
         * @param action 
         */
        void logAction(const std::string& action) const
        {
            std::cout << "[LOG] " << action << "\n";
        }
    };
}