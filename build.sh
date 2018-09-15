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

thirdPartiesPath=NibblerThirdParties
if [ ! -d "$thirdPartiesPath" ]
then
	git clone https://github.com/nestoroprysk/NibblerThirdParties.git
	if [ ! -d "$thirdPartiesPath" ]
	then
		echo "${RED}${BOLD}Unable to clone third parties.${EOC}"
		exit 2
	fi
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
