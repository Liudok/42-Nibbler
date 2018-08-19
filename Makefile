
default_target: script

script:
	sh build.sh

clean:
	rm -rf Build

fclean: clean
	rm -rf Build

re: fclean all