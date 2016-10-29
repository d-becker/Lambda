cmake_minimum_required (VERSION 2.8)

set(SRC_FILES
	Appl.cpp
	Func.cpp
	lambda.cpp
	Lexer.cpp
	main.cpp
	Parser.cpp
	Subs.cpp
   )

add_executable(lambda "main.cpp" ${SRC_FILES})
