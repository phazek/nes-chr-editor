#include "uimanager.h"

#include "uielements.h"

namespace Editor::UI {

Manager::Manager(olc::PixelGameEngine& engine, const Style& style)
: engine_(engine) {
	guiManager_.colText = style.textColor;
	guiManager_.colNormal = style.bgColor;
	guiManager_.colHover = style.bgColor;
	guiManager_.colClick = style.highlightColor;
	guiManager_.colDisable = style.disabledColor;
	guiManager_.colBorder = style.borderColor;
}

void Manager::Update() {
	guiManager_.Update(&engine_);
}

void Manager::Draw() {
	guiManager_.Draw(&engine_);
	for (auto& handler: updateHandlers_) {
	    if (handler.first->bReleased) {
			handler.second(handler.first);
	    }
	}
}

void Manager::AddLabel(const std::string& text, const olc::vf2d& pos, const olc::vf2d& size) {
	new olc::QuickGUI::Label(guiManager_, text, pos, size);
}

void Manager::AddColorButton(const olc::Pixel& color, const std::string& text, const olc::vf2d& pos,
	const olc::vf2d& size, UpdateHandler_t handler) {
	auto* button = new ColorButton(guiManager_, color, text, pos, size);
	updateHandlers_[button] = handler;
}

void Manager::AddButtonStrip(uint8_t buttonCount, const olc::vf2d& pos, const olc::vf2d& size, std::function<void(int)> buttonHandler) {
	auto buttonWidth = std::min(static_cast<int>(size.x) / buttonCount, 30);
	auto leftPad = (size.x - buttonWidth * buttonCount) / 2;
	for (int i = 0; i < buttonCount; ++i) {
		auto* button = new olc::QuickGUI::Button(guiManager_, std::to_string(i), {pos.x + leftPad + buttonWidth * i, pos.y}, {static_cast<float>(buttonWidth), 30});
		updateHandlers_[button] = [buttonHandler, i](olc::QuickGUI::BaseControl*) {
			buttonHandler(i);
		};
	}
}

} // namespace Editor::UI
