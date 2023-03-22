#include "model.h"

#include "nes/constants.h"
#include "nes/palette.h"

#include "tfm/tinyformat.h"

namespace {
struct Tile {
	std::array<uint8_t, 8 * 8> data;

	void FromData(const std::span<uint8_t>& src) {
		for (int row = 0; row < 8; ++row) {
			for (int col = 0; col < 8; ++col) {
				bool ll = !!(src[row] & (1 << (7 - col)));
				bool hh = !!(src[8 + row] & (1 << (7 - col)));
				data[row * 8 + col] = (hh ? 2 : 0) | (ll ? 1 : 0);
			}
		}
	}
};
} // namespace

namespace Editor {

bool Model::SetChrData(std::span<uint8_t> data) {
	if (data.size() != nes::kChrBankSize) {
		tfm::printf("ERROR: invalid chr bank size: %s (correct %s)\n",
			data.size(), nes::kChrBankSize);
		return false;
	}

	data_ = std::move(data);
	UpdateSprite();
	return true;
}

const uint8_t Model::GetPaletteColorId(uint8_t index) const {
	assert(0 <= index && index < palette_.size());

	return palette_[index];
}

void Model::SetPaletteColorId(uint8_t index, uint8_t id) {
	assert(0 <= index && index < palette_.size());

	palette_[index] = id;
	UpdateSprite();
}

olc::Sprite& Model::GetSpriteAtlas() {
	return spriteAtlas_;
}

void Model::UpdateSprite() {
	static Tile t;
	std::array<olc::Pixel, 4> colors = {
		nes::kColorPalette[palette_[0]],
		nes::kColorPalette[palette_[1]],
		nes::kColorPalette[palette_[2]],
		nes::kColorPalette[palette_[3]],
	};

	for (int spriteIdx = 0; spriteIdx < 256; ++spriteIdx) {
		std::span<uint8_t> input{data_.data() + spriteIdx * 16, 16};
		olc::vi2d topLeft = {(spriteIdx % 16) * 8, spriteIdx / 16 * 8};
		t.FromData(input);
		for (int y = 0; y < 8; ++y) {
			for (int x = 0; x < 8; ++x) {
				auto colorId = t.data[y * 8 + x];
				spriteAtlas_.SetPixel(topLeft.x + x, topLeft.y + y, colors[colorId]);
			}
		}
	}
}

} // namespace Editor
