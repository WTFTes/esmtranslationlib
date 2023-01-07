#ifndef MWTEXTLIB_WRITER_H
#define MWTEXTLIB_WRITER_H

#include "arguments.hpp"
#include "esmdata.hpp"

namespace mwtextlib
{
    int write(const Arguments &info, ESMData &data, bool throwOnErrors = false);
}

#endif