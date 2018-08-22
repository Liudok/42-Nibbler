RED='\033[0;31m'
GREEN='\033[0;32m'
EOC='\033[0m'

cmakePath=$(command -v cmake)
if [ "$cmakePath" == "" ]
then
	echo "${RED}Please install cmake.${EOC}"
	exit 1
else
	echo "${GREEN}Cmake found${EOC}"
fi

sfmlPath=SFML
if [ ! -d "$sfmlPath" ]
then
	echo "${GREEN}Installing sfml...${EOC}"
	git clone https://github.com/SFML/SFML.git
	(cd SFML && cmake . && make);
	echo "${GREEN}Sfml installed${EOC}"
else
	echo "${GREEN}Sfml found${EOC}"
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
