#ifndef defines_H
#define defines_H

#ifdef WIN32
// Pragma to disable warning when using STL
#pragma warning(disable:4786)

// Pragma to disable warning when exporting classes from a DLL that contain templates components
#pragma warning(disable:4251)
#define vsnprintf _vsnprintf
#endif

#ifdef KDP2PDLL
#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )
#else
#define DllImport
#define DllExport
#endif


#endif //defines_H
