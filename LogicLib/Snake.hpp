#pragma once
#include "../IWindow/IWindow.hpp"

struct Point
{
	size_t x = 0;
	size_t y = 0;
	bool operator==(Point const& rhs)
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
	bool isOutOfField() const;
private:
	void updateDirection(const direction newDirection);
	Point defineNewHeadPosition() const;
	bool validNewDirection(const direction newDirection) const;
	Point headPos_ {3, 0};
	std::vector<Point> body_ {{2, 0}, {1, 0}, {0, 0}, {0, 1}};
	direction currentDirection_ = right;
	size_t width_ = 0;
	size_t height_ = 0;
	bool outOfField_ = false;
	Point foodPos_ {10, 10};
};
