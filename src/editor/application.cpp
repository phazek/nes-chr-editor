#include "application.h"

namespace Editor {

Application::Application() {
	sAppName = "NES CHR editor";
}

bool Application::OnUserCreate() {

	return true;
}

bool Application::OnUserUpdate(float fElapsedTime) {

	return true;
}

} // namespace Editor
