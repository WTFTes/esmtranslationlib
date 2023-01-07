#ifndef MWTEXTLIB_LOADER_H
#define MWTEXTLIB_LOADER_H

#include "arguments.hpp"
#include "esmdata.hpp"

namespace mwtextlib
{
    void printRawTes3(const std::filesystem::path &path);

    int load(const Arguments &info, ESMData *data, bool throwOnErrors = false);
}

#endif //MWTEXTLIB_LOADER_H
