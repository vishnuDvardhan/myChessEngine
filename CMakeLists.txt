cmake_minimum_required(VERSION 3.8)

set(My chess engine)

project(${This} C CXX) 

set(CMAKE_CXX_STANDARD 11)
enable_testing()

# Add executable for tests
add_executable(unitTests board_tests.cc)

# Link libraries using the keyword signature
target_link_libraries(unitTests PRIVATE /usr/lib/libgtest.a /usr/lib/libgtest_main.a pthread)

# Enable coverage flags for Clang or GCC
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(unitTests PRIVATE --coverage)
    target_link_libraries(unitTests PRIVATE --coverage)
    
endif()

# Add custom target to capture LCOV data
add_custom_target(generate_lcov
    COMMAND lcov --capture --directory ${CMAKE_BINARY_DIR} --output-file ${CMAKE_BINARY_DIR}/lcov.info
    COMMENT "Generating LCOV coverage report"
)

# Add tests
add_test(NAME unitTests COMMAND unitTests)
add_dependencies(generate_lcov unitTests)

