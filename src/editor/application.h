#include "olc/olcPixelGameEngine.h"
#include "olc/olcPGEX_QuickGUI.h"

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
	bool HandleInput();
	void DrawScene();

	Style style_;
	olc::QuickGUI::Manager guiManager_;
	std::vector<std::unique_ptr<olc::QuickGUI::BaseControl>> controls_;
	std::unordered_map<olc::QuickGUI::BaseControl*, std::function<void()>> controlHandlers_;
};

} // namespace Editor


