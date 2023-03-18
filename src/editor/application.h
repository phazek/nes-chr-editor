#include "olc/olcPixelGameEngine.h"
#include "olc/olcPGEX_QuickGUI.h"

#include "model.h"
#include "style.h"

#include <unordered_map>

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
	std::vector<std::unique_ptr<olc::QuickGUI::BaseControl>> controls_;
	std::unordered_map<olc::QuickGUI::BaseControl*, std::function<void()>> controlHandlers_;
	Style style_;

	bool HandleInput();
	void DrawScene();
	void BuildUI();

};

} // namespace Editor


