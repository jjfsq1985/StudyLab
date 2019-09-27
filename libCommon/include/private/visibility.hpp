#ifndef LIB_PRIVATE_VISIBILITY_HEADER
#define LIB_PRIVATE_VISIBILITY_HEADER


// Symbol visibility. There are two main cases: Building libcommon and using it
#ifdef BUILDING_LIBCOMMON

  // Two cases when building: Windows, other platform
  #ifdef LIB_WINDOWS
 
    // Under Windows we can either use Visual Studio or a proper compiler
    #ifdef _MSC_VER
      #ifdef DLL_EXPORT
            #define LIB_PUBLIC_SYMBOL __declspec(dllexport)
            #define EXPORT_TEMPLATE /*empty*/
      #endif
    #else
            #define EXPORT_TEMPLATE extern
      #ifdef DLL_EXPORT
            #define LIB_PUBLIC_SYMBOL __declspec(dllexport)
      #else
            #define LIB_PUBLIC_SYMBOL __attribute__((visibility("default")))
            #define LIB_PRIVATE_SYMBOL __attribute__((visibility("hidden")))
      #endif
    #endif

  #else
        #define EXPORT_TEMPLATE extern
        #define LIB_PUBLIC_SYMBOL __attribute__((visibility("default")))
        #define LIB_PRIVATE_SYMBOL __attribute__((visibility("hidden")))

  #endif

#else

  // Under MSW it makes a difference whether we use a static library or a DLL
#if defined(LIB_WINDOWS) && defined(LIB_USING_DLL)
    #define EXPORT_TEMPLATE extern
   #define LIB_PUBLIC_SYMBOL __declspec(dllimport)
  #endif

#endif

#ifndef EXPORT_TEMPLATE
    #define EXPORT_TEMPLATE
#endif

#ifndef LIB_PUBLIC_SYMBOL
    #define LIB_PUBLIC_SYMBOL
#endif

#ifndef LIB_PRIVATE_SYMBOL
    #define LIB_PRIVATE_SYMBOL
#endif

#endif
