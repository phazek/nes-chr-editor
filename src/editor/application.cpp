#include "application.h"

namespace Editor {

Application::Application() {
	sAppName = "NES CHR editor";
}

bool Application::OnUserCreate() {
	guiManager_.colText = style_.textColor;
	guiManager_.colNormal = style_.bgColor;
	guiManager_.colHover = style_.bgColor;
	guiManager_.colClick = style_.highlightColor;
	guiManager_.colDisable = style_.disabledColor;
	guiManager_.colBorder = style_.borderColor;

	return true;
}

bool Application::OnUserUpdate(float fElapsedTime) {
	if (!HandleInput()) {
		return false;
	}

	DrawScene();

	return true;
}

bool Application::HandleInput() {
	if (GetKey(olc::Key::ESCAPE).bReleased) return false; // Exit on ESC

	return true;
}

void Application::DrawScene() {
	Clear(style_.bgColor);
	for (auto& control : controls_) {
		control->Update(this);
		control->Draw(this);

		auto it = controlHandlers_.find(control.get());
		if (it != controlHandlers_.end()) {
			it->second();
		}
	}

}

} // namespace Editor
