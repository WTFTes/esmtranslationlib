#include "writer.hpp"
#include "components/debug/debuglog.hpp"
#include <components/esm3/esmreader.hpp>
#include <components/esm3/esmwriter.hpp>
#include <components/files/conversion.hpp>
#include <deque>
#include <iostream>
#include <fstream>
#include <unordered_set>

int mwtextlib::write(const mwtextlib::Arguments &info, ESMData &data, bool throwOnErrors)
{
    if (info.outname.empty())
    {
        std::string error = "You need to specify an output name";
        Log(Debug::Error) << error << "\n";
        if (throwOnErrors)
            throw std::exception(error.c_str());
        return 1;
    }

    size_t recordCount = data.mRecords.size();

    int digitCount = 1; // For a nicer output
    if (recordCount > 0)
        digitCount = (int) std::log10(recordCount) + 1;

    Log(Debug::Debug) << "Loaded " << recordCount << " records:\n\n";

    int i = 0;
    for (std::pair<int, int> stat: data.mRecordStats)
    {
        ESM::NAME name;
        name = stat.first;
        int amount = stat.second;
        Log(Debug::Debug) << std::setw(digitCount) << amount << " " << name.toStringView() << "  ";
        if (++i % 3 == 0)
            Log(Debug::Debug) << '\n';
    }

    if (i % 3 != 0)
        Log(Debug::Debug) << '\n';

    Log(Debug::Debug) << "\nSaving records to: " << Files::pathToUnicodeString(info.outname) << "...\n";

    ESM::ESMWriter esm;
    ToUTF8::Utf8Encoder encoder(ToUTF8::calculateEncoding(info.encoding));
    esm.setEncoder(&encoder);
    esm.setHeader(data.mHeader);
    esm.setVersion(ESM::VER_13);
    esm.setRecordCount(recordCount);

    std::fstream save(info.outname, std::fstream::out | std::fstream::binary);
    esm.save(save);

    int saved = 0;
    for (auto &record: data.mRecords)
    {
        if (i <= 0)
            break;

        const ESM::NAME typeName = record->getType();

        esm.startRecord(typeName, record->getFlags());

        record->save(esm);
        if (typeName.toInt() == ESM::REC_CELL)
        {
            ESM::Cell *ptr = &record->cast<ESM::Cell>()->get();
            if (!data.mCellRefs[ptr].empty())
            {
                for (std::pair<ESM::CellRef, bool> &ref: data.mCellRefs[ptr])
                    ref.first.save(esm, ref.second);
            }
        }

        esm.endRecord(typeName);

        saved++;
        int perc = recordCount == 0 ? 100 : (int) ((saved / (float) recordCount) * 100);
        if (perc % 10 == 0)
        {
            Log(Debug::Debug) << "\r" << perc << "%";
        }
    }

    Log(Debug::Debug) << "\rDone!";

    esm.close();
    save.close();

    return 0;
}
