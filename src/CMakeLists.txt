cmake_minimum_required (VERSION 2.8)

set(SRC_FILES
	Appl.cpp
	Func.cpp
	lambda.cpp
	Lexer.cpp
	Parser.cpp
	Subs.cpp
   )

set(SRC_MAIN main.cpp)
set(SRC_TEST_MAIN test_main.cpp)

add_executable(lambda ${SRC_MAIN} ${SRC_FILES})
add_executable(test_lambda ${SRC_TEST_MAIN} ${SRC_FILES})
