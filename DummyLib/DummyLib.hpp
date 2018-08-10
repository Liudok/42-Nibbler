#pragma once
#include "../IWindow/IWindow.hpp"

extern "C"
{
	IWindow* create();
}

class DummyWindow : public IWindow
{
public:
	responseType getResponse() override;
	void drow(std::vector<std::vector<size_t>> const&) override;
};
