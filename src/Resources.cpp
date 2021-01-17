#include "Resources.hpp"

std::string	Resources::_COMMON_PATH = "";

void		Resources::setPath(std::string argv_0) {
	unsigned int last_slash = argv_0.find_last_of('/');
	_COMMON_PATH = argv_0.substr(0, last_slash) + "/resources/";
}

std::string	Resources::getPath(std::string resource_file) {
	return _COMMON_PATH + resource_file;
}