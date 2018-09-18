#pragma once

#include <IWindow.hpp>
#include <IMusicPlayer.hpp>
#include <unordered_set>

namespace SnakeUtils
{
    struct Point
    {
        size_t x = 0;
        size_t y = 0;
        GameFieldCellType cellType = empty;
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
    
        Snake(NibblerParameters, std::shared_ptr<IMusicPlayer>);
        Snake(Snake const&) = delete;
        Snake& operator=(Snake const&) = delete;
        ~Snake() = default;

        using Direction = ResponseType;

        void fillMap(GameField&) const;
        void move(const Direction = noResponse);

        inline bool collapsed() const
        { return outOfField_ || hitBody_ || hitObstacle_; }
        inline double getSpeed() const
        {  return speed_; }
        inline size_t getScore() const
        { return score_; }

    private:

        void updateDirection(const Direction newDirection);
        SnakeUtils::Point defineNewHeadPosition() const;
        bool validNewDirection(const Direction newDirection) const;
        bool headHitBody() const;
        void processCollisionwithFieldObjects();

        SnakeUtils::Point generatePoint(GameFieldCellType) const;

        NibblerParameters params_{40,50,classic};

        SnakeUtils::Point headPos_ {params_.width / 2, params_.height / 2};
        std::vector<SnakeUtils::Point> body_ {{headPos_.x - 1, headPos_.y},
            {headPos_.x - 2, headPos_.y}, {headPos_.x - 3, headPos_.y},
                {headPos_.x - 3, headPos_.y - 1}};
        Direction currentDirection_ = right;

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

        std::shared_ptr<IMusicPlayer> musicPlayer_;

};
