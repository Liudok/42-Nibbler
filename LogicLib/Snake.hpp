#pragma once

#include <IWindow.hpp>
#include <IMusicPlayer.hpp>
#include <unordered_set>
#include <array>
#include <forward_list>

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
        { return speed_; }
        inline size_t getScore() const
        { return score_; }

    private:

        void updateDirection(const Direction);
        SnakeUtils::Point defineNewHeadPosition() const;
        bool validNewDirection(const Direction) const;
        bool headHitBody() const;
        void processCollisionwithFieldObjects();

        using BodyParts = std::vector<SnakeUtils::Point>;

        BodyParts initBody() const;

        SnakeUtils::Point generatePoint(GameFieldCellType) const;

        static constexpr auto nbProcessFunctions = 3;
        using ProcessFunctionType = std::function<void(SnakeUtils::Point)>;
        using ProcessFunctionsArray = std::array<ProcessFunctionType, nbProcessFunctions>;

        ProcessFunctionsArray initProcessFunctions();
        void processCollisionWithFood(SnakeUtils::Point const&);
        void processCollisionWithSuperFood(SnakeUtils::Point const&);
        void processCollisionWithObstacle(); 

        ProcessFunctionsArray processFunctions_ = initProcessFunctions();

        NibblerParameters params_{defaultWidth, defaultHeight, classic};
        SnakeUtils::Point headPos_{params_.width / 2, params_.height / 2};
        BodyParts body_ = initBody();
        Direction currentDirection_ = right;

        const double speedIncrement_ = 0.05;
        const size_t scoreIncrement_ = 50;
        const size_t superFoodFactor_ = 5;

        bool outOfField_ = false;
        bool hitBody_ = false;
        bool hitObstacle_ = false;
        size_t speed_ = defaultSpeed;
        size_t score_ = defaultScore;

        using FieldObjects =
            std::unordered_set<SnakeUtils::Point,
                SnakeUtils::PointHashBySum>;
        
        FieldObjects fieldObjects_;

        using MusicPlayerSPtr = std::shared_ptr<IMusicPlayer>;

        MusicPlayerSPtr musicPlayer_;

        static size_t adjustCellTypeIndex(GameFieldCellType);

        using FutureBodyParts = std::forward_list<SnakeUtils::Point>;
        FutureBodyParts futureBodyParts_;

};
