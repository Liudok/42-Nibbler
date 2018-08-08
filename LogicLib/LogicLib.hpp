#pragma once
#include <array>
#include <vector>

int sampleSum(int a, int b);

class LogicUnit
{
public:
	LogicUnit();
	void loopTheGame();
	~LogicUnit();
private:
	enum libraryType { ncurses, dummy };
	enum responseType { noResponse, left, right, up, down,
		toNcurses, toDummy, };
	enum gameStateType { empty, body, head, food };
	static constexpr size_t nbLibraries = 2;
	static constexpr size_t height = 6;
	static constexpr size_t width = 10;
	using ptrToLibraryType = void*;
	using responseFunctionsType = unsigned long(*)();
	std::array<ptrToLibraryType, nbLibraries> initLibraries();
	std::array<responseFunctionsType, nbLibraries> initGetResponseFunctions();
	responseType getResponse() const;
	void sendGameState() const; 
	std::array<ptrToLibraryType, nbLibraries> libraries_;
	std::array<responseFunctionsType, nbLibraries>
		librariesFunctionsGetResponsePtrs_;
	libraryType currentLibrary_;
	std::vector<std::vector<size_t>> gameState_;
};
