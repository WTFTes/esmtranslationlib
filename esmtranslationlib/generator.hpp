#ifndef ESMTRANSLATIONLIB_GENERATOR_H
#define ESMTRANSLATIONLIB_GENERATOR_H

#include <esmdefines.hpp>
#include "translationstate.hpp"

ESMTRANSLATIONLIB_API EsmTranslationLib::TranslationState *
__cdecl Translation_GetTexts(wchar_t const *fileName, char const *encoding);
ESMTRANSLATIONLIB_API int
__cdecl Translation_Save(EsmTranslationLib::TranslationState *state, wchar_t const *fileName, char const *encoding);
ESMTRANSLATIONLIB_API char const *__cdecl Translation_GetLastError();

#endif //ESMTRANSLATIONLIB_GENERATOR_H
