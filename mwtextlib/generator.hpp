#ifndef MWTEXTLIB_GENERATOR_H
#define MWTEXTLIB_GENERATOR_H

#include <esmdefines.hpp>
#include "translationstate.hpp"

MWTEXTLIB_API mwtextlib::TranslationState *
__cdecl Translation_GetTexts(wchar_t const *fileName, char const *encoding);
MWTEXTLIB_API int
__cdecl Translation_Save(mwtextlib::TranslationState *state, wchar_t const *fileName, char const *encoding);
MWTEXTLIB_API char const *__cdecl Translation_GetLastError();

#endif //MWTEXTLIB_GENERATOR_H
