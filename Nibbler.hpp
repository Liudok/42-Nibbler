#pragma once

#include "IWindow/IWindow.hpp"
#include "IMusicPlayer/IMusicPlayer.hpp"

constexpr auto defaultZoomFaftor = 15;
constexpr auto defaultWidth = 40;
constexpr auto defaultHeight = 60;
constexpr auto defaultScore = 0;
constexpr auto defaultSpeed = 1.0;
constexpr auto defaultGameMode = classic;
constexpr auto defaultLibrary = sfml;
constexpr auto defaultMusicPlayerState = soundsOnly;
constexpr auto colorSpectrum = 255;

struct NibblerParameters
{
    size_t width = 30;
    size_t height = 40;
    GameMode mode = defaultGameMode;
    LibraryType lib = defaultLibrary;
    MusicPlayerState player = defaultMusicPlayerState;
};
