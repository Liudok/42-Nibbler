#pragma once

enum responseType { toNcurses, toDummy,
	left, right, up, down, noResponse };

extern "C" responseType getResponse();
