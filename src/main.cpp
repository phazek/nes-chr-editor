#include "editor/application.h"
#include "argparse/argparse.hpp"
#include "version.h"

int main(int argc, char** argv) {
	argparse::ArgumentParser program("NES CHR editor", APP_VERSION_STRING);

	program.add_argument("path")
	.required()
    .help("Path to .nes file");

	try {
		program.parse_args(argc, argv);
	} catch (const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << program;
		std::exit(1);
	}

	Editor::Application app;
	auto path = program.get<std::string>("path");
	app.SetRomPath(path);

	if (app.Construct(850, 480, 2, 2)) {
		app.Start();
	}

	return 0;
}
