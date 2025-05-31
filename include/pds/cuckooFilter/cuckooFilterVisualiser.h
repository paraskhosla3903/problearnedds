#pragma once

#include <iostream>
#include <iomanip>
#include <optional>

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

    template <typename T>
    class CuckooFilter;

    template <typename T>
    class CuckooFilterVisualiser
    {
    public:
        void logAction(const std::string& action) const
        {
            std::cout << "[LOG] " << action << "\n";
        }

        void logState(const CuckooFilter<T>& filter,
                      std::optional<size_t> highlight = std::nullopt,
                      VisualContext ctx = VisualContext::UNKNOWN) const
        {
            std::cout << "\n[Table State] Context: ";
            switch (ctx)
            {
                case VisualContext::INIT:   std::cout << "INIT"; break;
                case VisualContext::INSERT: std::cout << "INSERT"; break;
                case VisualContext::QUERY:  std::cout << "QUERY"; break;
                case VisualContext::ERASE:  std::cout << "ERASE"; break;
                default: std::cout << "UNKNOWN"; break;
            }
            std::cout << "\n\n";

            const size_t entriesPerRow = filter._entriesPerBucket;

            // Print header row
            std::cout << " Bucket |";
            for (size_t j = 0; j < entriesPerRow; ++j)
            {
                std::cout << " Slot " << j << " ";
            }
            std::cout << "\n" << std::string(9 + entriesPerRow * 9, '-') << "\n";

            for (size_t i = 0; i < filter._numBuckets; ++i)
            {
                std::cout << std::setw(6) << i << " | ";

                for (size_t j = 0; j < entriesPerRow; ++j)
                {
                    uint8_t val = j < filter._table->at(i).size() ? filter._table->at(i)[j] : 0;
                    bool isOccupied = val != 0;

                    // Highlighting logic
                    if (highlight && highlight.value() == i)
                    {
                        switch (ctx)
                        {
                            case VisualContext::INSERT:
                                std::cout << "\033[44m"; break; // Blue
                            case VisualContext::QUERY:
                                std::cout << "\033[43m"; break; // Yellow
                            case VisualContext::ERASE:
                                std::cout << "\033[41m"; break; // Red
                            case VisualContext::INIT:
                                std::cout << "\033[46m"; break; // Cyan
                            default:
                                std::cout << "\033[47m"; break; // White fallback
                        }
                    }
                    else
                    {
                        std::cout << (isOccupied ? "\033[42m" : "\033[41m"); // Green or Red background
                    }

                    std::cout << std::setw(4) << static_cast<int>(val) << " \033[0m  ";
                }

                std::cout << "\n";
            }

            std::cout << "\n";
        }
    };

}