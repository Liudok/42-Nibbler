#include "Snake.hpp"

Snake::Snake(size_t width, size_t height)
	: width_(width), height_(height) {}

void Snake::fillMap(gameField& field) const
{
	for(auto& line : field)
		for (auto& cell : line)
			cell = empty;
	field[head_.y][head_.x] = head;
	for (auto const& bodyPart : body_)
		field[bodyPart.y][bodyPart.x] = body;
}

void Snake::move(const direction newDirection)
{
	updateDirection(newDirection);
	const auto newHeadPosition = defineNewHeadPosition();
	const size_t overflow = std::numeric_limits<size_t>::max();
	if (newHeadPosition.x == width_ || newHeadPosition.x == overflow ||
		newHeadPosition.y == height_ || newHeadPosition.y == overflow){
		outOfField_ = true;
		return;
	}
	for (size_t i = body_.size() - 1; i > 0; --i)
		body_[i] = body_[i - 1];
	body_[0] = head_;
	head_ = newHeadPosition;
}

bool Snake::isOutOfField() const
{
	return outOfField_;
}

void Snake::updateDirection(const direction newDirection)
{
	if (validNewDirection(newDirection))
		currentDirection_ = newDirection;
}

Point Snake::defineNewHeadPosition() const
{
	Point result = head_;
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
