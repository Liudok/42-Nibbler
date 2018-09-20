RED='\033[0;31m'
CYAN='\033[0;36m'

filesToDelete="$(ls *.dylib 2> /dev/null)"

sh .clean.sh

buildPath=Build
if [ -d "$buildPath" ] && [ "$filesToDelete" != "" ]
then
    sh .rm.sh
    sh .talk.sh "Binaries cleaned" "$CYAN"
else
    sh .talk.sh "No binaries to delete" "$RED"
fi
