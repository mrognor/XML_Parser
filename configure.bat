if not exist build mkdir build

cd build

cmake ..

cd ..

xcopy xml_files\ build\xml_files /I /Y