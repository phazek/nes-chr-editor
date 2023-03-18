#include "palettespritecache.h"
#include "nes/palette.h"
#include <algorithm>

namespace Editor {

PaletteSpriteCache::PaletteSpriteCache() {
	Init();
}

const olc::Sprite& PaletteSpriteCache::GetColorSprite(uint8_t index) {
	assert(index < nes::kColorPalette.size());

	return *spriteCache_[index];
}

void PaletteSpriteCache::Init() {
	spriteCache_.reserve(nes::kColorPalette.size());
	for (auto& color : nes::kColorPalette) {
		auto& sprite = spriteCache_.emplace_back(new olc::Sprite{30, 30});
		memset(sprite->pColData.data(), color.n, sprite->pColData.size());
	}
}

} // namespace Editor
