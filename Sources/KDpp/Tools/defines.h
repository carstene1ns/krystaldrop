#ifndef defines_H
#define defines_H

#ifdef WIN32
#pragma warning(disable:4786)
#define vsnprintf _vsnprintf
#endif

#ifdef KDPPDLL
#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )
#else
#define DllImport
#define DllExport
#endif


#endif //defines_H
