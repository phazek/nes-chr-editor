#include "model.h"

#include "nes/constants.h"
#include "nes/palette.h"

#include "tfm/tinyformat.h"

namespace {
constexpr int kRawTileSize = 16;
} // namespace

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

std::array<uint8_t, 16> Model::Tile::ToRawData() const {
	std::array<uint8_t, 16> res;
	res.fill(0);

	for (int i = 0; i < data.size(); ++i) {
		int row = i / 8;
		int col = i % 8;
		uint8_t px = data[i];

		res[row]     |=  (px & 0b01)       << (7-col);
		res[8 + row] |= ((px & 0b10) >> 1) << (7-col);
	}

	return res;
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
	// Clear undo stack
	undoStack_ = {};

	return true;
}

const uint8_t Model::GetPaletteColorId(uint8_t index) const {
	assert(0 <= index && index < palette_.size());

	return palette_[index];
}

void Model::SetPaletteColorId(uint8_t index, uint8_t id) {
	assert(0 <= index && index < palette_.size());

	palette_[index] = id;
	UpdateAllSprites();
}

const std::vector<olc::Sprite*>& Model::GetSprites() const {
	return sprites_;
}

void Model::SetTilePixel(uint8_t tileIdx, olc::vi2d coord, uint8_t colorId) {
	auto& tile = tiles_[tileIdx];
	auto& pixel = tile.data[coord.y * 8 + coord.x];
	if (pixel == colorId) {
		return;
	}

	undoStack_.push({tileIdx, coord, pixel});

	pixel = colorId;
	tile.dirty = true;
	UpdateSprite(tileIdx);
}

void Model::Save() {
	for (int i = 0; i < tiles_.size(); ++i) {
		auto& tile = tiles_[i];
		if (!tile.dirty) {
			continue;
		}

		auto raw = tile.ToRawData();
		int startAddress = i * kRawTileSize;
		for (int j = 0; j < raw.size(); ++j) {
			data_[startAddress + j] = raw[j];
		}
	}
}

void Model::Undo() {
	if (!undoStack_.empty()) {
		auto undoItem = undoStack_.top();
		undoStack_.pop();
		auto idx = undoItem.coord.y * 8 + undoItem.coord.x;
		tiles_[undoItem.tileIdx_].data[idx] = undoItem.colorId;
		UpdateSprite(undoItem.tileIdx_);
	}
}

void Model::ParseTiles() {
	for (int i = 0; i < 256; ++i) {
		std::span<uint8_t> input{data_.data() + i * kRawTileSize, kRawTileSize};
		tiles_[i].ParseData(input);
	}

	UpdateAllSprites();
}

void Model::UpdateSprite(int idx) {
	std::array<olc::Pixel, 4> colors = {
		nes::kColorPalette[palette_[0]],
		nes::kColorPalette[palette_[1]],
		nes::kColorPalette[palette_[2]],
		nes::kColorPalette[palette_[3]],
	};
	auto* sprite = sprites_[idx];
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			auto colorId = tiles_[idx].data[y * 8 + x];
			sprite->SetPixel(x, y, colors[colorId]);
		}
	}
}

void Model::UpdateAllSprites() {
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
