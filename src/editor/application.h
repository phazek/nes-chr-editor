#include "olc/olcPixelGameEngine.h"
#include "olc/olcPGEX_QuickGUI.h"
#include "ui/uimanager.h"
#include "nes/nesfile.h"

#include "model.h"

namespace Editor {

class Application: public olc::PixelGameEngine {
public:
	Application();

	// olc::PixelGameEngine
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

private:
	olc::QuickGUI::Manager guiManager_;
	Model editorModel_;
	Style style_;
	UI::Manager uiManager_;
	std::unique_ptr<nes::File> nesFile_;
	std::vector<UI::Manager::ControlHandle> paletteButtons_;
	UI::Manager::ControlHandle colorSelectorHandle_;
	int currentlyEditedPaletteEntry_ = -1;
	int selectedSpriteIndex_ = 5;


	bool HandleInput();
	void DrawScene();

	void BuildUI();

	void LoadNesFile(const std::string& path);
};

} // namespace Editor


