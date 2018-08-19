#brew update
#brew install sfml
#git clone https://github.com/SFML/SFML.git ~/Library/Frameworks/SFML
#cd SFML
#cmake CMakeLists.txt

mkdir Build
cd SfmlLib
cmake --version;

if [[ $? == 127 ]]; then
	echo "cmake is not installed, please install it first" ;
	exit ;
fi

git clone https://github.com/SFML/SFML.git
(cd SFML && cmake . && make);

cd ../Build
cmake ..
cmake --build .
