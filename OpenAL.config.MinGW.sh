cd thirdparty;
git clone https://github.com/kcat/openal-soft.git;
cd openal-soft;
git checkout f5e0eef34db3a3ab94b61a2f99f84f078ba947e7;
cd build;
cmake .. -G "MinGW Makefiles";
cmake --build .;