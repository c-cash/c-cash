cmake_minimum_required(VERSION 3.16)
project(c-cash VERSION 0.4.0)

set(CMAKE_CXX_STANDARD 17)

set(PARSER 
    src/parser/FunctionDefinition.cpp
    src/parser/Parser.cpp
    src/parser/Statement.cpp
    src/parser/Tokenaizer.cpp
    src/parser/ClassDefinition.cpp
)
set(TRANSPILER
    src/transpiler/Transpiler.cpp
)
set(COMPILER
    src/compiler/Compiler.cpp
)
set(VM
    src/vm/VirtualMachine.cpp
    src/vm/VMClass.cpp
)
set(ANALYSIS
    src/analysis/Analysis.cpp
)
set(LINKER
    src/linker/Linker.cpp
)
set(OPTIMIZER
    src/optimizer/Optimizer.cpp
)

#add execute file
add_executable(${PROJECT_NAME} main.cpp)

#add lib
add_library(${PROJECT_NAME}-parser STATIC ${PARSER})
add_library(${PROJECT_NAME}-transpiler STATIC ${TRANSPILER})
add_library(${PROJECT_NAME}-compiler STATIC ${COMPILER})
add_library(${PROJECT_NAME}-vm STATIC ${VM})
add_library(${PROJECT_NAME}-analysis STATIC ${ANALYSIS})
add_library(${PROJECT_NAME}-linker STATIC ${LINKER})
add_library(${PROJECT_NAME}-optimizer STATIC ${OPTIMIZER})

#link lib
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-parser)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-transpiler)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-compiler)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-vm)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-analysis)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-linker)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-optimizer)

#Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)
set_target_properties(${PROJECT_NAME}-parser PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)
set_target_properties(${PROJECT_NAME}-transpiler PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)
set_target_properties(${PROJECT_NAME}-compiler PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)
set_target_properties(${PROJECT_NAME}-vm PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)
set_target_properties(${PROJECT_NAME}-analysis PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)
set_target_properties(${PROJECT_NAME}-linker PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)
set_target_properties(${PROJECT_NAME}-optimizer PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON 
)

#tests
#
#enable_testing()
#
#add_executable(testIf tests/ifTest.cpp)
#add_test(TestIf testIf)
#
#add_executable(testReturn tests/returnTest.cpp)
#add_test(TestReturn testReturn)
#
#add_executable(mathTest tests/mathTest.cpp)
#add_test(MathTest mathTest)
#
#add_executable(varTest tests/variableTest.cpp)
#add_test(VarTest varTest)
#
#add_executable(loopTest tests/loopTest.cpp)
#add_test(LoopTest loopTest)
#
#add_executable(tableTest tests/tableTest.cpp)
#add_test(TableTest tableTest)
#