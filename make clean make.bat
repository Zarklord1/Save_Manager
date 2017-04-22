rmdir %cd%\build /q /s 2>nul
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=%WUT_ROOT%\cmake\wut-toolchain.cmake -G "Unix Makefiles" ../
make
pause