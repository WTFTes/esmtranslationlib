#ifndef ESMTRANSLATIONLIB_ESMDATA_H
#define ESMTRANSLATIONLIB_ESMDATA_H

#include <components/esm3/loadtes3.hpp>
#include <apps/esmtool/record.hpp>
#include <deque>

namespace EsmTranslationLib
{
    struct ESMData
    {
        ESM::Header mHeader;
        std::deque<std::unique_ptr<EsmTool::RecordBase>> mRecords;
        // Value: (Reference, Deleted flag)
        std::map<ESM::Cell *, std::deque<std::pair<ESM::CellRef, bool>>> mCellRefs;
        std::map<int, int> mRecordStats;
    };
}

#endif //ESMTRANSLATIONLIB_ESMDATA_H
