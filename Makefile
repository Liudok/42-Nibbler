all: script

script:
#	sh build.sh
	cd Build && make && cd ..

clean:
	rm -rf Build

fclean: clean
	rm -rf NibblerThirdParties

re: fclean all
