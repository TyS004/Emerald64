#ifdef E64_WIN32
	#ifdef E64_BUILD_DLL
		#define E64_API __declspec(dllexport)
	#else
		#define E64_API __declspec(dllimport)
	#endif
#else
	#define E64_API
#endif