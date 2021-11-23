#pragma once

#pragma warning(disable: 6335 6387)

#define x64

#ifdef x32
#define X_GetThreadContext Wow64GetThreadContext
#define X_PCONTEXT PWOW64_CONTEXT
#else
#define Cross_GetThreadContext GetThreadContext
#define X_PCONTEXT PCONTEXT
#endif