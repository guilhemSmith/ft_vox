#include "OS.hpp"

std::string getRessourceDir() {
   char buff[FILENAME_MAX];
   GET_CWD( buff, FILENAME_MAX );
   std::string current_working_dir(buff);
   return current_working_dir + "/resources";
}