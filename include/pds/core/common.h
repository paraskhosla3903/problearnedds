#pragma once

#include <iostream>

#define BIT_ARRAY_SIZE 1024

namespace pds
{
    enum class VisualContext : uint8_t
    {
        INIT,
        INSERT,
        QUERY,
        ERASE,
        UNKNOWN
    };

    inline std::string toString(VisualContext ctx) const
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
}
