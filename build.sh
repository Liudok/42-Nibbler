RED='\033[0;31m'
GREEN='\033[0;32m'
EOC='\033[0m'

brewPath=$(command -v brew)
if [ "$brewPath" == "" ]
then
	echo "${RED}Please install brew.${EOC}"
	exit 1
else
	echo "${GREEN}Brew found${EOC}"
fi

sdl2Installed=$(brew list | grep sdl2)
if [ "$sdl2Installed" == "" ]
then
	echo "Insalling sdl2..."
	brew install sdl2
	echo "${GREEN}sdl2 installed${EOC}"
else
	echo "${GREEN}sdl2 found${EOC}"
fi

sfmlInstalled=$(brew list | grep sfml)
if [ "$sfmlInstalled" == "" ]
then
	echo "Insalling sfml..."
	brew install sfml
	echo "${GREEN}sfml installed${EOC}"
else
	echo "${GREEN}sfml found${EOC}"
fi

mkdir Build
cd Build
cmake ..
cmake --build .
