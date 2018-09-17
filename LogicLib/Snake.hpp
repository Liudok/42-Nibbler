#pragma once

#include <IWindow.hpp>
#include <unordered_set>

struct Point
{
    size_t x = 0;
    size_t y = 0;
    bool operator==(Point const& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

};

struct PointHashBySum
{
    size_t operator()(Point const& point) const
    {
        return std::hash<size_t>()(point.x * point.y);
    }
};

class Snake
{
    public:
    
        Snake(size_t width, size_t height);
        Snake(Snake const & other);
        Snake& operator=(Snake const& src);
        ~Snake();

        using direction = responseType;

        void fillMap(gameField&) const;
        void move(const direction = noResponse);
        bool collapsed() const;
        double getSpeed() const;
        size_t getScore() const;

    private:

        void updateDirection(const direction newDirection);
        Point defineNewHeadPosition() const;
        bool validNewDirection(const direction newDirection) const;
        bool headHitBody() const;
        bool headHitObstacle() const;
        void processCollisionWithFood();

        Point generatePoint() const;

        size_t width_ = 0;
        size_t height_ = 0;
        Point headPos_ {width_ / 2, height_ / 2};
        std::vector<Point> body_ {{headPos_.x - 1, headPos_.y},
            {headPos_.x - 2, headPos_.y}, {headPos_.x - 3, headPos_.y},
                {headPos_.x - 3, headPos_.y - 1}};
        direction currentDirection_ = right;

        bool outOfField_ = false;
        bool hitBody_ = false;
        bool hitObstacle_ = false;
        double speed_ = 1.0;
        size_t score_ = 0;
        std::unordered_set<Point, PointHashBySum> foodPos_;
        std::unordered_set<Point, PointHashBySum> obstacles_;
};
