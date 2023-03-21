#include "uimanager.h"
#include "olc/olcPGEX_QuickGUI.h"

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
}

void Manager::AddLabel(const std::string& text, const olc::vf2d& pos, const olc::vf2d& size) {
	new olc::QuickGUI::Label(guiManager_, text, pos, size);
}

} // namespace Editor::UI