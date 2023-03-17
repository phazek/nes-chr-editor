#include "olc/olcPixelGameEngine.h"

#include "style.h"

namespace Editor {

class Application: public olc::PixelGameEngine {
public:
	Application();

	// olc::PixelGameEngine
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
private:
	bool HandleInput();
	void DrawScene();

	Style style_;
};

} // namespace Editor


