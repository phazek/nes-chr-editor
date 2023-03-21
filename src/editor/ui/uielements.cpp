#include "uielements.h"
#include <algorithm>

namespace Editor::UI {

ColorButton::ColorButton(olc::QuickGUI::Manager& manager,
			olc::Pixel color,
			const std::string& text,
			const olc::vf2d& pos,
			const olc::vf2d& size)
: olc::QuickGUI::Button(manager, text, pos, size)
, sprite_(size.x, size.y)
, spritePressed_(size.x, size.y)
{
	SetColor(color);
}

void ColorButton::Draw(olc::PixelGameEngine* pge) {
	if (!bVisible) {
	    return;
	}

	if (bHeld) {
	    pge->DrawSprite(vPos, &spritePressed_);
	} else {
	    pge->DrawSprite(vPos, &sprite_);
	}

	if (!sText.empty()) {
	    olc::vf2d vText = pge->GetTextSizeProp(sText);
	    pge->DrawStringProp(vPos + (vSize - vText) * 0.5f, sText,
				m_manager.colText);
	}
}

void ColorButton::DrawDecal(olc::PixelGameEngine* pge) {
	// TODO
	olc::QuickGUI::Button::DrawDecal(pge);
}

void ColorButton::SetColor(olc::Pixel color) {
	color_ = color;
	std::fill(sprite_.pColData.begin(), sprite_.pColData.end(), color);
	color.r = color.r / 2;
	color.g = color.g / 2;
	color.b = color.b / 2;
	std::fill(spritePressed_.pColData.begin(), spritePressed_.pColData.end(), color);
}

void ColorButton::SetText(const std::string& text) {
	sText = text;
}

} //namespace Editor::UI
