#pragma once

#include <iostream>
#include <iomanip>
#include <optional>
#include "pds/core/common.h"

namespace pds::cardinality
{
    template <typename T>
    class LinearCounter;

    template <typename T>
    class LinearCounterVisualiser
    {
    public:
        void logAction(const std::string& action) const
        {
            std::cout << action << "\n";
        }

        void logState(const LinearCounter<T>& counter,
                      std::optional<size_t> highlight = std::nullopt,
                      pds::VisualContext ctx = pds::VisualContext::UNKNOWN) const
        {
            std::cout << "\n[Bit Array State] Context: ";
            switch (ctx)
            {
                case pds::VisualContext::INIT: std::cout << "INIT"; break;
                case pds::VisualContext::INSERT: std::cout << "INSERT"; break;
                case pds::VisualContext::QUERY: std::cout << "QUERY"; break;
                default: std::cout << "UNKNOWN";
            }
            std::cout << "\n\n";

            constexpr size_t rowSize = 32;
            for (size_t i = 0; i < counter._m; ++i)
            {
                bool isSet = counter._bitArray->test(i);
                if (highlight.has_value() && highlight.value() == i)
                {
                    std::cout << "\033[44m"; // Blue background
                }
                else
                {
                    std::cout << (isSet ? "\033[42m" : "\033[41m"); // Green or Red
                }

                std::cout << "  \033[0m";

                if ((i + 1) % rowSize == 0)
                {
                    std::cout << "  <- [" << std::setw(3) << (i - rowSize + 1)
                              << " - " << std::setw(3) << i << "]\n";
                }
            }

            if (counter._m % rowSize != 0)
            {
                std::cout << "  <- [" << counter._m - (counter._m % rowSize)
                          << " - " << counter._m - 1 << "]\n";
            }

            std::cout << "\n";
        }
    };
}