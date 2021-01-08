#! /bin/bash

prefix="\033[2;37m[ft_vox/setup] - \033[0m"

function mac_setup() {
	list=`brew list --formulae`;
	if [ $? -ne 0 ]
	then
		printf "$prefix brew is missing.\n";
		rm -rf $HOME/.brew \
			&& git clone --depth=1 https://github.com/Homebrew/brew $HOME/.brew \
			&& echo 'export PATH=$HOME/.brew/bin:$PATH' >> $HOME/.zshrc \
			&& source $HOME/.zshrc \
			&& brew update;
		printf "$prefix brew installed.\n";
		list=`brew list --formulae`;
		if [ $? -ne 0 ]
		then
			printf "$prefix\033[2;31mbrew installation failed.\n\033[0m";
			exit 1;
		fi
	fi

	sdl2=`echo $list | grep sdl2`
	if [ $? -ne 0 ]
	then
		printf "$prefix sdl2 is missing.\n"
		brew install SDL2
		if [ $? -ne 0 ]
		then
			printf "$prefix\033[2;31msdl2 installation failed.\n\033[0m";
			exit 1;
		fi
		printf "$prefix sdl2 installed.\n"
	fi

	sdl2_image=`echo $list | grep sdl2_image`
	if [ $? -ne 0 ]
	then
		printf "$prefix sdl2_image is missing.\n"
		brew install sdl2_image
		if [ $? -ne 0 ]
		then
			printf "$prefix\033[2;31msdl2_image installation failed.\n\033[0m";
			exit 1;
		fi
		printf "$prefix sdl2_image installed.\n"
	fi

	sdl2_ttf=`echo $list | grep sdl2_ttf`
	if [ $? -ne 0 ]
	then
		printf "$prefix sdl2_ttf is missing.\n"
		brew install sdl2_ttf
		if [ $? -ne 0 ]
		then
			printf "$prefix\033[2;31msdl2_ttf installation failed.\n\033[0m";
			exit 1;
		fi
		printf "$prefix sdl2_ttf installed.\n"
	fi

	glew=`echo $list | grep glew`
	if [ $? -ne 0 ]
	then
		printf "$prefix glew is missing.\n"
		brew install glew
		if [ $? -ne 0 ]
		then
			printf "$prefix\033[2;31mglew installation failed.\n\033[0m";
			exit 1;
		fi
		printf "$prefix glew installed.\n"
	fi

	glm=`echo $list | grep glm`
	if [ $? -ne 0 ]
	then
		printf "$prefix glm is missing.\n"
		brew install glm
		if [ $? -ne 0 ]
		then
			printf "$prefix\033[2;31mglm installation failed.\n\033[0m";
			exit 1;
		fi
		printf "$prefix glm installed.\n"
	fi

	freetype=`echo $list | grep freetype`
	if [ $? -ne 0 ]
	then
		printf "$prefix freetype is missing.\n"
		brew install freetype
		if [ $? -ne 0 ]
		then
			printf "$prefix\033[2;31mfreetype installation failed.\n\033[0m";
			exit 1;
		fi
		printf "$prefix freetype installed.\n"
	fi
}

function linux_setup() {
	sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libfreetype6-dev libglew-dev libglm-dev;
}

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    linux_setup;
elif [[ "$OSTYPE" == "darwin"* ]]; then
    mac_setup;
fi
