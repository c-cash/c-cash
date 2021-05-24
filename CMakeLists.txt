cmake_minimum_required(VERSION 3.13)
project(c-cash VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(INTERPRETER 
    interpreter/Namespace.cpp
    interpreter/Functions.cpp
    interpreter/Interpreter.cpp
)

set(PARSER 
    parser/FunctionDefinition.cpp
    parser/Parser.cpp
    parser/Statement.cpp
    parser/Tokenaizer.cpp
)

set(OTHER
    other/ParseSaver.cpp
)

set(TRANSPILER
    transpiler/Transpiler.cpp
)

set(VARIABLE
    variables/Object.cpp
    variables/Integer.cpp
    variables/Double.cpp
    variables/String.cpp
    variables/Boolean.cpp
)

#add execute file
add_executable(${PROJECT_NAME} main.cpp)

#add lib
add_library(${PROJECT_NAME}-inter STATIC ${INTERPRETER})
add_library(${PROJECT_NAME}-parser STATIC ${PARSER})
add_library(${PROJECT_NAME}-other STATIC ${OTHER})
add_library(${PROJECT_NAME}-transpiler STATIC ${TRANSPILER})
add_library(${PROJECT_NAME}-variable STATIC ${VARIABLE})

#link lib
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-inter)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-parser)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-other)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-transpiler)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-variable)

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

#tests
enable_testing()

add_executable(testIf tests/ifTest.cpp)
add_test(TestIf testIf)

add_executable(testReturn tests/returnTest.cpp)
add_test(TestReturn testReturn)

add_executable(mathTest tests/mathTest.cpp)
add_test(MathTest mathTest)

add_executable(varTest tests/variableTest.cpp)
add_test(VarTest varTest)

add_executable(loopTest tests/loopTest.cpp)
add_test(LoopTest loopTest)