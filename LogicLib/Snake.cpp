#include "Snake.hpp"
#include <forward_list>

Snake::Snake(size_t width, size_t height)
    : width_(width)
    , height_(height)
{
    srand(time(NULL));
    foodPos_.insert(generateFood());
}

Snake::Snake(Snake const& other)
    : width_(other.width_)
    , height_(other.height_)
{
    
}

Snake& Snake::operator=(Snake const& rhs)
{
  if (this != &rhs)
  {
    width_ = rhs.width_;
    height_ = rhs.height_;
    body_ = rhs.body_;
    speed_ = rhs.speed_;
    score_ = rhs.score_;
  }
  return *this;
}

Snake::~Snake()
{

}

void Snake::fillMap(gameField& field) const
{
    for(auto& line : field)
        for (auto& cell : line)
            cell = empty;
    for (auto const& foodPiece : foodPos_)
        field[foodPiece.y][foodPiece.x] = food;
    for (auto const& bodyPart : body_)
        field[bodyPart.y][bodyPart.x] = body;
    field[headPos_.y][headPos_.x] =
        collapsed() ? collision : head;
}

void Snake::move(const direction newDirection)
{
    processCollisionWithFood();
    updateDirection(newDirection);
    const auto newHeadPosition = defineNewHeadPosition();
    const auto overflow = std::numeric_limits<size_t>::max();
    if (newHeadPosition.x == width_ - 1 || newHeadPosition.x == overflow ||
        newHeadPosition.y == height_ - 1 || newHeadPosition.y == overflow){
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
    return outOfField_ || hitBody_;
}

void Snake::updateDirection(const direction newDirection)
{
    if (validNewDirection(newDirection))
        currentDirection_ = newDirection;
}

Point Snake::defineNewHeadPosition() const
{
    Point result = headPos_;
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
    for (auto const& bodyPart : body_)
        if (bodyPart == headPos_)
            return true;
    return false;
}

void Snake::processCollisionWithFood()
{
    static std::forward_list<Point> futureBodyParts;
    if (!futureBodyParts.empty()){
        body_.push_back(futureBodyParts.front());
        futureBodyParts.pop_front();
    }
    for (auto const& foodPiece : foodPos_){
        if (headPos_ == foodPiece){
            speed_ += 0.05;
            score_ += 50;
            const auto newBodyPart = body_[body_.size() - 1];
            futureBodyParts.push_front(std::move(newBodyPart));
            const size_t priorSize = foodPos_.size();
            while (foodPos_.size() == priorSize)
                foodPos_.insert(generateFood());
            foodPos_.erase(foodPiece);
            return;
        }
    }
}

Point Snake::generateFood() const
{
    size_t y = rand() % height_;
    size_t x = rand() % width_;
    Point result = {(x != 0 && x != width_ - 1) ? x : 9,
        (y != 0 && y != height_ - 1) ? y : 9};
    return result;
}
