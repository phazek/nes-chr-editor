#include "uielements.h"
#include <algorithm>

namespace Editor::UI {

ColorButton::ColorButton(olc::QuickGUI::Manager& manager,
			olc::Pixel color,
			const std::string& text,
			const olc::vf2d& pos,
			const olc::vf2d& size)
: olc::QuickGUI::Button(manager, text, pos, size)
, sprite_(size.x, size.y)
, spritePressed_(size.x, size.y)
{
	SetColor(color);
}

void ColorButton::Draw(olc::PixelGameEngine* pge) {
	if (!bVisible) {
	    return;
	}

	if (bHeld) {
	    pge->DrawSprite(vPos, &spritePressed_);
	} else {
	    pge->DrawSprite(vPos, &sprite_);
	}

	if (!sText.empty()) {
	    olc::vf2d vText = pge->GetTextSizeProp(sText);
	    pge->DrawStringProp(vPos + (vSize - vText) * 0.5f, sText,
				m_manager.colText);
	}
}

void ColorButton::DrawDecal(olc::PixelGameEngine* pge) {
	// TODO
	olc::QuickGUI::Button::DrawDecal(pge);
}

void ColorButton::SetColor(olc::Pixel color) {
	color_ = color;
	std::fill(sprite_.pColData.begin(), sprite_.pColData.end(), color);
	color.r = color.r / 2;
	color.g = color.g / 2;
	color.b = color.b / 2;
	std::fill(spritePressed_.pColData.begin(), spritePressed_.pColData.end(), color);
}

void ColorButton::SetText(const std::string& text) {
	sText = text;
}

// ButtonStrip
ButtonStrip::ButtonStrip(
	olc::PixelGameEngine& pge,
	const olc::vf2d& pos,
	const olc::vf2d& size,
	const Style& style)
: engine_(pge)
, pos_(pos)
, size_(size)
, style_(style) {
	SetButtonCount(3);
}

void ButtonStrip::Update(float fElapsedTime) {
	auto boundsCheck = [](const olc::vi2d& mousePos, const Button& button) {
	    return (button.pos.x < mousePos.x && button.pos.y < mousePos.y) &&
		   (button.pos.x + button.size.x > mousePos.x &&
		    button.pos.y + button.size.y > mousePos.y);
	};

	auto mousePos = engine_.GetMousePos();
	auto mousePressed = engine_.GetMouse(olc::Mouse::LEFT).bPressed;
	auto mouseReleased = engine_.GetMouse(olc::Mouse::LEFT).bReleased;
	for (int i = 0; i < buttons_.size(); ++i) {
	    auto& button = buttons_[i];
	    if (boundsCheck(mousePos, button)) {
			if (button.pressed && mouseReleased) {
				button.pressed = false;
				buttonHandler_(i);
			} else if (mousePressed) {
				button.pressed = true;
			}
	    }
	}
}

void Editor::UI::ButtonStrip::Draw() {
	for (int i = 0; i < buttons_.size(); ++i) {
		auto& button = buttons_[i];
		auto color = button.pressed ? style_.highlightColor: style_.bgColor;
		engine_.FillRect(button.pos, button.size, color);
		engine_.DrawRect(button.pos, button.size, style_.borderColor);
		auto textSize = engine_.GetTextSize(button.text);
		olc::vf2d pos{(button.size.x - textSize.x) / 2, (button.size.y - textSize.y) / 2};
		pos += button.pos;
		engine_.DrawString(pos, button.text, style_.textColor);
	}
}

void Editor::UI::ButtonStrip::SetButtonCount(unsigned int count) {
	buttons_.clear();
	auto btnWidth = size_.x / count;
	for (int i = 0; i < count; ++i) {
		buttons_.push_back({std::to_string(i+1),
			olc::vf2d{pos_.x + i * btnWidth, pos_.y},
			olc::vf2d{btnWidth, size_.y}});
	}
}

void Editor::UI::ButtonStrip::SetButtonHandler(Handler_t handler) {
	buttonHandler_ = std::move(handler);
}

} //namespace Editor::UI
