//
// Copyright (C) 2006-2008 Mateusz Loskot
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SOCI_PLATFORM_H_INCLUDED
#define SOCI_PLATFORM_H_INCLUDED

#include <memory>
#include "soci-config.h" // for SOCI_HAVE_CXX_C11

#if defined(_MSC_VER) || defined(__MINGW32__)
#define LL_FMT_FLAGS "I64"
#else
#define LL_FMT_FLAGS "ll"
#endif

// Portability hacks for Microsoft Visual C++ compiler
#ifdef _MSC_VER
#include <stdlib.h>

// Define if you have the vsnprintf variants.
#if _MSC_VER < 1500
# define HAVE_VSNPRINTF 1
# define vsnprintf _vsnprintf
#endif

#if _MSC_VER < 1500
// Define if you have the snprintf variants.
#define HAVE_SNPRINTF 1
#define snprintf _snprintf
#endif

// Define if you have the strtoll and strtoull variants.
#if _MSC_VER >= 1300
# define HAVE_STRTOLL 1
# define HAVE_STRTOULL 1

#if _MSC_VER < 1800
namespace std {
    inline long long strtoll(char const* str, char** str_end, int base)
    {
        return _strtoi64(str, str_end, base);
    }

    inline unsigned long long strtoull(char const* str, char** str_end, int base)
    {
        return _strtoui64(str, str_end, base);
    }
}
#endif

#else
# undef HAVE_STRTOLL
# undef HAVE_STRTOULL
# error "Visual C++ versions prior 1300 don't support _strtoi64 and _strtoui64"
#endif // _MSC_VER >= 1300
#endif // _MSC_VER

// C++11 features are always available in MSVS as it has no separate C++98
// mode, we just need to check for the minimal compiler version supporting them
// (see https://msdn.microsoft.com/en-us/library/hh567368.aspx).

namespace soci
{

	namespace cxx_details
	{

#if defined(SOCI_HAVE_CXX_C11) || (defined(_MSC_VER) && _MSC_VER >= 1800)
		template <typename T>
		using auto_ptr = std::unique_ptr<T>;
#else // std::unique_ptr<> not available
		using std::auto_ptr;
#endif

	} // namespace cxx_details

} // namespace soci

#if defined(SOCI_HAVE_CXX_C11) || (defined(_MSC_VER) && _MSC_VER >= 1800)
#define SOCI_NOT_ASSIGNABLE(classname) \
        classname& operator=(const classname&) = delete;
#define SOCI_NOT_COPYABLE(classname) \
        classname(const classname&) = delete; \
        SOCI_NOT_ASSIGNABLE(classname)
#else // no C++11 deleted members support
#define SOCI_NOT_ASSIGNABLE(classname) \
        classname& operator=(const classname&);
#define SOCI_NOT_COPYABLE(classname) \
        classname(const classname&); \
        SOCI_NOT_ASSIGNABLE(classname)
#endif // C++11 deleted members available

#define SOCI_UNUSED(x) (void)x;

#if defined(SOCI_HAVE_CXX_C11) || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define SOCI_NOEXCEPT_FALSE noexcept(false)
#else
#define SOCI_NOEXCEPT_FALSE
#endif

#endif // SOCI_PLATFORM_H_INCLUDED
