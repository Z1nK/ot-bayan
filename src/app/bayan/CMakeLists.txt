add_executable(bayan)

target_compile_features(bayan PUBLIC cxx_std_20)

target_sources(bayan PRIVATE main.cpp)

target_link_libraries(bayan PRIVATE
    bayan::core
)

if (MSVC)
    target_compile_options(bayan PRIVATE /W4)
else()
    target_compile_options(bayan PRIVATE -Wall -Wextra -pedantic -Werror)
endif()


# Installation

install(TARGETS bayan RUNTIME DESTINATION bin)