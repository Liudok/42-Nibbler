#pragma once

#include <vector>

enum responseType { noResponse, left, right, up, down,
		toNcurses, toDummy, };

extern "C"
{
	responseType getResponse();
	void drow(std::vector<std::vector<size_t>> const&);
}
