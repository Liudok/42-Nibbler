mkdir Build
if [[ $? == 127 ]]; then
	echo "cmake is not installed, please install it first" ;
	exit ;
fi

cd SfmlLib
git clone https://github.com/SFML/SFML.git
(cd SFML && cmake . && make);
cp -R SFML/include/SFML/ /usr/local/include/SFML/
cp -R SFML/lib/ /usr/local/lib/

cd ../Build
cmake ..
cmake --build .
