RED='\033[0;31m'
GREEN='\033[0;32m'
BOLD=$(tput bold)
EOC='\033[0m'

cmakePath=$(command -v cmake)
if [ "$cmakePath" == "" ]
then
	echo "${RED}${BOLD}Please install cmake.${EOC}"
	exit 1
else
	echo "${GREEN}Cmake found${EOC}"
fi

thirdPartiesPath=ThirdParties
if [ ! -d "$thirdPartiesPath" ]
then
	mkdir ThirdParties
	cd ThirdParties
	echo "${GREEN}${BOLD}Installing and building third parties...${EOC}"
	echo "${GREEN}${BOLD}Please wait it may take a while${EOC}"
	sleep 1

	git clone https://github.com/SDL-mirror/SDL.git
<<<<<<< HEAD
	cd SDL
	git clone https://github.com/ysgard/sdl2frameworks.git
	cd ..
=======
>>>>>>> master
	mkdir SDL/lib
	(cd SDL/lib && cmake .. && cmake --build .);

	git clone https://github.com/SFML/SFML.git
	(cd SFML && cmake . && cmake --build .);
	echo "${GREEN}${BOLD}Third parties installed${EOC}"
	cd ..
else
	echo "${GREEN}Third parties found${EOC}"
fi

buildPath=Build
if [ -d "$buildPath" ]
then
	echo "Deleting old Build repo"
	rm -rf Build
fi

mkdir Build
cd Build
cmake ..
cmake --build .