#pragma once

enum responseType { noResponse, toNcurses, toDummy,
	left, right, up, down };

extern "C" responseType getResponse();
