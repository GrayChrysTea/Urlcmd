add_library(
    ${PROJECT_NAME}-parser STATIC
    constants.cpp
    parser.cpp
    utils.cpp
    baseportion.cpp
    "path.cpp"
    query.cpp
    queryformat.cpp
    fragment.cpp
)
