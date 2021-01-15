#ifndef LIBS_HPP
# define LIBS_HPP

# include "OS.hpp"


# ifdef APPLE
#  include <SDL.h>
#  include <SDL_image.h>
#  include <GL/glew.h>
#  include <OpenGL/gl.h>
# endif

# ifdef LINUX
#  include <SDL2/SDL.h>
#  include <SDL2/SDL_image.h>
#  include <GL/glew.h>
#  include <GL/gl.h>
# endif

#endif