#include "uielements.h"
#include "nes/palette.h"

#include "tfm/tinyformat.h"

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

// Base
Base::Base(olc::PixelGameEngine &pge, const olc::vf2d &pos,
	const olc::vf2d &size, const Style &style)
: engine_(pge), pos_(pos), size_(size), style_(style) {}


void Base::SetVisibility(bool visible) {
	visible_ = visible;
}

// ColorButton
ColorButton::ColorButton(olc::PixelGameEngine& pge, olc::Pixel color,
	     const std::string& text, const olc::vf2d& pos,
	     const olc::vf2d& size, const Style& style)
: Base(pge, pos, size, style)
, color_(color)
{
	SetText(text);
}

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
	if (pressed_) {
		engine_.FillRect(pos_, size_, style_.highlightColor);
	} else {
		engine_.FillRect(pos_, size_, color_);
	}

	if (selected_) {
		auto pos = pos_;
		auto size = size_;
		engine_.DrawRect(pos, size, style_.borderColor);
		pos += olc::vf2d{1.f, 1.f};
		size -= olc::vf2d{2.f, 2.f};
		engine_.DrawRect(pos, size, style_.bgColor);
		pos += olc::vf2d{1.f, 1.f};
		size -= olc::vf2d{2.f, 2.f};
		engine_.DrawRect(pos, size, style_.borderColor);
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

// SpriteButton
SpriteButton::SpriteButton(
	olc::PixelGameEngine& pge,
	olc::Sprite* sprite,
	const olc::vf2d& pos,
	const olc::vf2d& size,
	const Style& style)
: Base(pge, pos, size, style)
, sprite_(sprite)
{
}

void SpriteButton::Update(float fElapsedTime) {
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

void SpriteButton::Draw(int scale) {
	if (selected_) {
		engine_.FillRect(pos_ - olc::vf2d{2.f, 2.f},
						 sprite_->Size() * scale + olc::vf2d{4.f, 4.f},
						 style_.borderColor);
	}
	engine_.DrawSprite(pos_, sprite_, scale);
}

void SpriteButton::SetSelected(bool selected) {
	selected_ = selected;
}

void SpriteButton::SetButtonHandler(ButtonHandler_t handler) {
	buttonHandler_ = std::move(handler);
}

// PaletteSelector
PaletteSelector::PaletteSelector(
	olc::PixelGameEngine& pge, const olc::vf2d& pos,
	const olc::vf2d& size, const Style& style)
: Base(pge, pos, size, style)
{
	const auto gridSize = size_.x / 4;
	auto buttonWidth = gridSize - 2;
	const olc::vf2d buttonSize = olc::vf2d{buttonWidth, size_.y};

	for (int i = 0; i < 4; ++i) {
		auto position = pos_ + olc::vf2d{i * gridSize, 0};
		buttons_.emplace_back(engine_, style_.bgColor, std::to_string(i + 1),
			position, buttonSize, style_);
	}
}

void PaletteSelector::SetColor(uint8_t idx, olc::Pixel color) {
	buttons_[idx].SetColor(color);
}

void PaletteSelector::SetSelected(uint8_t idx) {
	if (selectedButtonIdx_ != -1) {
		buttons_[selectedButtonIdx_].SetSelected(false);
	}

	selectedButtonIdx_ = idx;
	buttons_[selectedButtonIdx_].SetSelected(true);
}

uint8_t PaletteSelector::GetSelected() const {
	return selectedButtonIdx_;
}

void PaletteSelector::SetButtonHandler(Handler_t handler) {
	buttonHandler_ = std::move(handler);
	for (int i = 0; i < 4; ++i) {
		auto& button = buttons_[i];
		button.SetButtonHandler([i, this](){
			buttonHandler_(i);
		});
	}
}

void PaletteSelector::Update(float fElapsedTime) {
	for (auto& button : buttons_) {
		button.Update(fElapsedTime);
	}
}

void PaletteSelector::Draw() {
	if (!visible_) {
		return;
	}

	for (auto& button : buttons_) {
		button.Draw();
	}
}

// ButtonStrip
ButtonStrip::ButtonStrip(
	olc::PixelGameEngine& pge,
	const olc::vf2d& pos,
	const olc::vf2d& size,
	const Style& style)
: Base(pge, pos, size, style)
{
	SetButtonCount(1);
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

void ButtonStrip::Draw() {
	if (!visible_) {
		return;
	}

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

void ButtonStrip::SetButtonCount(unsigned int count) {
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

// ColorSelector
ColorSelector::ColorSelector(
	olc::PixelGameEngine& pge,
	const olc::vf2d& pos,
	const olc::vf2d& size,
	const Style& style)
: Base(pge, pos, size, style)
{
	InitializeButtons();
}

void ColorSelector::Update(float fElapsedTime) {
	for (auto& button : buttons_) {
		button->Update(fElapsedTime);
	}
}

void ColorSelector::Draw() {
	if (!visible_) {
		return;
	}
	for (auto& button : buttons_) {
		button->Draw();
	}
}

void ColorSelector::SetButtonHandler(Handler_t handler) {
	buttonHandler_ = std::move(handler);
}

void ColorSelector::InitializeButtons() {
	const int kButtonPerRow = 5;
	olc::vf2d gridSize = {size_.x / kButtonPerRow, size_.x / kButtonPerRow};
	olc::vf2d buttonSize = gridSize - olc::vf2d{1.f, 1.f};

	for (int i = 0; i < nes::kColorPalette.size(); ++i) {
		olc::vf2d pos = pos_ + gridSize * olc::vi2d{i % kButtonPerRow, i / kButtonPerRow};
		auto button = std::make_unique<ColorButton>(
			engine_,
			nes::kColorPalette[i],
			tfm::format("%02X", i),
			pos,
			buttonSize,
			style_);
		button->SetButtonHandler([i, this](){
			buttonHandler_(i);
		});
		buttons_.push_back(std::move(button));
	}
}

// BankDisplay
BankDisplay::BankDisplay(
	Model& model,
	olc::PixelGameEngine& pge,
	const olc::vf2d& pos,
	const olc::vf2d& size,
	const Style& style)
: Base(pge, pos, size, style)
, model_(model)
{
	InitializeButtons();
}

void BankDisplay::Update(float fElapsedTime) {
	for (auto& button : buttons_) {
		button->Update(fElapsedTime);
	}
}

void BankDisplay::Draw() {
	if (!visible_) {
		return;
	}
	for (auto& button : buttons_) {
		button->Draw(kScale);
	}
}

void BankDisplay::SetButtonHandler(Handler_t handler) {
	buttonHandler_ = std::move(handler);
}

void BankDisplay::InitializeButtons() {
	const int kButtonPerRow = 16;
	const olc::vf2d kPadding{2.f, 2.f};
	olc::vf2d gridSize = olc::vf2d{kScale * 8.f, kScale * 8.f} + kPadding;
	olc::vf2d buttonSize = gridSize - kPadding;
	auto& sprites = model_.GetSprites();

	for (int i = 0; i < sprites.size(); ++i) {
		olc::vf2d pos = pos_ + gridSize * olc::vi2d{i % kButtonPerRow, i / kButtonPerRow};
		auto button = std::make_unique<SpriteButton>(
			engine_,
			sprites[i],
			pos,
			buttonSize,
			style_);
		button->SetButtonHandler([i, this](){
			buttonHandler_(i);
			SetSelected(i);
		});
		buttons_.push_back(std::move(button));
	}
}

void BankDisplay::SetSelected(int idx) {
	if (selectedButtonIdx_ != -1) {
		buttons_[selectedButtonIdx_]->SetSelected(false);
	}

	selectedButtonIdx_ = idx;
	buttons_[selectedButtonIdx_]->SetSelected(true);
}

// SpriteDisplay
SpriteDisplay::SpriteDisplay(olc::PixelGameEngine& pge, const olc::vf2d& pos,
		int scale, const Style& style)
: Base(pge, pos, olc::vf2d{8, 8} * scale, style)
, scale_(scale) {}

void SpriteDisplay::Update(float fElapsedTime) {
	auto mousePos = engine_.GetMousePos();
	auto bounds = Bounds{{pos_.x, pos_.y}, {pos_.x + size_.x, pos_.y + size_.y}};
	if (!IsInBounds(mousePos, bounds)) {
		return;
	}

	const auto mousePressed = engine_.GetMouse(olc::Mouse::LEFT).bPressed;
	const auto mouseHeld = engine_.GetMouse(olc::Mouse::LEFT).bHeld;
	const auto mouseActive = mousePressed || mouseHeld;

	if (mouseActive) {
		auto coord = mousePos - pos_;
		coord /= scale_;
		buttonHandler_(coord);
	}
}

void SpriteDisplay::Draw() {
	if (!visible_) {
		return;
	}

	engine_.DrawSprite(pos_, sprite_, scale_);
}

void SpriteDisplay::SetSprite(olc::Sprite* sprite) {
	sprite_ = sprite;
}

void SpriteDisplay::SetButtonHandler(Handler_t handler) {
	buttonHandler_ = std::move(handler);
}

} //namespace Editor::UI
