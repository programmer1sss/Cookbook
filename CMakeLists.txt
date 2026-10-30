cmake_minimum_required(VERSION 3.16)
project(Cookbook CXX)

# ── C++17 ──
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Санітайзери ──
option(ENABLE_SANITIZERS "Build with ASan and UBSan" OFF)

# ── Спільні попередження ──
set(PROJECT_WARNINGS -Wall -Wextra -Wpedantic)

# Допоміжна функція: застосувати попередження/санітайзери до цілі
function(apply_warnings_and_sanitizers target)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE ${PROJECT_WARNINGS} -g)
        if(ENABLE_SANITIZERS)
            target_compile_options(${target} PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
                -g
            )
            target_link_options(${target} PRIVATE
                -fsanitize=address,undefined
            )
        endif()
    endif()
endfunction()

# ── Основна ціль (каталог книг) ──
add_executable(library_catalog
    src/main.cpp
    src/book.cpp
)
target_compile_features(library_catalog PRIVATE cxx_std_17)
apply_warnings_and_sanitizers(library_catalog)

# ── Демонстрація часу життя Book ──
add_executable(lifetime_demo
    lifetime_demo.cpp
    src/book.cpp
)
target_compile_features(lifetime_demo PRIVATE cxx_std_17)
target_include_directories(lifetime_demo PRIVATE src)
apply_warnings_and_sanitizers(lifetime_demo)

# ── Кулінарна книга: демонстрація часу життя Recipe ──
add_executable(recipe_demo
    recipe_demo.cpp
    src/recipe.cpp
)
target_compile_features(recipe_demo PRIVATE cxx_std_17)
target_include_directories(recipe_demo PRIVATE src)
apply_warnings_and_sanitizers(recipe_demo)

# ── Тести ──
enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# book_test
add_executable(book_test
    tests/book_test.cpp
    src/book.cpp
)
target_compile_features(book_test PRIVATE cxx_std_17)
target_include_directories(book_test PRIVATE src)
target_link_libraries(book_test PRIVATE GTest::gtest_main)
apply_warnings_and_sanitizers(book_test)

# recipe_test
add_executable(recipe_test
    tests/recipe_test.cpp
    src/recipe.cpp
)
target_compile_features(recipe_test PRIVATE cxx_std_17)
target_include_directories(recipe_test PRIVATE src)
target_link_libraries(recipe_test PRIVATE GTest::gtest_main)
apply_warnings_and_sanitizers(recipe_test)

include(GoogleTest)
gtest_discover_tests(book_test)
gtest_discover_tests(recipe_test)