#pragma once

#include <IWindow.hpp>
#include <unordered_set>

namespace SnakeUtils
{
    struct Point
    {
        size_t x = 0;
        size_t y = 0;
        gameFieldCellType cellType = empty;
        bool operator==(Point const& rhs) const;
    };

    struct PointHashBySum
    {
        size_t operator()(Point const& point) const;
    };
}

class Snake
{
    public:
    
        Snake(NibblerParameters);
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
        SnakeUtils::Point defineNewHeadPosition() const;
        bool validNewDirection(const direction newDirection) const;
        bool headHitBody() const;
        void processCollisionwithFieldObjects();

        SnakeUtils::Point generatePoint(gameFieldCellType) const;

        NibblerParameters params_{40,50,classic};

        SnakeUtils::Point headPos_ {params_.width / 2, params_.height / 2};
        std::vector<SnakeUtils::Point> body_ {{headPos_.x - 1, headPos_.y},
            {headPos_.x - 2, headPos_.y}, {headPos_.x - 3, headPos_.y},
                {headPos_.x - 3, headPos_.y - 1}};
        direction currentDirection_ = right;

        const double speedIncrement_ = 0.05;
        const size_t scoreIncrement_ = 50;
        const size_t superFoodFactor_ = 5;

        bool outOfField_ = false;
        bool hitBody_ = false;
        bool hitObstacle_ = false;
        double speed_ = 1.0;
        size_t score_ = 0;
        std::unordered_set<SnakeUtils::Point,
            SnakeUtils::PointHashBySum> fieldObjects_;
};
