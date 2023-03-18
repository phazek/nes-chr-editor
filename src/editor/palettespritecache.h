#include "nes/palette.h"
#include "olc/olcPixelGameEngine.h"

namespace Editor {

class PaletteSpriteCache {
public:
	PaletteSpriteCache();

	const olc::Sprite& GetColorSprite(uint8_t index);

private:
	std::vector<std::unique_ptr<olc::Sprite>> spriteCache_;
	void Init();
};

} // namespace Editor
