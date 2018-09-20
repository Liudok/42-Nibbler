RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'

cmakePath=$(command -v cmake)
if [ "$cmakePath" == "" ]
then
    sh .talk.sh "Please install cmake" "$RED"
    exit 1
else
    sh .talk.sh "Cmake found" "$CYAN"
fi

thirdPartiesPath=NibblerThirdParties
if [ ! -d "$thirdPartiesPath" ]
then
    sh .talk.sh "Clonning third parties..." "$GREEN"
    git clone https://github.com/nestoroprysk/NibblerThirdParties.git
    if [ ! -d "$thirdPartiesPath" ]
    then
        sh .talk.sh "Unable to clone third parties" "$RED"
        exit 2
    fi
else
    sh .talk.sh "Third parties found" "$CYAN"
fi

buildPath=Build
if [ -d "$buildPath" ]
then
    sh .talk.sh "Deleting old Build repo" "$CYAN"
    rm -rf Build
fi

sh .talk.sh "Building the project..." "$GREEN"

mkdir Build
cd Build
cmake ..
