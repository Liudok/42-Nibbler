GREEN='\033[0;32m'
RED='\033[0;31m'

cmakePath=$(command -v cmake)
if [ "$cmakePath" == "" ]
then
    sh .talk.sh "Please install cmake" "$RED"
    exit 1
else
    sh .talk.sh "Cmake found" "$GREEN"
fi

thirdPartiesPath=NibblerThirdParties
if [ ! -d "$thirdPartiesPath" ]
then
    git clone https://github.com/nestoroprysk/NibblerThirdParties.git
    if [ ! -d "$thirdPartiesPath" ]
    then
        sh .talk.sh "Unable to clone third parties" "$RED"
        exit 2
    fi
else
    sh .talk.sh "Third parties found" "$GREEN"
fi

buildPath=Build
if [ -d "$buildPath" ]
then
    sh .talk.sh "Deleting old Build repo" "$GREEN"
    rm -rf Build
fi

mkdir Build
cd Build
cmake ..
