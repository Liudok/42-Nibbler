#pragma once

#include <vector>
#include <cstddef>

enum ResponseType { noResponse, left, right, up, down,
        toNcurses, toSDL, toSFML, pauseContinue, changeGameMode, playerPressedEscape };

constexpr size_t nbResponses = 11;

enum GameFieldCellType { empty, body, head, food, superFood, obstacle, collision };

constexpr size_t nbGameFieldCellTypes = 7;

enum LibraryType { ncurses, sdl, sfml };

constexpr size_t nbLibraries = 3;

enum GameMode {classic, granny, insane, rasta};

constexpr size_t nbGameModes = 4;

using GameField = std::vector<std::vector<size_t>>;

class IWindow
{
    public:

        virtual ResponseType getResponse() = 0;
        virtual void draw(GameField const&, size_t score, size_t speed, GameMode) = 0;
        virtual void openWindow(size_t width, size_t height) = 0;
        virtual void closeWindow() = 0;
        virtual void showGameOver() = 0;
        virtual ~IWindow() {}

};
