#pragma once

enum responseTypes { toNcurses, toDummy,
	left, right, up, down, noResponse };

extern "C" responseTypes sampleFunction();
