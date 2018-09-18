GREEN='\033[0;32m'
RED='\033[0;31m'

executable=Build/Nibbler
if [ -e "$executable" ]
then	
    sh .talk.sh "Basic usage: ./Build/Nibbler" "$GREEN"
	sh .talk.sh "Possibly: ./Build/Nibbler [width] [heigth] [mode] [lib]" "$GREEN"
	sh .talk.sh "Where modes may be 'classic' 'granny' 'insane' or 'rasta'" "$GREEN"
	sh .talk.sh "Where default lib may be 'ncurses' 'sdl' ' or 'sfml'" "$GREEN"
else
    sh .talk.sh "Unable to build the project" "$RED"
fi
