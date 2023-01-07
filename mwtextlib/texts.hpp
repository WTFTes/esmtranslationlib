#ifndef MWTEXTLIB_TEXTS_H
#define MWTEXTLIB_TEXTS_H

#include "textinfo.hpp"
#include <apps/esmtool/record.hpp>
#include <components/esm/records.hpp>

namespace mwtextlib
{
    template <class T>
    std::string getUniqId(T &record);

    template <class T>
    CollectedTexts collectTexts(T &record);
}

#endif //MWTEXTLIB_TEXTS_H
