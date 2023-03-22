#include "olc/olcPGEX_QuickGUI.h"
#include "../palettespritecache.h"
#include "../style.h"

#include <unordered_map>

namespace Editor::UI {

class Manager {
public:
	using UpdateHandler_t = std::function<void(olc::QuickGUI::BaseControl*)>;

	Manager(olc::PixelGameEngine& engine, const Style& style);
	void Update();
	void Draw();

	void AddLabel(const std::string& text, const olc::vf2d& pos, const olc::vf2d& size);
	void AddColorButton(const olc::Pixel& color, const std::string& text, const olc::vf2d& pos,
		const olc::vf2d& size, UpdateHandler_t handler);
	void AddButtonStrip(uint8_t buttonCount, const olc::vf2d& pos, const olc::vf2d& size, std::function<void(int)> buttonHandler);

private:
	olc::PixelGameEngine& engine_;
	olc::QuickGUI::Manager guiManager_;
	PaletteSpriteCache spriteCache_;
	std::unordered_map<olc::QuickGUI::BaseControl*, UpdateHandler_t> updateHandlers_;
};

} // namespace Editor::UI
