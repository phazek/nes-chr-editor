#include "application.h"

namespace Editor {

Application::Application() {
	sAppName = "NES CHR editor";
}

bool Application::OnUserCreate() {

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
	}

}

} // namespace Editor
