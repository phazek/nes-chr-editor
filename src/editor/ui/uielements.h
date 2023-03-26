#pragma once

#include "../style.h"
#include "editor/model.h"
#include <functional>

namespace Editor::UI {

using ButtonHandler_t = std::function<void()>;


class Base {
public:
	Base(olc::PixelGameEngine& pge, const olc::vf2d& pos,
		const olc::vf2d& size, const Style& style);

	void SetVisibility(bool visible);

protected:
	olc::PixelGameEngine& engine_;
	olc::vf2d pos_;
	olc::vf2d size_;
	const Style& style_;
	bool visible_ = true;
};

class ColorButton : public Base {
public:
	ColorButton(olc::PixelGameEngine& pge, olc::Pixel color,
		const std::string& text, const olc::vf2d& pos,
		const olc::vf2d& size, const Style& style);

	void SetColor(olc::Pixel color);
	void SetText(const std::string& text);
	void SetSelected(bool selected);
	void SetButtonHandler(ButtonHandler_t handler);

	void Update(float fElapsedTime);
	void Draw();
private:
	olc::Pixel color_{};
	std::string text_{};
	olc::vf2d textSize_{};
	bool selected_ = false;
	bool pressed_ = false;
	ButtonHandler_t buttonHandler_{};
};

class SpriteButton : public Base {
public:
	SpriteButton(olc::PixelGameEngine& pge, olc::Sprite* sprite, const olc::vf2d& pos,
		const olc::vf2d& size, const Style& style);

	void SetSprite(olc::Sprite* sprite);
	void SetSelected(bool selected);
	void SetButtonHandler(ButtonHandler_t handler);

	void Update(float fElapsedTime);
	void Draw(int scale = 1);
private:
	olc::Sprite* sprite_ = nullptr;
	bool selected_ = false;
	bool pressed_ = false;
	ButtonHandler_t buttonHandler_{};
};

class PaletteSelector : public Base {
public:
	using Handler_t = std::function<void(int)>;

	PaletteSelector(olc::PixelGameEngine& pge, const olc::vf2d& pos,
			const olc::vf2d& size, const Style& style);

	void SetColor(uint8_t idx, olc::Pixel color);
	void SetSelected(uint8_t idx);
	uint8_t GetSelected() const;
	void SetButtonHandler(Handler_t handler);
	void Update(float fElapsedTime);
	void Draw();

private:
	std::vector<ColorButton> buttons_;
	Handler_t buttonHandler_{};
	int selectedButtonIdx_ = -1;
};

class ButtonStrip : public Base {
public:
	using Handler_t = std::function<void(int)>;

	ButtonStrip(olc::PixelGameEngine& pge, const olc::vf2d& pos,
		    const olc::vf2d& size, const Style& style);

	void Update(float fElapsedTime);
	void Draw();
	void SetButtonCount(unsigned int count);
	void SetButtonHandler(Handler_t handler);

private:
	struct Button {
		std::string text;
		olc::vf2d pos;
		olc::vf2d size;
		bool pressed = false;
	};

	Handler_t buttonHandler_;
	std::vector<Button> buttons_;
};

class ColorSelector: public Base {
public:
	using Handler_t = std::function<void(int)>;

	ColorSelector(olc::PixelGameEngine& pge, const olc::vf2d& pos,
		    const olc::vf2d& size, const Style& style);

	void Update(float fElapsedTime);
	void Draw();
	void SetButtonHandler(Handler_t handler);

private:
	Handler_t buttonHandler_;
	std::vector<std::unique_ptr<ColorButton>> buttons_;

	void InitializeButtons();
};

class BankDisplay: public Base {
public:
	using Handler_t = std::function<void(int)>;

	BankDisplay(Model& model, olc::PixelGameEngine& pge, const olc::vf2d& pos,
		    const olc::vf2d& size, const Style& style);

	void Update(float fElapsedTime);
	void Draw();
	void SetButtonHandler(Handler_t handler);

private:
	Model& model_;
	Handler_t buttonHandler_;
	std::vector<std::unique_ptr<SpriteButton>> buttons_;
	int selectedButtonIdx_ = -1;
	const int kScale = 3;

	void InitializeButtons();
	void SetSelected(int idx);
};

class SpriteDisplay : public Base {
public:
	using Handler_t = std::function<void(olc::vi2d)>;

	SpriteDisplay(olc::PixelGameEngine& pge, const olc::vf2d& pos,
		    int scale, const Style& style);

	void Update(float fElapsedTime);
	void Draw();
	void SetSprite(olc::Sprite* sprite);
	void SetButtonHandler(Handler_t handler);

private:
	int scale_ = 1;
	olc::Sprite* sprite_ = nullptr;
	Handler_t buttonHandler_{};
};

} //namespace Editor::UI
