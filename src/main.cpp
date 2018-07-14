#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>

#include "file_manager.hpp"
#include "gui.hpp"

int main(int argc, char* argv[]){
	if (argc != 2) {
		std::cout << "Usage : " << argv[0] << " [input file]" << std::endl;
		return 1;
	}

	std::ifstream input_file;
	input_file.open(argv[1]);

	if (!input_file.is_open()) {
		std::cout << "Error while opening " << argv[1] << std::endl;
	}

	file_ new_file(input_file, argv[1]);

	new_file.show();

	GUI g(new_file);

	g.run();

	return 0;
}
