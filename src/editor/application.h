#include "olc/olcPixelGameEngine.h"

namespace Editor {

class Application: public olc::PixelGameEngine {
public:
	Application();

	// olc::PixelGameEngine
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
private:

};

} // namespace Editor


