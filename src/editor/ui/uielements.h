#include "olc/olcPGEX_QuickGUI.h"

namespace Editor::UI {

	class ColorButton : public olc::QuickGUI::Button
	{
	public:
		ColorButton(olc::QuickGUI::Manager& manager,
			olc::Pixel color,
			const std::string& text,
			const olc::vf2d& pos,
			const olc::vf2d& size);

		void SetColor(olc::Pixel color);
		void SetText(const std::string& text);

		void Draw(olc::PixelGameEngine* pge) override;
		void DrawDecal(olc::PixelGameEngine* pge) override;
	private:
		olc::Pixel color_;
		olc::Sprite sprite_;
		olc::Sprite spritePressed_;

	};

} //namespace Editor::UI
