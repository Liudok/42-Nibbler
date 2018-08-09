#pragma once

#include <vector>

enum responseType { noResponse, toNcurses, toDummy,
	left, right, up, down };

extern "C"
{
	responseType getResponse();
	void drow(std::vector<std::vector<size_t>> const&);
}
