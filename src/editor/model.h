#pragma once

#include "olc/olcPixelGameEngine.h"
#include <cstdint>
#include <span>

namespace Editor {

class Model {
public:
	Model();

	bool SetChrData(std::span<uint8_t> data);

	const uint8_t GetPaletteColorId(uint8_t index) const;
	void SetPaletteColorId(uint8_t index, uint8_t id);

	const std::vector<olc::Sprite*>& GetSprites() const;
private:
	struct Tile {
		void ParseData(const std::span<uint8_t>& src);

		std::array<uint8_t, 8 * 8> data;
	};

	std::span<uint8_t> data_;
	std::array<uint8_t, 4> palette_{0x22, 0x16, 0x27, 0x18};
	std::vector<olc::Sprite*> sprites_;
	std::vector<Tile> tiles_;

	void ParseTiles();
	void UpdateSprites();
};

} // namespace Editor
