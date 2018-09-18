#pragma once

#include <vector>

enum ResponseType { noResponse, left, right, up, down,
        toNcurses, toSDL, toSFML, pauseContinue, PlayerPressedEscape };

constexpr size_t nbResponses = 10;

enum GameFieldCellType { empty, body, head, food, superFood, obstacle, collision };

constexpr size_t nbGameFieldCellTypes = 7;

enum GameMode {classic, granny, insane, rasta};

constexpr size_t nbGameModes = 4;

struct NibblerParameters
{
    size_t width = 30;
    size_t height = 40;
    GameMode mode = classic;
};

using GameField = std::vector<std::vector<size_t>>;

class IWindow
{
    public:

        virtual ResponseType getResponse() = 0;
        virtual void draw(GameField const&, size_t score, size_t speed) = 0;
        virtual void openWindow(size_t width, size_t height) = 0;
        virtual void closeWindow() = 0;
        virtual void showGameOver() = 0;
        virtual ~IWindow() {}

};
