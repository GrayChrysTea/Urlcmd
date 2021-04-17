add_executable(${PROJECT_NAME} main.cpp appargs.cpp parser.cpp)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ${Boost_PROGRAM_OPTIONS_LIBRARIES}
    ${Boost_REGEX_LIBRARIES}
)