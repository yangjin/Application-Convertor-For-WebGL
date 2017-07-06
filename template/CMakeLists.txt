set(W2N_LIB w2n)
set(GED_LIB ged)
set(SKIA_LIB skia)

FILE(GLOB SOURCES *.cpp)
FILE(GLOB HEADERS *.h)

add_library(${W2N_LIB} ${SOURCES} ${HEADERS})

set(PROGRAM gl_code_main)
add_executable(${PROGRAM} gbm.cpp)
target_link_libraries(${PROGRAM} ${GED_LIB} ${W2N_LIB} ${SKIA_LIB} pthread)
MESSAGE("${PROGRAM} links ${GED_LIB} ${W2N_LIB} ${SKIA_LIB} pthread")
