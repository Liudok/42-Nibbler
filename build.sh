RED='\033[0;31m'
GREEN='\033[0;32m'
EOC='\033[0m'

brewPath=$(command -v cmake)
if [ "$brewPath" == "" ]
then
	echo "${RED}Please install cmake.${EOC}"
	exit 1
else
	echo "${GREEN}Cmake found${EOC}"
fi

sfmlPath=SfmlLib/SFML
if [ ! -d "$sfmlPath" ]
then
	echo "${GREEN}Installing sfml...${EOC}"
	cd SfmlLib
	git clone https://github.com/SFML/SFML.git
	(cd SFML && cmake . && make);
	cp -R SFML/include/SFML/ /usr/local/include/SFML/
	cp -R SFML/lib/ /usr/local/lib/
	cd ..
	echo "${GREEN}Sfml installed${EOC}"
else
	echo "${GREEN}Sfml found${EOC}"
fi

buildPath=Build
if [ -d "$sfmlPath" ]
then
	rm -rf Build
fi

mkdir Build
cd Build
cmake ..
cmake --build .
