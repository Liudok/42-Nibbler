#pragma once

#include <vector>

enum responseType { noResponse, left, right, up, down,
        toGlwf, toSDL, toSFML, pauseContinue, endGame };

constexpr size_t nbResponses = 10;

enum gameFieldCellType { empty, body, head, food, obstacle, collision };

constexpr size_t nbGameFieldCellTypes = 6;

using field = std::vector<std::vector<size_t>>;

class IWindow
{
public:
    virtual responseType getResponse() = 0;
    virtual void draw(field const&, size_t score, size_t speed) = 0;
    virtual void openWindow(size_t width, size_t height) = 0;
    virtual void closeWindow() = 0;
    virtual void showGameOver() = 0;
    virtual ~IWindow() {}
};
