#ifndef ESMTRANSLATIONLIB_TEXTS_H
#define ESMTRANSLATIONLIB_TEXTS_H

#include "textinfo.hpp"
#include <apps/esmtool/record.hpp>
#include <components/esm/records.hpp>

namespace EsmTranslationLib
{
    template <class T>
    std::string getUniqId(T &record);

    template <class T>
    CollectedTexts collectTexts(T &record);
}

#endif //ESMTRANSLATIONLIB_TEXTS_H
