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

} // namespace Editor::UI
