#ifndef LIB_PRIVATE_DEFS_HEADER
#define LIB_PRIVATE_DEFS_HEADER

#if defined(_MSC_VER)
	#if _MSC_VER < 1200
		#error You need to use a C++11 compiler, such as Visual Studio 2013 or higher.
	#endif
#else
	#if defined(__cplusplus) && (__cplusplus < 201103)
		#error You need to use a C++11 compiler. Try passing -std=c++11 as compiler flag.
	#endif
#endif

// Set some mandatory defines which are used to select platform implementations
#if !defined(LIB_WINDOWS) && !defined(LIB_MAC) && !defined(LIB_UNIX)
	#if defined(_WIN32) || defined(_WIN64)
        #define LIB_WINDOWS 1
	#elif defined(__APPLE__)
        #define LIB_MAC 1
	#else
        #define LIB_UNIX 1
	#endif
#endif

#if defined(LIB_WINDOWS) && defined(_MSC_VER)
// Incredibly noisy warning, disable it.
//以数据成员方式在DLL导出类中使用了模板类造成的
#pragma warning(disable:4251)

// Useless warnings
#if defined(BUILDING_LIBCOMMON) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#endif //defined(LIB_WINDOWS) && defined(_MSC_VER)

#endif
