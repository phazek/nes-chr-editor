#pragma once

#include "olc/olcPixelGameEngine.h"

namespace Editor {

struct Style {
	olc::Pixel bgColor{30, 30, 46};
	olc::Pixel highlightColor{89, 91, 112};
	olc::Pixel textColor{205, 214, 244};
	olc::Pixel subtextColor{185, 194, 222};
	olc::Pixel borderColor{180, 190, 254};
	olc::Pixel disabledColor{89, 91, 112};
	olc::Pixel RED{243, 139, 168};
	olc::Pixel GREEN{166, 227, 161};
	olc::Pixel YELLOW{249, 226, 175};
	olc::Pixel BLUE{137, 180, 250};
	olc::Pixel PINK{245, 194, 231};
	olc::Pixel TEAL{149, 226, 213};
};

} // namespace Editor
