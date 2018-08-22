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

thirdPartiesPath=ThirdParties
if [ ! -d "$thirdPartiesPath" ]
then
	mkdir ThirdParties
	cd ThirdParties
	echo "${GREEN}Installing and building third parties...${EOC}"
	echo "${GREEN}Please wait it may take a while${EOC}"
	sleep 1
	
	git clone https://github.com/nestoroprysk/Sdl2.git
	(cd /usr/local/opt/ && mkdir sdl2 &&
		cd sdl2 && mkdir lib);
	cp Sdl2/lib/libSDL2-2.0.0.dylib /usr/local/opt/sdl2/lib/
	
	git clone https://github.com/SFML/SFML.git
	(cd SFML && cmake . && make);
	echo "${GREEN}Third parties installed${EOC}"
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
