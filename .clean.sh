GREEN='\033[0;32m'
RED='\033[0;31m'

filesToDelete="$(find . -name "*\.o" | cut -d "/" -f 2-)"

if [ "$filesToDelete" == "" ]
then
	sh .talk.sh "No *.o files to delete" "$RED"
else
	rm $filesToDelete
	sh .talk.sh "*.o files cleaned" "$GREEN"
fi
