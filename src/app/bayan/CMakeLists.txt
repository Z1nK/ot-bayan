add_executable(bayan)

target_compile_features(bayan PUBLIC cxx_std_20)

target_sources(bayan PRIVATE bayan.cpp main.cpp)

target_link_libraries(bayan PRIVATE
    cli-parser::cli-parser
    bayan::project-version
    Boost::filesystem
)

if (MSVC)
    target_compile_options(bayan PRIVATE /W4)
else()
    target_compile_options(bayan PRIVATE -Wall -Wextra -pedantic -Werror)
endif()


# Installation

install(TARGETS bayan RUNTIME DESTINATION bin)