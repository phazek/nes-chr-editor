#pragma once

#include "olc/olcPixelGameEngine.h"
#include "ui/uielements.h"
#include "nes/nesfile.h"

#include "model.h"

namespace Editor {

class Application: public olc::PixelGameEngine {
public:
	Application();

	void SetRomPath(const std::string& path);
	// olc::PixelGameEngine
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

private:
	Model editorModel_;
	Style style_;
	std::unique_ptr<nes::File> nesFile_;
	int currentlyEditedPaletteEntry_ = -1;
	int selectedSpriteIndex_ = 5;

	UI::ButtonStrip* bankSelectorUi_;
	UI::PaletteSelector* paletteSelector_;
	UI::ColorSelector* colorSelector_;
	UI::BankDisplay* bankDisplay_;
	UI::SpriteDisplay* spriteDisplay_;

	bool HandleInput();
	void DrawScene();

	void BuildUI();
	void UpdateUI(float fElapsedTime);

	void LoadNesFile(const std::string& path);
};

} // namespace Editor


