#include "editor/application.h"

int main(int argc, char** argv) {
	Editor::Application app;

	if (app.Construct(640, 480, 2, 2)) {
		app.Start();
	}

	return 0;
}
