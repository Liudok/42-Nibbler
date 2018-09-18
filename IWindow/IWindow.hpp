#pragma once

#include <vector>

enum ResponseType { noResponse, left, right, up, down,
        toNcurses, toSDL, toSFML, pauseContinue, changeGameMode, playerPressedEscape };

constexpr size_t nbResponses = 11;

enum GameFieldCellType { empty, body, head, food, superFood, obstacle, collision };

constexpr size_t nbGameFieldCellTypes = 7;

enum GameMode {classic, granny, insane, rasta};

constexpr size_t nbGameModes = 4;

enum LibraryType { ncurses, sdl, sfml };

constexpr size_t nbLibraries = 3;

constexpr auto defaultZoomFaftor = 15;
constexpr auto defaultWidth = 40;
constexpr auto defaultHeight = 60;
constexpr auto defaultSpeed = 1.0;
constexpr auto defaultScore = 0;
constexpr auto defaultGameMode = classic;
constexpr auto defaultLibrary = sfml;
constexpr auto colorSpectrum = 255;

struct NibblerParameters
{
    size_t width = 30;
    size_t height = 40;
    GameMode mode = defaultGameMode;
    LibraryType lib = defaultLibrary;
};

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
