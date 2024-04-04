cmake_minimum_required(VERSION 3.20)

project(rock_paper_scissors)

set(EMCC_DEBUG 1)
set(CMAKE_CXX_STANDARD 23)
add_definitions(-w)
if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -error-limit=0 --preload-file res -fwasm-exceptions -s USE_GLFW=3 -sINITIAL_MEMORY=1677721600 -s ASSERTIONS=1 -s WASM=1 -s EXPORTED_FUNCTIONS=\"['_main', '_malloc']\" -s \"EXPORTED_RUNTIME_METHODS=['ccall']\"")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMAGICKCORE_HDRI_ENABLE=1 -DCMAKE_C_FLAGS=’-fwasm-exceptions‘ -sSUPPORT_LONGJMP=wasm")
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sSUPPORT_LONGJMP=wasm " )
# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Linclude/imgparse" )

file(GLOB IMGPARSE_SRC "include/imgparse/MapleAccessor.cpp")


add_subdirectory(lib/raylib-4.2.0)
add_subdirectory(lib/raylib-cpp-4.2.7)
add_subdirectory(include/cryptopp)
include_directories(include)
include_directories(include/imgparse)
include_directories(/opt/homebrew/Cellar/nlohmann-json/3.11.3/include)
include_directories(/opt/homebrew/Cellar/boost/1.84.0_1/include)
include_directories(/opt/homebrew/Cellar/libpng/1.6.43/include)
include_directories(/usr/local/include/)
include_directories(/opt/homebrew/Cellar/imagemagick/7.1.1-29_1/include/ImageMagick-7)
include_directories(/opt/homebrew/Cellar/opencv/4.9.0_5/include/opencv4)
include_directories(/opt/homebrew/Cellar/cryptopp/8.9.0/include)
# include_directories(/Users/snow/Downloads)
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(Cryptopp REQUIRED IMPORTED_TARGET cryptopp)
# find_package(CryptoPP REQ÷UIRED)
add_library(imgparse STATIC ${IMGPARSE_SRC})
set(LIB_INCLUDES
    lib/raygui-3.2/include
)


FILE(GLOB MyCSources 
"src/*.cpp"
"src/*.hpp"
"src/util/*.cpp"
"src/util/*.hpp"
"src/screens/*.hpp"
"src/screens/*.cpp"
"include/imgparse/*.hpp"
"include/imgparse/*.cpp"
"include/imgparse/AES/AES.cpp"
"include/imgparse/pngwriter.cpp"
)
set(SOURCE_FILES
    ${MyCSources}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} cryptopp::cryptopp raylib raylib_cpp -lidbfs.js)
set_target_properties(${PROJECT_NAME}  PROPERTIES
    LINK_FLAGS "-s FETCH=1"
)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_INCLUDES} ${CRYPTOPP_INCLUDE_DIRS})
