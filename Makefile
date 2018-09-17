all: script

script:
	cd Build && make && cd ..
	@sh usage.sh

clean:
	rm -rf Build

fclean: clean
	rm -rf NibblerThirdParties

re: fclean all
