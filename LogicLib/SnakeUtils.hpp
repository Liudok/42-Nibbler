#pragma once

#include <Nibbler.hpp>
#include <unordered_set>

namespace SnakeUtils
{
    struct Point
    {
        size_t x = 0;
        size_t y = 0;
        GameFieldCellType cellType = empty;
        inline bool operator==(Point const& rhs) const
        { return x == rhs.x && y == rhs.y; }
    };

    struct PointHashBySum
    {
        inline size_t operator()(Point const& point) const
        { return std::hash<size_t>()(point.x * point.y); }
    };
}
