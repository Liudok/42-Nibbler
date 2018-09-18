GREEN='\033[0;32m'
RED='\033[0;31m'

filesToDelete="$(ls Build/*.dylib 2> /dev/null)"

buildPath=Build
if [ -d "$buildPath" ] && [ "$filesToDelete" != "" ]
then
	sh .clean.sh
    cd Build && make clean && cd ..
    sh .talk.sh "Binaries cleaned" "$GREEN"
else
    sh .talk.sh "Nothing to delete" "$RED"
fi
