####### Compiler, tools and options

CC            = gcc494
CXX           = g++494
DEFINES       = -DBUILDING_LIBCOMMON -DLIB_UNIX 
CFLAGS        = -pipe -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fstack-protector-strong --param=ssp-buffer-size=4 -grecord-gcc-switches -m64 -mtune=generic -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -std=c++11 -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fstack-protector-strong --param=ssp-buffer-size=4 -grecord-gcc-switches -m64 -mtune=generic -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -I../libCommon -I/usr/include -I. -I../libCommon -I.
LINK          = g++494
LFLAGS        = -Wl,-O1 -Wl,-z,relro -shared -Wl,-soname,liblibCommon.so.1
LIBS          = $(SUBLIBS)  -L/usr/lib -lpthread 
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = 
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = ../libCommon/src/event_handler.cpp \
		../libCommon/src/event_loop.cpp \
		../libCommon/src/event.cpp \
		../libCommon/src/file.cpp \
		../libCommon/src/iputils.cpp \
		../libCommon/src/local_filesys.cpp \
		../libCommon/src/mutex.cpp \
		../libCommon/src/process.cpp \
		../libCommon/src/recursive_remove.cpp \
		../libCommon/src/string.cpp \
		../libCommon/src/thread.cpp \
		../libCommon/src/time.cpp \
		../libCommon/src/util.cpp 
OBJECTS       = event_handler.o \
		event_loop.o \
		event.o \
		file.o \
		iputils.o \
		local_filesys.o \
		mutex.o \
		process.o \
		recursive_remove.o \
		string.o \
		thread.o \
		time.o \
		util.o
DIST          = ../libCommon/libCommon.pro
DESTDIR       = 
TARGET        = liblibCommon.so.1.0.0
TARGETA       = liblibCommon.a
TARGETD       = liblibCommon.so.1.0.0
TARGET0       = liblibCommon.so
TARGET1       = liblibCommon.so.1
TARGET2       = liblibCommon.so.1.0

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile  $(TARGET)

$(TARGET):  $(OBJECTS) $(SUBLIBS) $(OBJCOMP)  
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)



staticlib: $(TARGETA)

$(TARGETA):  $(OBJECTS) $(OBJCOMP) 
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(TARGETA) $(OBJECTS)

Makefile: ../libCommon/libCommon.pro  

dist: 
	@$(CHK_DIR_EXISTS) .tmp/libCommon1.0.0 || $(MKDIR) .tmp/libCommon1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/libCommon1.0.0/ && $(COPY_FILE) --parents ../libCommon/include/apply.hpp ../libCommon/include/event_handler.hpp ../libCommon/include/event_loop.hpp ../libCommon/include/event.hpp ../libCommon/include/file.hpp ../libCommon/include/index_sequence.hpp ../libCommon/include/iputils.hpp ../libCommon/include/libcommon.hpp ../libCommon/include/local_filesys.hpp ../libCommon/include/mutex.hpp ../libCommon/include/optional.hpp ../libCommon/include/process.hpp ../libCommon/include/recursive_remove.hpp ../libCommon/include/shared.hpp ../libCommon/include/string.hpp ../libCommon/include/thread.hpp ../libCommon/include/time.hpp ../libCommon/include/util.hpp ../libCommon/include/private/defs.hpp ../libCommon/include/private/visibility.hpp ../libCommon/include/private/windows.hpp .tmp/libCommon1.0.0/ && $(COPY_FILE) --parents ../libCommon/src/event_handler.cpp ../libCommon/src/event_loop.cpp ../libCommon/src/event.cpp ../libCommon/src/file.cpp ../libCommon/src/iputils.cpp ../libCommon/src/local_filesys.cpp ../libCommon/src/mutex.cpp ../libCommon/src/process.cpp ../libCommon/src/recursive_remove.cpp ../libCommon/src/string.cpp ../libCommon/src/thread.cpp ../libCommon/src/time.cpp ../libCommon/src/util.cpp .tmp/libCommon1.0.0/ && (cd `dirname .tmp/libCommon1.0.0` && $(TAR) libCommon1.0.0.tar libCommon1.0.0 && $(COMPRESS) libCommon1.0.0.tar) && $(MOVE) `dirname .tmp/libCommon1.0.0`/libCommon1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/libCommon1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA)
	-$(DEL_FILE) Makefile


check: first
compiler_clean: 

####### Compile

event_handler.o: ../libCommon/src/event_handler.cpp ../libCommon/include/event_handler.hpp \
		../libCommon/include/event_loop.hpp \
		../libCommon/include/apply.hpp \
		../libCommon/include/index_sequence.hpp \
		../libCommon/include/event.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp \
		../libCommon/include/mutex.hpp \
		../libCommon/include/time.hpp \
		../libCommon/include/private/windows.hpp \
		../libCommon/include/thread.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o event_handler.o ../libCommon/src/event_handler.cpp

event_loop.o: ../libCommon/src/event_loop.cpp ../libCommon/include/event_loop.hpp \
		../libCommon/include/apply.hpp \
		../libCommon/include/index_sequence.hpp \
		../libCommon/include/event.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp \
		../libCommon/include/mutex.hpp \
		../libCommon/include/time.hpp \
		../libCommon/include/private/windows.hpp \
		../libCommon/include/thread.hpp \
		../libCommon/include/event_handler.hpp \
		../libCommon/include/util.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o event_loop.o ../libCommon/src/event_loop.cpp

event.o: ../libCommon/src/event.cpp ../libCommon/include/event.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o event.o ../libCommon/src/event.cpp

file.o: ../libCommon/src/file.cpp ../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp \
		../libCommon/include/file.hpp \
		../libCommon/include/private/windows.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o file.o ../libCommon/src/file.cpp

iputils.o: ../libCommon/src/iputils.cpp ../libCommon/include/iputils.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o iputils.o ../libCommon/src/iputils.cpp

local_filesys.o: ../libCommon/src/local_filesys.cpp ../libCommon/include/local_filesys.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp \
		../libCommon/include/time.hpp \
		../libCommon/include/private/windows.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o local_filesys.o ../libCommon/src/local_filesys.cpp

mutex.o: ../libCommon/src/mutex.cpp ../libCommon/include/mutex.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp \
		../libCommon/include/time.hpp \
		../libCommon/include/private/windows.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mutex.o ../libCommon/src/mutex.cpp

process.o: ../libCommon/src/process.cpp ../libCommon/include/process.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp \
		../libCommon/include/private/windows.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o process.o ../libCommon/src/process.cpp

recursive_remove.o: ../libCommon/src/recursive_remove.cpp ../libCommon/include/recursive_remove.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp \
		../libCommon/include/private/windows.hpp \
		../libCommon/include/local_filesys.hpp \
		../libCommon/include/time.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o recursive_remove.o ../libCommon/src/recursive_remove.cpp

string.o: ../libCommon/src/string.cpp ../libCommon/include/string.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/private/windows.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o string.o ../libCommon/src/string.cpp

thread.o: ../libCommon/src/thread.cpp ../libCommon/include/thread.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp \
		../libCommon/include/mutex.hpp \
		../libCommon/include/time.hpp \
		../libCommon/include/private/windows.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o thread.o ../libCommon/src/thread.cpp

time.o: ../libCommon/src/time.cpp ../libCommon/include/time.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp \
		../libCommon/include/private/windows.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o time.o ../libCommon/src/time.cpp

util.o: ../libCommon/src/util.cpp ../libCommon/include/util.hpp \
		../libCommon/include/time.hpp \
		../libCommon/include/libcommon.hpp \
		../libCommon/include/private/defs.hpp \
		../libCommon/include/private/visibility.hpp \
		../libCommon/include/string.hpp \
		../libCommon/include/private/windows.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o util.o ../libCommon/src/util.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

