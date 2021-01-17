#ifndef RESOURCES_HPP
# define RESOURCES_HPP

# include <string>

class Resources {
	private:
	static std::string	_COMMON_PATH;
	public:
	static void			setPath(std::string argv_0);
	static std::string	getPath(std::string resource_file);
};

#endif