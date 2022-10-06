conan install --build=missing -if build_debug -pr:b=default -s build_type=Debug .
cmake -B build_debug -DCMAKE_BUILD_TYPE=Debug --toolchain build_debug/conan_toolchain.cmake .
cmake --build build_debug --config Debug
