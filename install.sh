# create the files
echo "creating files"
touch ~/.tasks ~/.prd-cli-config.lua

# compile the program
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
