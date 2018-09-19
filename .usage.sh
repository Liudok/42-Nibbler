RED='\033[0;31m'
GREEN='\033[0;32m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'

executable=Build/Nibbler
if [ -e "$executable" ]
then	
    sh .talk.sh "Basic usage: ./Build/Nibbler" "$GREEN"
	sh .talk.sh "Possibly: ./Build/Nibbler [width] [heigth] [gameMode] [lib] [musicPlayerMode]" "$GREEN"
	sh .talk.sh "Where gameMode may be 'classic' 'granny' 'insane', or 'rasta'" "$PURPLE"
	sh .talk.sh "Where default lib may be 'glfw' 'sdl', ' or 'sfml'" "$PURPLE"
	sh .talk.sh "Where musicPlayerMode may be 'on' 'soundsOnly', or 'off'" "$PURPLE"
	sh .talk.sh "You may change the gui at the runtime by pressing '1', '2', or '3'" "$CYAN"
	sh .talk.sh "You may change the game mode at the runtime by pressing '4'" "$CYAN"
else
    sh .talk.sh "Unable to build the project" "$RED"
fi
