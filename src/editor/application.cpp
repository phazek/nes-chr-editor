#include "application.h"
#include "nes/palette.h"
#include "tfm/tinyformat.h"
#include "ui/uielements.h"

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

	DrawChrSprites({150, 10}, editorModel_.GetSprites());
}

void Application::BuildUI() {
	// Palette
	uiManager_.AddLabel("Palette", {}, {135, 20});
	for (int i = 0; i < 4; ++i) {
		auto button = uiManager_.AddColorButton(
		    IdToColor(editorModel_.GetPaletteColorId(i)),
		    std::to_string(i),
			{(float)(i * 35), 20},
			{30, 30},
			[this, i](olc::QuickGUI::BaseControl* control) {
				if (control->bReleased) {
					currentlyEditedPaletteEntry_ = i;
					colorSelectorHandle_.SetVisibility(true);
				}
			}
		);
		paletteButtons_.push_back(std::move(button));
	}

	colorSelectorHandle_ = uiManager_.AddColorSelector({10, 60}, {130, 580}, [this](int idx) {
		editorModel_.SetPaletteColorId(currentlyEditedPaletteEntry_, idx);
		colorSelectorHandle_.SetVisibility(false);
		auto* control = paletteButtons_[currentlyEditedPaletteEntry_].controls.front();
		auto* btn = static_cast<UI::ColorButton*>(control);
		btn->SetColor(nes::kColorPalette[idx]);
	});
	colorSelectorHandle_.SetVisibility(false);

	// CHR bank selector
	uiManager_.AddButtonStrip(4, {330, 330}, {150, 30},
		[this](int idx) {
			editorModel_.SetChrData(nesFile_->GetChrBank(idx));
			selectedSpriteIndex_ = -1;
	});
}

void Application::DrawChrSprites(olc::vf2d pos, const std::vector<olc::Sprite*>& sprites) {
	const int kSpacing = 2;
	for (int y = 0; y < 16; ++y) {
	for (int x = 0; x < 16; ++x) {
			auto xPos = pos.x + x * (2 * 8 + kSpacing);
			auto yPos = pos.y + y * (2 * 8 + kSpacing);
			auto idx = y * 16 + x;
			if (idx == selectedSpriteIndex_) {
			    FillRect(xPos - kSpacing, yPos - kSpacing,
				     16 + 2 * kSpacing, 16 + 2 * kSpacing,
				     style_.borderColor);
			}
			DrawSprite(xPos, yPos, sprites[idx],2);
		}}
}

void Application::LoadNesFile(const std::string& path) {
	nesFile_ = nes::File::LoadFromFile(path);
	editorModel_.SetChrData(nesFile_->GetChrBank(0));
}

} // namespace Editor
