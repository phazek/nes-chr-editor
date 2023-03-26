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

Application::Application() {
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
	if (GetKey(olc::Key::K1).bReleased) paletteSelector_->SetSelected(0);
	if (GetKey(olc::Key::K2).bReleased) paletteSelector_->SetSelected(1);
	if (GetKey(olc::Key::K3).bReleased) paletteSelector_->SetSelected(2);
	if (GetKey(olc::Key::K4).bReleased) paletteSelector_->SetSelected(3);

	return true;
}

void Application::DrawScene() {
	Clear(style_.bgColor);

	DrawLine({140, 0}, {140, ScreenHeight()}, style_.borderColor);

	bankSelectorUi_->Draw();
	paletteSelector_->Draw();
	colorSelector_->Draw();
	bankDisplay_->Draw();
}

void Application::BuildUI() {
	// Palette
	paletteSelector_ = new UI::PaletteSelector{*this, {0, 20}, {140, 30}, style_};
	for (int i = 0; i < 4; ++i) {
		paletteSelector_->SetColor(i, IdToColor(editorModel_.GetPaletteColorId(i)));
	}

	paletteSelector_->SetButtonHandler([this](int idx) {
		if (currentlyEditedPaletteEntry_ == idx) {
			currentlyEditedPaletteEntry_ = -1;
			colorSelector_->SetVisibility(false);
		} else {
			currentlyEditedPaletteEntry_ = idx;
			colorSelector_->SetVisibility(true);
		}
	});

	// Color chooser
	colorSelector_ = new UI::ColorSelector{*this, {0.f, 60.f}, {140.f, 310}, style_};
	colorSelector_->SetButtonHandler([this](int idx) {
		editorModel_.SetPaletteColorId(currentlyEditedPaletteEntry_, idx);
		const auto &color = IdToColor(editorModel_.GetPaletteColorId(
			currentlyEditedPaletteEntry_));
		paletteSelector_->SetColor(currentlyEditedPaletteEntry_, color);

		currentlyEditedPaletteEntry_ = -1;
		colorSelector_->SetVisibility(false);
	});
	colorSelector_->SetVisibility(false);

	// CHR bank selector
	bankSelectorUi_ = new UI::ButtonStrip(*this, {275, 430}, {150, 30}, style_);
	bankSelectorUi_->SetButtonHandler([this](int idx){
		tfm::printf("CHR bank selected: %d\n", idx);
		editorModel_.SetChrData(nesFile_->GetChrBank(idx));
	});

	// CHR bank display
	bankDisplay_ = new UI::BankDisplay{editorModel_, *this, {150.f, 10}, {400, 400}, style_};
	bankDisplay_->SetButtonHandler([](int idx){
		tfm::printf("Tile %d selected\n", idx);
	});
}

void Application::UpdateUI(float fElapsedTime) {
	bankSelectorUi_->Update(fElapsedTime);
	paletteSelector_->Update(fElapsedTime);
	colorSelector_->Update(fElapsedTime);
	bankDisplay_->Update(fElapsedTime);
}

void Application::LoadNesFile(const std::string& path) {
	nesFile_ = nes::File::LoadFromFile(path);
	bankSelectorUi_->SetButtonCount(nesFile_->GetChrBankCount());
	editorModel_.SetChrData(nesFile_->GetChrBank(0));
}

} // namespace Editor
