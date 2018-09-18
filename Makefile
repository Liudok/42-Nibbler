GREEN = '\033[0;32m'
CYAN='\033[0;36m'

DUILDDIR = Build

all: | $(DUILDDIR)

$(DUILDDIR):
	@sh .talk.sh "The project is not built yet" $(CYAN)
	@sh .talk.sh "Building the project" $(GREEN)
	@sh build.sh || true

NAME = Nibbler

all: $(NAME)

$(NAME):
	@cd Build 2> /dev/null && make && cd .. || true
	@sh .usage.sh

clean:
	@sh .clean.sh

fclean:
	@sh .fclean.sh

re: fclean all
