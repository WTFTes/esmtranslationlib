#ifndef ESMTRANSLATIONLIB_WRITER_H
#define ESMTRANSLATIONLIB_WRITER_H

#include "arguments.hpp"
#include "esmdata.hpp"

namespace EsmTranslationLib
{
    int write(const Arguments &info, ESMData &data, bool throwOnErrors = false);
}

#endif