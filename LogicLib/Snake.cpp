#include "Snake.hpp"

Snake::Snake(size_t width, size_t height)
	: width_(width), height_(height) {}

void Snake::fillMap(gameField& field) const
{
	for(auto& line : field)
		for (auto& cell : line)
			cell = empty;
	field[foodPos_.y][foodPos_.x] = food;
	field[headPos_.y][headPos_.x] = head;
	for (auto const& bodyPart : body_)
		field[bodyPart.y][bodyPart.x] = body;
}

void Snake::move(const direction newDirection)
{
	const size_t overflow = std::numeric_limits<size_t>::max();
	if (headPos_ == foodPos_){
		auto newBodyPart = body_[body_.size() - 1];
		--newBodyPart.x;
		if (newBodyPart.x == overflow){
			outOfField_ = true;
			return;
		}
		body_.push_back(std::move(newBodyPart));
		srand(time(NULL));
		foodPos_ = {rand() % width_, rand() % height_};
	}
	updateDirection(newDirection);
	const auto newHeadPosition = defineNewHeadPosition();
	if (newHeadPosition.x == width_ || newHeadPosition.x == overflow ||
		newHeadPosition.y == height_ || newHeadPosition.y == overflow){
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
