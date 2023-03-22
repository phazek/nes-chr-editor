#include "application.h"
#include "nes/palette.h"
#include "tfm/tinyformat.h"

namespace {
const olc::vf2d kPaletteButtonSize = {30.f, 30.f};

olc::Pixel IdToColor(uint8_t id) {
	assert(id < nes::kColorPalette.size());
	return nes::kColorPalette[id];
}

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

	DrawLine({140, 0}, {140, ScreenHeight()}, style_.borderColor);

	DrawSprite(150, 10, &(editorModel_.GetSpriteAtlas()), 2);

}

void Application::BuildUI() {
	// Palette
	uiManager_.AddLabel("Palette", {}, {135, 20});
	uiManager_.AddColorButton(IdToColor(editorModel_.GetPaletteColorId(0)), "0", {0, 20}, {30, 30}, {});
	uiManager_.AddColorButton(IdToColor(editorModel_.GetPaletteColorId(1)), "1", {35, 20}, {30, 30}, {});
	uiManager_.AddColorButton(IdToColor(editorModel_.GetPaletteColorId(2)), "2", {70, 20}, {30, 30}, {});
	uiManager_.AddColorButton(IdToColor(editorModel_.GetPaletteColorId(3)), "3", {105, 20}, {30, 30}, {});

	// CHR bank selector
	uiManager_.AddButtonStrip(4, {330, 330}, {150, 30},
		[this](int idx) {
			editorModel_.SetChrData(nesFile_->GetChrBank(idx));
	});
}

void Application::LoadNesFile(const std::string& path) {
	nesFile_ = nes::File::LoadFromFile(path);
	editorModel_.SetChrData(nesFile_->GetChrBank(0));
}

} // namespace Editor
