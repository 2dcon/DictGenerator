cmake_minimum_required(VERSION 3.21)
project(DictGenerator)

set(CMAKE_CXX_STANDARD 23)

add_executable(DictGenerator main.cpp Dict.h HtmlReader.cpp HtmlReader.h HtmlTree.cpp HtmlTree.h Tools.cpp Tools.h GenDictMauro.cpp GenDictMauro.h Chars.h Decoder.cpp Decoder.h GenDictMerriam.cpp GenDictMerriam.h)
