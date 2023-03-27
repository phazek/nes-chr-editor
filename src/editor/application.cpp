#include "application.h"
#include "nes/palette.h"
#include "tfm/tinyformat.h"
#include "ui/uielements.h"

namespace {
const olc::vf2d kBankSelectorPos{275.f, 420.f};
const olc::vf2d kPaletteSelectorPos{0.f, 5.f};
const olc::vf2d kColorSelectorPos{0.f, 40.f};
const olc::vf2d kBankDisplayPos{150.f, 5.f};
const olc::vf2d kSpriteDisplayPos{580.f, 5.f};

const olc::vf2d kPaletteButtonSize{30.f, 30.f};

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

void Application::SetRomPath(const std::string& path) {
	LoadNesFile(path);
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

	if (GetKey(olc::Key::BACK).bReleased) editorModel_.Undo();
	if (GetKey(olc::Key::S).bReleased) {
		editorModel_.Save();
		nesFile_->WriteToFile("copy.nes");
		tfm::printf("Saved successfully\n");
	}

	return true;
}

void Application::DrawScene() {
	Clear(style_.bgColor);

	DrawLine({140, 0}, {140, ScreenHeight()}, style_.borderColor);

	bankSelectorUi_->Draw();
	paletteSelector_->Draw();
	colorSelector_->Draw();
	bankDisplay_->Draw();
	spriteDisplay_->Draw();
}

void Application::BuildUI() {
	// Palette
	paletteSelector_ = new UI::PaletteSelector{*this, kPaletteSelectorPos, {140, 30}, style_};
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
	colorSelector_ = new UI::ColorSelector{*this, kColorSelectorPos, {140.f, 310}, style_};
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
	bankSelectorUi_ = new UI::ButtonStrip(*this, kBankSelectorPos, {150, 30}, style_);
	bankSelectorUi_->SetButtonHandler([this](int idx){
		editorModel_.SetChrData(nesFile_->GetChrBank(idx));
	});

	// CHR bank display
	bankDisplay_ = new UI::BankDisplay{editorModel_, *this, kBankDisplayPos, {400, 400}, style_};
	bankDisplay_->SetButtonHandler([this](int idx){
		selectedSpriteIndex_ = idx;
		spriteDisplay_->SetSprite(editorModel_.GetSprites()[selectedSpriteIndex_]);
		spriteDisplay_->SetVisibility(true);
	});

	// Editor panel
	spriteDisplay_ = new UI::SpriteDisplay{*this, kSpriteDisplayPos, 32, style_};
	spriteDisplay_->SetVisibility(false);
	spriteDisplay_->SetButtonHandler([this](olc::vi2d coord) {
		auto selectedColor = paletteSelector_->GetSelected();
		assert(selectedColor >= 0);
		editorModel_.SetTilePixel(selectedSpriteIndex_, coord, selectedColor);
	});
}

void Application::UpdateUI(float fElapsedTime) {
	bankSelectorUi_->Update(fElapsedTime);
	paletteSelector_->Update(fElapsedTime);
	colorSelector_->Update(fElapsedTime);
	bankDisplay_->Update(fElapsedTime);
	spriteDisplay_->Update(fElapsedTime);
}

void Application::LoadNesFile(const std::string& path) {
	nesFile_ = nes::File::LoadFromFile(path);
	bankSelectorUi_->SetButtonCount(nesFile_->GetTileMapCount());
	editorModel_.SetChrData(nesFile_->GetChrBank(0));
}

} // namespace Editor
