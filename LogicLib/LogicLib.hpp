#pragma once
#include <array>

int sampleSum(int a, int b);

class LogicUnit
{
public:
	LogicUnit();
	void loopTheGame();
	~LogicUnit();
private:
	enum libraryType { ncurses, dummy };
	enum responseType { noResponse, toNcurses, toDummy,
		left, right, up, down };
	static constexpr size_t nbLibraries = 2;
	using ptrToLibraryType = void*;
	using responseFunctionsType = unsigned long(*)();
	std::array<ptrToLibraryType, nbLibraries> initLibraries();
	std::array<responseFunctionsType, nbLibraries> initGetResponseFunctions();
	responseType getResponse();
	std::array<ptrToLibraryType, nbLibraries> libraries_;
	std::array<responseFunctionsType, nbLibraries>
		librariesFunctionsGetResponsePtrs_;
	libraryType currentLibrary_;
};
