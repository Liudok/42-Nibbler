#pragma once
#include <IWindow.hpp>

extern "C"
{
	IWindow* create();
}

class DummyWindow : public IWindow
{
public:
	responseType getResponse() override;
	void draw(std::vector<std::vector<size_t>> const&) override;
	void openWindow(size_t width, size_t height) override;
	void closeWindow() override;
};
