cmake_minimum_required(VERSION 3.22)

set(EXAMPLES
    basic
    nested
)

foreach(EXAMPLE IN LISTS EXAMPLES)
    add_executable(${EXAMPLE} "Examples/${EXAMPLE}.c")
    target_link_libraries(${EXAMPLE} ${PROJECT_NAME})
endforeach(EXAMPLE IN LISTS EXAMPLES)

