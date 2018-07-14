#include "file_manager.hpp"

file_::file_(std::ifstream &f, char* f_name) {
  file_ptr = &f;

  std::string line_buffer;

  while (std::getline(*file_ptr, line_buffer)) {
    file_txt.push_back(line_buffer);
  }

  current_txt = file_txt;
  file_name = f_name;
}

void file_::show() {
  for (auto line : current_txt) {
    std::cout << line << std::endl;
  }
}

txt_content file_::get_current_txt() {
  // std::string out_str = "";
  // for (auto line : current_txt) {
  //   out_str += (line + "\n");
  // }
  return this->current_txt;
}

void file_::scroll_down() {
  
}

