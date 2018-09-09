#pragma once
#include "../IWindow/IWindow.hpp"

struct Point
{
	size_t x = 0;
	size_t y = 0;
	bool operator==(Point const& rhs) const
	{
		return this->x == rhs.x && this->y == rhs.y;
	}
};

class Snake
{
public:
	using gameField = std::vector<std::vector<size_t>>;
	using direction = responseType;
	Snake(size_t width, size_t height);
	void fillMap(gameField&) const;
	void move(const direction = noResponse);
	bool collapsed() const;
private:
	void updateDirection(const direction newDirection);
	Point defineNewHeadPosition() const;
	bool validNewDirection(const direction newDirection) const;
	bool headHitBody() const;
	size_t width_ = 0;
	size_t height_ = 0;
	Point headPos_ {width_ / 2, height_ / 2};
	std::vector<Point> body_ {{headPos_.x - 1, headPos_.y}, {headPos_.x - 2, headPos_.y}, {headPos_.x - 3, headPos_.y}, {headPos_.x - 3, headPos_.y - 1}};
	direction currentDirection_ = right;

	bool outOfField_ = false;
	bool hitBody_ = false;
	Point foodPos_ {9, 9};
};
