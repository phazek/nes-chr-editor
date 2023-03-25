#include "uielements.h"
#include <algorithm>

namespace {
struct Bounds {
	olc::vf2d min;
	olc::vf2d max;
};

template<typename T>
bool IsInBounds(const olc::v2d_generic<T>& point, const Bounds& bounds) {
	return (bounds.min.x < point.x && bounds.min.y < point.y) &&
	       (bounds.max.x > point.x && bounds.max.y > point.y);
}

} // namespace

namespace Editor::UI {

ColorButton::ColorButton(olc::PixelGameEngine& pge, olc::Pixel color,
	     const std::string& text, const olc::vf2d& pos,
	     const olc::vf2d& size, const Style& style)
: engine_(pge)
, color_(color)
, text_(text)
, style_(style) {}

void ColorButton::Update(float fElapsedTime) {
	auto mousePos = engine_.GetMousePos();
	auto mousePressed = engine_.GetMouse(olc::Mouse::LEFT).bPressed;
	auto mouseReleased = engine_.GetMouse(olc::Mouse::LEFT).bReleased;
	auto bounds = Bounds{{pos_.x, pos_.y}, {pos_.x + size_.x, pos_.y + size_.y}};

	if (IsInBounds(mousePos, bounds)) {
	    if (pressed_ && mouseReleased) {
			pressed_ = false;
			buttonHandler_();
	    } else if (mousePressed) {
			pressed_ = true;
	    }
	}
}

void ColorButton::Draw() {
	engine_.FillRect(pos_, size_, color_);

	if (selected_) {
		engine_.DrawRect(pos_, size_, style_.borderColor);
	}

	olc::vf2d textPos{pos_.x + (size_.x - textSize_.x) / 2,
					  pos_.y + (size_.y - textSize_.y) / 2};

	engine_.DrawString(textPos, text_, style_.textColor);
}

void ColorButton::SetColor(olc::Pixel color) {
	color_ = color;
}

void ColorButton::SetText(const std::string& text) {
	text_ = text;
	textSize_ = engine_.GetTextSize(text_);
}

void ColorButton::SetSelected(bool selected) {
	selected_ = selected;
}

void ColorButton::SetButtonHandler(ButtonHandler_t handler) {
	buttonHandler_ = std::move(handler);
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
	auto mousePos = engine_.GetMousePos();
	auto mousePressed = engine_.GetMouse(olc::Mouse::LEFT).bPressed;
	auto mouseReleased = engine_.GetMouse(olc::Mouse::LEFT).bReleased;
	for (int i = 0; i < buttons_.size(); ++i) {
	    auto& button = buttons_[i];
		auto bounds = Bounds{{button.pos.x, button.pos.y}, {button.pos.x + button.size.x, button.pos.y + button.size.y}};
	    if (IsInBounds(mousePos, bounds)) {
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
