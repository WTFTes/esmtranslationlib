#ifndef ESMTRANSLATIONLIB_LOADER_H
#define ESMTRANSLATIONLIB_LOADER_H

#include "arguments.hpp"
#include "esmdata.hpp"

namespace EsmTranslationLib
{
    void printRawTes3(const std::filesystem::path &path);

    int load(const Arguments &info, ESMData *data, bool throwOnErrors = false);
}

#endif //ESMTRANSLATIONLIB_LOADER_H
