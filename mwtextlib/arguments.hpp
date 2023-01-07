#ifndef MWTEXTLIB_ARGUMENTS_H
#define MWTEXTLIB_ARGUMENTS_H

#include <filesystem>
#include <components/esm/format.hpp>

namespace mwtextlib
{
    struct Arguments
    {
        std::optional<ESM::Format> mRawFormat;
        bool log_actions = false;
        bool print_records = false;
        bool loadcells_given = true;
        bool plain_given = false;

        std::string mode; //"clone";
        std::string encoding; //"win1251";
        std::filesystem::path filename;//"G:\\Downloads\\OpenMW - RU\\OpenMW - RU hd\\Data files\\Tribunal_orig.esm";
        std::filesystem::path outname;//"G:\\Downloads\\OpenMW - RU\\OpenMW - RU hd\\Data files\\Tribunal_resaved2.esm";;

        std::vector<std::string> types { };
        std::string name;
    };
}

#endif //MWTEXTLIB_ARGUMENTS_H
