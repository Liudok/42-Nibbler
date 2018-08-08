#pragma once

enum responseType { noResponse, left, right, up, down,
		toNcurses, toDummy, };

extern "C" responseType getResponse();
