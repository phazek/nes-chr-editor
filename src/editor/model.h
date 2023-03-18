#include "olc/olcPixelGameEngine.h"
#include <cstdint>
#include <span>

namespace Editor {

class Model {
public:
	bool SetChrData(std::span<uint8_t> data);

	const uint8_t GetPaletteColorId(uint8_t index) const;
	void SetPaletteColorId(uint8_t index, uint8_t id);
private:
	std::span<uint8_t> data_;
	std::array<uint8_t, 4> palette_;
};

} // namespace Editor
