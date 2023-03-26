#include "model.h"

#include "nes/constants.h"
#include "nes/palette.h"

#include "tfm/tinyformat.h"

namespace Editor {

void Model::Tile::ParseData(const std::span<uint8_t>& src) {
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			bool ll = !!(src[row] & (1 << (7 - col)));
			bool hh = !!(src[8 + row] & (1 << (7 - col)));
			data[row * 8 + col] = (hh ? 2 : 0) | (ll ? 1 : 0);
		}
	}
}

Model::Model() {
	for (int i = 0; i < 256; ++i) {
		sprites_.push_back(new olc::Sprite(8, 8));
		tiles_.emplace_back();
	}
}

bool Model::SetChrData(std::span<uint8_t> data) {
	if (data.size() != nes::kChrBankSize) {
		tfm::printf("ERROR: invalid chr bank size: %s (correct %s)\n",
			data.size(), nes::kChrBankSize);
		return false;
	}

	data_ = std::move(data);
	ParseTiles();
	return true;
}

const uint8_t Model::GetPaletteColorId(uint8_t index) const {
	assert(0 <= index && index < palette_.size());

	return palette_[index];
}

void Model::SetPaletteColorId(uint8_t index, uint8_t id) {
	assert(0 <= index && index < palette_.size());

	palette_[index] = id;
	UpdateSprites();
}

const std::vector<olc::Sprite*>& Model::GetSprites() const {
	return sprites_;
}

void Model::ParseTiles() {
	static const int kRawTileSize = 16;
	for (int i = 0; i < 256; ++i) {
		std::span<uint8_t> input{data_.data() + i * kRawTileSize, kRawTileSize};
		tiles_[i].ParseData(input);
	}

	UpdateSprites();
}

void Model::UpdateSprites() {
	std::array<olc::Pixel, 4> colors = {
		nes::kColorPalette[palette_[0]],
		nes::kColorPalette[palette_[1]],
		nes::kColorPalette[palette_[2]],
		nes::kColorPalette[palette_[3]],
	};

	for (int spriteIdx = 0; spriteIdx < 256; ++spriteIdx) {
		auto* sprite = sprites_[spriteIdx];
		for (int y = 0; y < 8; ++y) {
			for (int x = 0; x < 8; ++x) {
				auto colorId = tiles_[spriteIdx].data[y * 8 + x];
				sprite->SetPixel(x, y, colors[colorId]);
			}
		}
	}
}

} // namespace Editor
