#ifndef FILE_MANAGER_
#define FILE_MANAGER_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class file_ {
	public:
		file_(std::ifstream &f);
		void show();
		void scroll_down();
	
	private:
		std::vector<std::string> file_txt;
		std::vector<std::string> current_txt;
		std::ifstream* file_ptr;
};

#endif
