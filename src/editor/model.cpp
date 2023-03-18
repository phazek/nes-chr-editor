#include "model.h"

#include "nes/constants.h"

#include "tfm/tinyformat.h"

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

const olc::Sprite& Model::GetSpriteAtlas() const {
	return spriteAtlas_;
}

void Model::UpdateSprite() {
	// TODO
}

} // namespace Editor
