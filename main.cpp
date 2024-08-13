#include "utils/logger.h"
#include "filesystem/file_helpers.h"
#include "core/engine.h"
#include "core/globals.h"
#include "project/project.h"


int main(int argc, char* argv[]) {
	std::string current_exec = argv[0];
	std::vector<std::string> args;
	if (argc > 1) {
		args.assign(argv + 1, argv + argc);
	}

	Engine engine;
	Project project;

	if (project.FindProjectFileInDir() == false) Log.FatalError("Couldn't find a .gproj file.");
	project.InitProject();

	try {
		engine.Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}