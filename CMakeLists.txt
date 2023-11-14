cmake_minimum_required(VERSION 3.26)
project(QtWidgets VERSION 1.0)

# ------ begin options
option(BUILD_TEST "Build test" OFF)
# ------ end options

# ------ begin cmake config
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(MSVC_OPTIONS /utf-8) # using utf-8 in msvc
set(GCC_OPTIONS -Werror=return-type) # warning when no returns
add_compile_options(
        "$<$<C_COMPILER_ID:MSVC>:${MSVC_OPTIONS}>"
        "$<$<CXX_COMPILER_ID:MSVC>:${MSVC_OPTIONS}>"
        "$<$<C_COMPILER_ID:GNU>:${GCC_OPTIONS}>"
        "$<$<CXX_COMPILER_ID:GNU>:${GCC_OPTIONS}>"
)
set(WINDOWS_SPECIFIC_DEFINITIONS WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE _WIN32_WINNT=0x0601)
set(MSVC_DEFINITIONS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS)
add_compile_definitions(
        "$<$<PLATFORM_ID:Windows>:${WINDOWS_SPECIFIC_MACROS}>"
        "$<$<C_COMPILER_ID:MSVC>:${MSVC_MACROS}>"
        "$<$<CXX_COMPILER_ID:MSVC>:${MSVC_MACROS}>"
)
# ------ end cmake config

# ------ begin dependencies
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
# ------ end dependencies

add_library(QtWidgets STATIC
            include/QtIconFont
            include/qticonfont.h
            src/qticonfont.cpp

            include/QtImageWidget
            include/qtimagewidget.h
            src/qtimagewidget.cpp)
target_link_libraries(QtWidgets PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(QtWidgets PUBLIC include)

if (BUILD_TEST)
    add_executable(QtIconFont_test tests/iconfont.cpp)
    target_link_libraries(QtIconFont_test PRIVATE QtWidgets)

    add_executable(QtImageWidget_test tests/imagewidget.cpp)
    target_link_libraries(QtImageWidget_test PRIVATE QtWidgets)
endif ()
