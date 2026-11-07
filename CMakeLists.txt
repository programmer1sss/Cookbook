cmake_minimum_required(VERSION 3.16)
project(Cookbook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_SANITIZERS "Build with ASan and UBSan" OFF)

set(PROJECT_WARNINGS -Wall -Wextra -Wpedantic)

function(apply_warnings_and_sanitizers target)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE ${PROJECT_WARNINGS} -g)
        if(ENABLE_SANITIZERS)
            target_compile_options(${target} PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer -g)
            target_link_options(${target} PRIVATE
                -fsanitize=address,undefined)
        endif()
    endif()
endfunction()

add_executable(recipe_demo
    src/main.cpp
    src/recipe.cpp
)
target_include_directories(recipe_demo PRIVATE src)
target_compile_features(recipe_demo PRIVATE cxx_std_17)
apply_warnings_and_sanitizers(recipe_demo)
