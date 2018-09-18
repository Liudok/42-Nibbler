#include "Snake.hpp"

Snake::Snake(NibblerParameters params, std::shared_ptr<IMusicPlayer> mp) 
    : params_(params), musicPlayer_(mp)
{
    srand(time(NULL));
    fieldObjects_.insert(generatePoint(food));
    fieldObjects_.insert(generatePoint(food));
    fieldObjects_.insert(generatePoint(superFood));
    fieldObjects_.insert(generatePoint(obstacle));
}

void Snake::fillMap(GameField& field) const
{
    for(auto& line : field)
        for (auto& cell : line)
            cell = empty;
    for (auto const& fieldObject : fieldObjects_)
        field[fieldObject.y][fieldObject.x] =
            fieldObject.cellType;
    for (auto const& bodyPart : body_)
        field[bodyPart.y][bodyPart.x] = body;
    field[headPos_.y][headPos_.x] =
        collapsed() ? collision : head;
}

void Snake::move(const Direction newDirection)
{
    processCollisionwithFieldObjects();
    if (collapsed()) return;
    updateDirection(newDirection);
    const auto newHeadPosition = defineNewHeadPosition();
    const auto overflow = std::numeric_limits<size_t>::max();
    if (newHeadPosition.x == params_.width || newHeadPosition.x == overflow ||
        newHeadPosition.y == params_.height || newHeadPosition.y == overflow){
        outOfField_ = true;
        return;
    }
    for (size_t i = body_.size() - 1; i > 0; --i)
        body_[i] = body_[i - 1];
    body_[0] = headPos_;
    headPos_ = newHeadPosition;
    if (headHitBody()) hitBody_ = true;
}

void Snake::updateDirection(const Direction newDirection)
{
    if (validNewDirection(newDirection))
        currentDirection_ = newDirection;
}

SnakeUtils::Point Snake::defineNewHeadPosition() const
{
    SnakeUtils::Point result = headPos_;
    if (currentDirection_ == left)
        --result.x;
    else if (currentDirection_ == right)
        ++result.x;
    else if (currentDirection_ == up)
        --result.y;
    else if (currentDirection_ == down)
        ++result.y;
    else
        throw std::runtime_error("Invalid direction");
    return result;
}

bool Snake::validNewDirection(const Direction newDirection) const
{
    if ((currentDirection_ == left || currentDirection_ == right) && 
        (newDirection == up || newDirection == down))
        return true;
    if ((currentDirection_ == up || currentDirection_ == down) && 
        (newDirection == left || newDirection == right))
        return true;
    return false;
}

bool Snake::headHitBody() const
{
    for (const auto& bodyPart : body_)
        if (bodyPart == headPos_)
            return true;
    return false;
}

void Snake::processCollisionwithFieldObjects()
{
    
    if (!futureBodyParts_.empty()){
        body_.push_back(futureBodyParts_.front());
        futureBodyParts_.pop_front();
    }
    for (auto const& fieldObject : fieldObjects_){
        if (headPos_ == fieldObject){
            processFunctions_[adjustCellTypeIndex(fieldObject.cellType)]
                (fieldObject);
            return;
        }
    }
}

auto Snake::initBody() const
    -> BodyParts
{
    return{
        {headPos_.x - 1, headPos_.y},
        {headPos_.x - 2, headPos_.y},
        {headPos_.x - 3, headPos_.y},
        {headPos_.x - 3, headPos_.y - 1}
    };
}

auto Snake::generatePoint(GameFieldCellType t) const
    -> SnakeUtils::Point
{
    return {rand() % params_.width, rand() % params_.height, t};
}

auto Snake::initProcessFunctions()
    -> ProcessFunctionsArray
{
    return {{
        [this](const auto& o){ processCollisionWithFood(o); },
        [this](const auto& o){ processCollisionWithSuperFood(o); },
        [this](const auto&){ processCollisionWithObstacle(); }
    }};
}

void Snake::processCollisionWithFood(SnakeUtils::Point const& fieldObject)
{
    musicPlayer_->playSound(foodEaten);
    speed_ += speedIncrement_;
    score_ += scoreIncrement_;
    const auto newBodyPart = body_[body_.size() - 1];
    futureBodyParts_.push_front(std::move(newBodyPart));
    const size_t priorSize = fieldObjects_.size();
    while (fieldObjects_.size() == priorSize)
        fieldObjects_.insert(generatePoint(food));
    fieldObjects_.erase(fieldObject);
}

void Snake::processCollisionWithSuperFood(SnakeUtils::Point const& fieldObject)
{
    musicPlayer_->playSound(superFoodEaten);
    score_ += scoreIncrement_ * superFoodFactor_;
    for (size_t i = 0; i < superFoodFactor_; ++i){
        const auto newBodyPart = body_[body_.size() - 1];
        futureBodyParts_.push_front(std::move(newBodyPart));
    }
    const size_t priorSize = fieldObjects_.size();
    while (fieldObjects_.size() == priorSize)
        fieldObjects_.insert(generatePoint(superFood));
    fieldObjects_.erase(fieldObject);
    fieldObjects_.insert(generatePoint(obstacle));
}

void Snake::processCollisionWithObstacle()
{
    hitObstacle_ = true;
}

size_t Snake::adjustCellTypeIndex(GameFieldCellType t)
{
    const auto enumPoseAdjucter = 3;
    return static_cast<size_t>(t - enumPoseAdjucter);
}
