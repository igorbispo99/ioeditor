#ifndef FILE_MANAGER_
#define FILE_MANAGER_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

typedef std::vector<std::string> txt_content;

class file_ {
	public:
		file_(std::ifstream &f, char* f_name);
		void show();
		void scroll_down();
		txt_content get_current_txt();

		std::string file_name;
	
	private:
		txt_content file_txt;
		txt_content current_txt;
		std::ifstream* file_ptr;
};

#endif
