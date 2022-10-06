conan install --build=missing -if build_release -pr:b=default -s build_type=Release .
cmake -B build_release -DCMAKE_BUILD_TYPE=Release --toolchain build_release/conan_toolchain.cmake .
cmake --build build_release --config Release
