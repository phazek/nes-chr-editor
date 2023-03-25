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

	UpdateUI(fElapsedTime);
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

	bankSelectorUi_->Draw();
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

	bankSelectorUi_ = new UI::ButtonStrip(*this, {330, 330}, {150, 30}, style_);
	bankSelectorUi_->SetButtonHandler([this](int idx){
		tfm::printf("CHR bank selected: %d\n", idx);
		editorModel_.SetChrData(nesFile_->GetChrBank(idx));
	});
}

void Application::UpdateUI(float fElapsedTime) {
	bankSelectorUi_->Update(fElapsedTime);
}

void Application::LoadNesFile(const std::string& path) {
	nesFile_ = nes::File::LoadFromFile(path);
	bankSelectorUi_->SetButtonCount(nesFile_->GetChrBankCount());
	editorModel_.SetChrData(nesFile_->GetChrBank(0));
}

} // namespace Editor
