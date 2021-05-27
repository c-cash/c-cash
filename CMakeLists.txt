cmake_minimum_required(VERSION 3.13)
project(c-cash VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(INTERPRETER
    src/interpreter/Namespace.cpp
    src/interpreter/Functions.cpp
    src/interpreter/Interpreter.cpp
)

set(PARSER 
    src/parser/FunctionDefinition.cpp
    src/parser/Parser.cpp
    src/parser/Statement.cpp
    src/parser/Tokenaizer.cpp
)

set(OTHER
    src/other/ParseSaver.cpp
)

set(TRANSPILER
    transpiler/Transpiler.cpp
)

set(VARIABLE
    src/variables/Object.cpp
    src/variables/Integer.cpp
    src/variables/Double.cpp
    src/variables/String.cpp
    src/variables/Boolean.cpp
    src/variables/SpecialObject.cpp
    src/variables/Array.cpp
)

set(LIBRARIES
    src/libraries/LibraryRegistry.cpp
    src/libraries/Library.cpp
    src/libraries/MathLibrary.cpp
)

#add execute file
add_executable(${PROJECT_NAME} main.cpp)

#add lib
add_library(${PROJECT_NAME}-inter STATIC ${INTERPRETER})
add_library(${PROJECT_NAME}-parser STATIC ${PARSER})
add_library(${PROJECT_NAME}-other STATIC ${OTHER})
add_library(${PROJECT_NAME}-transpiler STATIC ${TRANSPILER})
add_library(${PROJECT_NAME}-variable STATIC ${VARIABLE})
add_library(${PROJECT_NAME}-libs STATIC ${LIBRARIES})

#link lib
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-inter)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-parser)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-other)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-transpiler)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-variable)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-libs)

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