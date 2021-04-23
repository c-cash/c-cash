cmake_minimum_required(VERSION 3.13)
project(c-cash VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(INTERPRETER 
    interpreter/Functions.cpp
    interpreter/Interpreter.cpp
)

set(PARSER 
    parser/FunctionDefinition.cpp
    parser/Parser.cpp
    parser/Statement.cpp
    parser/Tokenaizer.cpp
)

#add execute file
add_executable(${PROJECT_NAME} main.cpp)

#add lib
add_library(${PROJECT_NAME}-inter STATIC ${INTERPRETER})
add_library(${PROJECT_NAME}-parser STATIC ${PARSER})

#link lib
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-inter)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-parser)

#Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)
set_target_properties(${PROJECT_NAME}-inter PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)
set_target_properties(${PROJECT_NAME}-parser PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)