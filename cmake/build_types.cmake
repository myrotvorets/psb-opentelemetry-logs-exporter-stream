if(CMAKE_CONFIGURATION_TYPES)
    list(APPEND CMAKE_CONFIGURATION_TYPES "Coverage" "ASAN" "LSAN" "TSAN" "UBSAN")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)
string(TOLOWER "${CMAKE_CONFIGURATION_TYPES}" CMAKE_CONFIGURATION_TYPES_LOWER)

string(REGEX MATCH "Clang" CMAKE_COMPILER_IS_CLANG "${CMAKE_CXX_COMPILER_ID}")
string(REGEX MATCH "GNU" CMAKE_COMPILER_IS_GNU "${CMAKE_CXX_COMPILER_ID}")

if(CMAKE_COMPILER_IS_GNU OR CMAKE_COMPILER_IS_CLANG)
    set(CMAKE_CXX_FLAGS_ASAN "-O1 -g -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")
    set(CMAKE_CXX_FLAGS_TSAN "-O1 -g -fsanitize=thread -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_LSAN "-O1 -g -fsanitize=leak -fno-omit-frame-pointer -fno-optimize-sibling-calls")

    if(CMAKE_COMPILER_IS_GNU)
        set(CMAKE_CXX_FLAGS_COVERAGE "-Og -g --coverage -fprofile-abs-path")
        set(CMAKE_CXX_FLAGS_UBSAN "-O1 -g -fsanitize=undefined -fsanitize=float-divide-by-zero -fno-omit-frame-pointer")
        set(CMAKE_CXX_FLAGS_MM "-Wall -Wextra -Werror -pedantic -Wno-unknown-pragmas")
    elseif(CMAKE_COMPILER_IS_CLANG)
        set(CMAKE_CXX_FLAGS_COVERAGE "-Og -g --coverage")
        set(CMAKE_CXX_FLAGS_UBSAN "-O1 -g -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=integer -fsanitize=implicit-conversion -fsanitize=local-bounds -fsanitize=nullability -fno-omit-frame-pointer")
        set(CMAKE_CXX_FLAGS_MM "-Weverything -Werror -pedantic -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -fsafe-buffer-usage-suggestions")
    endif()
endif()
