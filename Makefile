GREEN = '\033[0;32m'
DUILDDIR = Build

all: | $(DUILDDIR)

$(DUILDDIR):
	@sh .talk.sh "The project is not built yet" $(GREEN)
	@sh .talk.sh "Building the project" $(GREEN)
	@sh build.sh

NAME = Nibbler

all: $(NAME)

$(NAME):
	@cd Build && make && cd ..
	@sh .talk.sh "Basic usage: ./Build/Nibbler" $(GREEN)
	@sh .talk.sh "Possibly: ./Build/Nibbler [width] [heigth] [mode] [lib]" $(GREEN)
	@sh .talk.sh "Where modes may be 'classic' 'granny' 'insane' or 'rasta'" $(GREEN)
	@sh .talk.sh "Where default lib may be 'ncurses' 'sdl' ' or 'sfml'" $(GREEN)

clean:
	@sh .clean.sh

fclean:
	@sh .fclean.sh

re: fclean all
