if(USE_CLANG_TIDY)
    find_program(CMAKE_CXX_CLANG_TIDY NAMES clang-tidy REQUIRED)
    list(APPEND CMAKE_CXX_CLANG_TIDY -p;${CMAKE_BINARY_DIR})
endif()

find_program(CCACHE NAMES ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()
find_program(CLANG_FORMAT NAMES clang-format)
find_program(CLANG_TIDY NAMES clang-tidy)

if(CLANG_FORMAT)
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT} --Wno-error=unknown -i -style=file ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.h ${CMAKE_SOURCE_DIR}/test/*.cpp ${CMAKE_SOURCE_DIR}/ci/*.cpp
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

if(CLANG_TIDY)
    add_custom_target(
        tidy
        COMMAND ${CLANG_TIDY} -p ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/test/*.cpp
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()
