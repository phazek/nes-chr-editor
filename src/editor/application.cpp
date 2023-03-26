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

	return true;
}

void Application::DrawScene() {
	Clear(style_.bgColor);

	DrawLine({140, 0}, {140, ScreenHeight()}, style_.borderColor);

	bankSelectorUi_->Draw();
	paletteSelector_->Draw();
}

void Application::BuildUI() {
	// Palette
	paletteSelector_ = new UI::PaletteSelector{*this, {0, 20}, {140, 30}, style_};
	for (int i = 0; i < 4; ++i) {
		paletteSelector_->SetColor(i, IdToColor(editorModel_.GetPaletteColorId(i)));
	}
	paletteSelector_->SetButtonHandler([this](int idx){
		tfm::printf("Palette entry selected: %d\n", idx);
		currentlyEditedPaletteEntry_ = idx;
		// TODO: open color selector for given entry
	});

	// CHR bank selector
	bankSelectorUi_ = new UI::ButtonStrip(*this, {330, 430}, {150, 30}, style_);
	bankSelectorUi_->SetButtonHandler([this](int idx){
		tfm::printf("CHR bank selected: %d\n", idx);
		editorModel_.SetChrData(nesFile_->GetChrBank(idx));
	});
}

void Application::UpdateUI(float fElapsedTime) {
	bankSelectorUi_->Update(fElapsedTime);
	paletteSelector_->Update(fElapsedTime);
}

void Application::LoadNesFile(const std::string& path) {
	nesFile_ = nes::File::LoadFromFile(path);
	bankSelectorUi_->SetButtonCount(nesFile_->GetChrBankCount());
	editorModel_.SetChrData(nesFile_->GetChrBank(0));
}

} // namespace Editor
