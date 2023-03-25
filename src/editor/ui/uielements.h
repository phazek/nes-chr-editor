#pragma once

#include "olc/olcPGEX_QuickGUI.h"
#include "../style.h"
#include <functional>

namespace Editor::UI {

class ColorButton : public olc::QuickGUI::Button
{
public:
	ColorButton(olc::QuickGUI::Manager& manager,
		olc::Pixel color,
		const std::string& text,
		const olc::vf2d& pos,
		const olc::vf2d& size);

	void SetColor(olc::Pixel color);
	void SetText(const std::string& text);

	void Draw(olc::PixelGameEngine* pge) override;
	void DrawDecal(olc::PixelGameEngine* pge) override;
private:
	olc::Pixel color_;
	olc::Sprite sprite_;
	olc::Sprite spritePressed_;
};

class ButtonStrip {
public:
	using Handler_t = std::function<void(int)>;

	ButtonStrip(olc::PixelGameEngine& pge, const olc::vf2d& pos,
		    const olc::vf2d& size, const Style& style);

	void Update(float fElapsedTime);
	void Draw();
	void SetButtonCount(unsigned int count);
	void SetButtonHandler(Handler_t handler);

private:
	struct Button {
		std::string text;
		olc::vf2d pos;
		olc::vf2d size;
		bool pressed = false;
	};

	olc::PixelGameEngine& engine_;
	olc::vf2d pos_;
	olc::vf2d size_;
	const Style& style_;
	Handler_t buttonHandler_;
	std::vector<Button> buttons_;
};

} //namespace Editor::UI
