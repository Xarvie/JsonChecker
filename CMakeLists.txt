PROJECT(main)
cmake_minimum_required (VERSION 3.2)
list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/cmake/macros")
include(AutoCollect)
include(StaticMFC)

# cd build
# cmake .. -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"

set(USE_STATIC_MFC 1)
list(APPEND CODEFILES ${CMAKE_CURRENT_SOURCE_DIR}/logo.rc)


PickIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    INCLUDEDIRS
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/build)

PickSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    CODEFILES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/build)

MakeFilter(
    ${CMAKE_CURRENT_SOURCE_DIR}/code)
        
ADD_EXECUTABLE(main 
    ${CODEFILES})
target_include_directories(main
    PUBLIC ${INCLUDEDIRS})
