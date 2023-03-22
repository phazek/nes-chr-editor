#include "uimanager.h"

#include "uielements.h"
#include "nes/palette.h"
#include "tfm/tinyformat.h"

namespace Editor::UI {

void Manager::ControlHandle::SetVisibility(bool visible) {
	for (auto* control : controls) {
		control->bVisible = visible;
	}
}

Manager::Manager(olc::PixelGameEngine& engine, const Style& style)
: engine_(engine) {
	guiManager_.colText = style.textColor;
	guiManager_.colNormal = style.bgColor;
	guiManager_.colHover = style.bgColor;
	guiManager_.colClick = style.highlightColor;
	guiManager_.colDisable = style.disabledColor;
	guiManager_.colBorder = style.borderColor;
}

void Manager::Update() {
	guiManager_.Update(&engine_);
}

void Manager::Draw() {
	for (auto& handler: updateHandlers_) {
		handler.second(handler.first);
	}
	guiManager_.Draw(&engine_);
}

Manager::ControlHandle Manager::AddLabel(const std::string& text, const olc::vf2d& pos, const olc::vf2d& size) {
	Manager::ControlHandle ret;
	ret.controls.push_back(new olc::QuickGUI::Label(guiManager_, text, pos, size));
	return ret;
}

Manager::ControlHandle Manager::AddColorButton(const olc::Pixel& color,
					       const std::string& text,
					       const olc::vf2d& pos,
					       const olc::vf2d& size,
					       UpdateHandler_t handler) {
	Manager::ControlHandle ret;

	auto* button = new ColorButton(guiManager_, color, text, pos, size);
	updateHandlers_[button] = handler;

	ret.controls.push_back(button);
	return ret;
}

Manager::ControlHandle Manager::AddButtonStrip(
    uint8_t buttonCount, const olc::vf2d& pos, const olc::vf2d& size,
    std::function<void(int)> buttonHandler) {
	Manager::ControlHandle ret;

	auto buttonWidth = std::min(static_cast<int>(size.x) / buttonCount, 30);
	auto leftPad = (size.x - buttonWidth * buttonCount) / 2;
	for (int i = 0; i < buttonCount; ++i) {
	    auto* button = new olc::QuickGUI::Button(
			guiManager_, std::to_string(i),
			{pos.x + leftPad + buttonWidth * i, pos.y},
			{static_cast<float>(buttonWidth), 30});

	    updateHandlers_[button] = [buttonHandler, i](olc::QuickGUI::BaseControl* control) {
			if (control->bReleased) {
				buttonHandler(i);
			}
	    };

		ret.controls.push_back(button);
	}

	return ret;
}

Manager::ControlHandle Manager::AddColorSelector(const olc::vf2d& pos, const olc::vf2d& size, std::function<void(int)> buttonHandler) {
	Manager::ControlHandle ret;

	const auto textSize = engine_.GetTextSize("XX");
	auto buttonWidth = std::min(static_cast<int>(size.x) / 4, textSize.x);
	olc::vf2d buttonSize(buttonWidth, buttonWidth);

	for (int i = 0; i < nes::kColorPalette.size(); ++i) {
		olc::vf2d btnPos{pos.x + (i % 6) * (buttonWidth + 5), pos.y + (i / 6) * (buttonWidth + 5)};
		auto* button = new ColorButton(
		    guiManager_, nes::kColorPalette[i], tfm::format("%02X", i), btnPos, buttonSize);

		updateHandlers_[button] = [buttonHandler, i](olc::QuickGUI::BaseControl* control) {
			if (control->bReleased) {
				buttonHandler(i);
			}
		};

		ret.controls.push_back(button);
	}

	return ret;
}

} // namespace Editor::UI
