#include "Snake.hpp"
#include <forward_list>

bool SnakeUtils::Point::operator==(Point const& rhs) const
{
    return x == rhs.x && y == rhs.y;
}

size_t SnakeUtils::PointHashBySum::operator()
    (Point const& point) const
{
    return std::hash<size_t>()(point.x * point.y);
}

Snake::Snake(NibblerParameters params, std::shared_ptr<IMusicPlayer> mp) 
    : params_(params), musicPlayer_(mp)
{
    srand(time(NULL));
    fieldObjects_.insert({0,0,food});
    fieldObjects_.insert({params_.width-1,params_.height-1,food});
    fieldObjects_.insert({8,8,superFood});
    fieldObjects_.insert({6,6,obstacle});
    fieldObjects_.insert({7,7,obstacle});
}

Snake::~Snake()
{

}

void Snake::fillMap(gameField& field) const
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

void Snake::move(const direction newDirection)
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
    if (headHitBody())
        hitBody_ = true;
}

double Snake::getSpeed() const
{
    return speed_;
}

size_t Snake::getScore() const
{
    return score_;
}

bool Snake::collapsed() const
{
    return outOfField_ || hitBody_ || hitObstacle_;
}

void Snake::updateDirection(const direction newDirection)
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

bool Snake::validNewDirection(const direction newDirection) const
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
    static std::forward_list<SnakeUtils::Point> futureBodyParts;
    if (!futureBodyParts.empty()){
        body_.push_back(futureBodyParts.front());
        futureBodyParts.pop_front();
    }
    for (auto const& fieldObject : fieldObjects_){
        if (headPos_ == fieldObject){
            if (fieldObject.cellType == food){
                musicPlayer_->playSound(foodEaten);
                speed_ += speedIncrement_;
                score_ += scoreIncrement_;
                const auto newBodyPart = body_[body_.size() - 1];
                futureBodyParts.push_front(std::move(newBodyPart));
                const size_t priorSize = fieldObjects_.size();
                while (fieldObjects_.size() == priorSize)
                    fieldObjects_.insert(generatePoint(food));
                fieldObjects_.erase(fieldObject);
                return;
            }
            else if (fieldObject.cellType == superFood){
                musicPlayer_->playSound(superFoodEaten);
                score_ += scoreIncrement_ * superFoodFactor_;
                for (size_t i = 0; i < superFoodFactor_; ++i){
                    const auto newBodyPart = body_[body_.size() - 1];
                    futureBodyParts.push_front(std::move(newBodyPart));
                }
                const size_t priorSize = fieldObjects_.size();
                while (fieldObjects_.size() == priorSize)
                    fieldObjects_.insert(generatePoint(superFood));
                fieldObjects_.erase(fieldObject);
                return;
            }
            else if (fieldObject.cellType == obstacle){
                hitObstacle_ = true;
                return;
            }
        }
    }
}

SnakeUtils::Point Snake::generatePoint(GameFieldCellType t) const
{
    return {rand() % params_.width, rand() % params_.height, t};
}
