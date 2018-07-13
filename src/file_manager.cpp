#include "file_manager.hpp"

file_::file_(std::ifstream &f) {
  file_ptr = &f;

  std::string line_buffer;

  while (std::getline(*file_ptr, line_buffer)) {
    file_txt.push_back(line_buffer);
  }

  current_txt = file_txt;
}

void file_::show() {
  for (auto line : current_txt) {
    std::cout << line << std::endl;
  }
}

void file_::scroll_down() {
  
}

