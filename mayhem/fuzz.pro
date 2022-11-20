TEMPLATE = app
SOURCES = fuzz_infoMessage.cpp
LIBS += -L/repo/build/release -ldiec -fsanitize=fuzzer,address
TARGET = fuzz
QMAKE_CXXFLAGS += -fsanitize=fuzzer,address
QMAKE_CXX=clang++
QMAKE_LINK=clang++
INCLUDEPATH += /repo/console_source