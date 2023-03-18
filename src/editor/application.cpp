#include "application.h"
#include "tfm/tinyformat.h"

namespace {
const olc::vf2d kPaletteButtonSize = {30.f, 30.f};
} // namespace

namespace Editor {

Application::Application()
: uiManager_(*this, Style{}) {
	sAppName = "NES CHR editor";

	BuildUI();
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
	uiManager_.Update();
	uiManager_.Draw();

}

void Application::BuildUI() {
	uiManager_.AddLabel("Palette", {}, {50, 20});

}

} // namespace Editor
