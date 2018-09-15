
all: script

script:
	sh build.sh

clean:
	rm -rf Build

fclean: clean
	rm -rf NibblerThirdParties

re: fclean all
