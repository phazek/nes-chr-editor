#include "olc/olcPixelGameEngine.h"
#include "olc/olcPGEX_QuickGUI.h"
#include "ui/uimanager.h"

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

	bool HandleInput();
	void DrawScene();

	void BuildUI();

};

} // namespace Editor


