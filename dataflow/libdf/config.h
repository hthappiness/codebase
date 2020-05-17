
/* 好久没有这样安安静静地写代码，这是种久违的快乐 */
#pragma once

#ifdef _WIN32
	#define DF_SYMBOL_EXPORT __declspec(dllexport)
	#define DF_SYMBOL_IMPORT __declspec(dllimport)
#else
	#define DF_SYMBOL_EXPORT __attribute__((visibility("default")))
#endif

#ifdef DF_EXPORTS
	#define DF_API DF_SYMBOL_EXPORT
#else
	#define DF_API DF_SYMBOL_EXPORT
#endif

#define DF_PLUGIN_API extern "C" DF_SYMBOL_EXPORT
#define DF_API_VERSION 1