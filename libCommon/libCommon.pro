#-------------------------------------------------
#
# Project created by QtCreator 2016-08-09T12:35:23
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=gnu++11

TARGET = libCommon
TEMPLATE = lib

DEFINES += LIB_UNIX

SOURCES += \
    src/event_handler.cpp \
    src/event_loop.cpp \
    src/event.cpp \
    src/file.cpp \
    src/iputils.cpp \
    src/local_filesys.cpp \
    src/mutex.cpp \
    src/process.cpp \
    src/recursive_remove.cpp \
    src/string.cpp \
    src/thread.cpp \
    src/time.cpp \
    src/util.cpp

HEADERS += \
    include/apply.hpp \
    include/event_handler.hpp \
    include/event_loop.hpp \
    include/event.hpp \
    include/file.hpp \
    include/index_sequence.hpp \
    include/iputils.hpp \
    include/libcommon.hpp \
    include/local_filesys.hpp \
    include/mutex.hpp \
    include/optional.hpp \
    include/process.hpp \
    include/recursive_remove.hpp \
    include/shared.hpp \
    include/string.hpp \
    include/thread.hpp \
    include/time.hpp \
    include/util.hpp \
    include/private/defs.hpp \
    include/private/visibility.hpp \
    include/private/windows.hpp

