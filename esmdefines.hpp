#ifndef MWTEXTLIB_ESMDEFINES_H
#define MWTEXTLIB_ESMDEFINES_H

#if defined(MWTEXTLIB_EXPORT)
# define MWTEXTLIB_API extern "C" __declspec(dllexport)
# define MWTEXTLIB_API_CLASS __declspec(dllexport)
#else
# define MWTEXTLIB_API
# define MWTEXTLIB_API_CLASS
#endif

#endif //MWTEXTLIB_ESMDEFINES_H
