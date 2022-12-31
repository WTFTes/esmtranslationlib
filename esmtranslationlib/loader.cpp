#include "loader.hpp"
#include <components/debug/debuglog.hpp>
#include <components/esm3/esmreader.hpp>
#include <components/files/conversion.hpp>
#include <components/files/openfile.hpp>
#include <components/misc/strings/algorithm.hpp>
#include <apps/esmtool/labels.hpp>
#include <iostream>
#include <fstream>
#include <unordered_set>

namespace EsmTranslationLib
{
    void printRawTes3(const std::filesystem::path &path)
    {
        Log(Debug::Debug) << "TES3 RAW file listing: " << Files::pathToUnicodeString(path) << '\n';
        ESM::ESMReader esm;
        esm.openRaw(path);
        while (esm.hasMoreRecs())
        {
            ESM::NAME n = esm.getRecName();
            Log(Debug::Debug) << "Record: " << n.toStringView() << '\n';
            esm.getRecHeader();
            while (esm.hasMoreSubs())
            {
                size_t offs = esm.getFileOffset();
                esm.getSubName();
                esm.skipHSub();
                n = esm.retSubName();
                Log(Debug::Debug) << "    " << n.toStringView() << " - " << esm.getSubSize() << " bytes @ 0x"
                                  << std::hex
                                  << offs << std::dec << '\n';
            }
        }
    }

    void loadCell(const Arguments &info, ESM::Cell &cell, ESM::ESMReader &esm, ESMData *data)
    {
        bool logActions = info.log_actions;
        bool printRecords = info.log_actions && info.print_records && info.mode != "clone";
        bool save = (info.mode == "clone");

        // Skip back to the beginning of the reference list
        // FIXME: Changes to the references backend required to support multiple plugins have
        //  almost certainly broken this following line. I'll leave it as is for now, so that
        //  the compiler does not complain.
        cell.restore(esm, 0);

        // Loop through all the references
        ESM::CellRef ref;
        if (printRecords)
            Log(Debug::Debug) << "  References:\n";

        bool deleted = false;
        ESM::MovedCellRef movedCellRef;
        bool moved = false;
        while (cell.getNextRef(esm, ref, deleted, movedCellRef, moved))
        {
            if (data != nullptr && save)
                data->mCellRefs[&cell].push_back(std::make_pair(ref, deleted));

            if (!printRecords)
                continue;

            Log(Debug::Debug) << "  - Refnum: " << ref.mRefNum.mIndex << '\n';
            Log(Debug::Debug) << "    ID: " << ref.mRefID << '\n';
            Log(Debug::Debug) << "    Position: (" << ref.mPos.pos[0] << ", " << ref.mPos.pos[1] << ", "
                              << ref.mPos.pos[2]
                              << ")\n";
            if (ref.mScale != 1.f)
                Log(Debug::Debug) << "    Scale: " << ref.mScale << '\n';
            if (!ref.mOwner.empty())
                Log(Debug::Debug) << "    Owner: " << ref.mOwner << '\n';
            if (!ref.mGlobalVariable.empty())
                Log(Debug::Debug) << "    Global: " << ref.mGlobalVariable << '\n';
            if (!ref.mFaction.empty())
                Log(Debug::Debug) << "    Faction: " << ref.mFaction << '\n';
            if (!ref.mFaction.empty() || ref.mFactionRank != -2)
                Log(Debug::Debug) << "    Faction rank: " << ref.mFactionRank << '\n';
            Log(Debug::Debug) << "    Enchantment charge: " << ref.mEnchantmentCharge << '\n';
            Log(Debug::Debug) << "    Uses/health: " << ref.mChargeInt << '\n';
            Log(Debug::Debug) << "    Gold value: " << ref.mGoldValue << '\n';
            Log(Debug::Debug) << "    Blocked: " << static_cast<int>(ref.mReferenceBlocked) << '\n';
            Log(Debug::Debug) << "    Deleted: " << deleted << '\n';
            if (!ref.mKey.empty())
                Log(Debug::Debug) << "    Key: " << ref.mKey << '\n';
            Log(Debug::Debug) << "    Lock level: " << ref.mLockLevel << '\n';
            if (!ref.mTrap.empty())
                Log(Debug::Debug) << "    Trap: " << ref.mTrap << '\n';
            if (!ref.mSoul.empty())
                Log(Debug::Debug) << "    Soul: " << ref.mSoul << '\n';
            if (ref.mTeleport)
            {
                Log(Debug::Debug) << "    Destination position: (" << ref.mDoorDest.pos[0] << ", "
                                  << ref.mDoorDest.pos[1]
                                  << ", " << ref.mDoorDest.pos[2] << ")\n";
                if (!ref.mDestCell.empty())
                    Log(Debug::Debug) << "    Destination cell: " << ref.mDestCell << '\n';
            }
            Log(Debug::Debug) << "    Moved: " << std::boolalpha << moved << std::noboolalpha << '\n';
            if (moved)
            {
                Log(Debug::Debug) << "    Moved refnum: " << movedCellRef.mRefNum.mIndex << '\n';
                Log(Debug::Debug) << "    Moved content file: " << movedCellRef.mRefNum.mContentFile << '\n';
                Log(Debug::Debug) << "    Target: " << movedCellRef.mTarget[0] << ", " << movedCellRef.mTarget[1]
                                  << '\n';
            }
        }
    }

    int
    loadTes3(const Arguments &info, std::unique_ptr<std::ifstream> &&stream, ESMData *data, bool throwOnErrors = false)
    {
        Log(Debug::Debug) << "Loading TES3 file: " << info.filename << '\n';

        ESM::ESMReader esm;
        ToUTF8::Utf8Encoder encoder(ToUTF8::calculateEncoding(info.encoding));
        esm.setEncoder(&encoder);

        std::unordered_set<uint32_t> skipped;

        int infos = 0;
        int infosWithoutDialogue = 0;
        try
        {
            bool logActions = info.log_actions;
            bool printRecords = info.log_actions && info.print_records && info.mode != "clone";
            bool loadCells = (info.loadcells_given || info.mode == "clone");
            bool save = (info.mode == "clone");

            esm.open(std::move(stream), info.filename);

            if (data != nullptr)
                data->mHeader = esm.getHeader();

            if (logActions)
            {
                Log(Debug::Debug) << "Author: " << esm.getAuthor() << '\n'
                                  << "Description: " << esm.getDesc() << '\n'
                                  << "File format version: " << esm.getFVer() << '\n';
                std::vector<ESM::Header::MasterData> masterData = esm.getGameFiles();
                if (!masterData.empty())
                {
                    Log(Debug::Debug) << "Masters:" << '\n';
                    for (const auto &master: masterData)
                        Log(Debug::Debug) << "  " << master.name << ", " << master.size << " bytes\n";
                }
            }

            ESM::Dialogue *dialogue = nullptr;

            // Loop through all records
            while (esm.hasMoreRecs())
            {
                const ESM::NAME n = esm.getRecName();
                uint32_t flags;
                esm.getRecHeader(flags);
                auto isIgnored = esm.getRecordFlags() & ESM::FLAG_Ignored;

                auto record = EsmTool::RecordBase::create(n);
                if (record == nullptr)
                {
                    if (logActions && skipped.count(n.toInt()) == 0)
                    {
                        Log(Debug::Debug) << "Skipping " << n.toStringView() << " records.\n";
                        skipped.emplace(n.toInt());
                    }

                    esm.skipRecord();
                    if (!logActions)
                        break;
                    Log(Debug::Debug) << "  Skipping\n";

                    continue;
                }

                record->setFlags(static_cast<int>(flags));
                record->setPrintPlain(info.plain_given);
                record->load(esm);

                if (!isIgnored)
                {
                    if (record->getType().toInt() == ESM::REC_INFO)
                    {
                        ++infos;
                        if (dialogue)
                        {
                            auto info = record->cast<ESM::DialInfo>();
                            dialogue->storeInfo(info->get(), info->getIsDeleted(), esm.getIndex() != 0);
                        } else
                        {
                            ++infosWithoutDialogue;
                            std::cout << 123 << std::endl;
                        }
                        //throw std::exception("Info without dialog");
                    }

                    if (record->getType().toInt() == ESM::REC_DIAL)
                        dialogue = &record->cast<ESM::Dialogue>()->get();
                    else if (record->getType().toInt() != ESM::REC_INFO)
                        dialogue = nullptr;
                }

                //                record->print();

                // Is the user interested in this record type?
                bool interested = true;
                if (!info.types.empty()
                    && std::find(info.types.begin(), info.types.end(), n.toStringView()) == info.types.end())
                    interested = false;

                if (!info.name.empty() && !Misc::StringUtils::ciEqual(info.name, record->getId()))
                    interested = false;

                if (printRecords && interested)
                {
                    Log(Debug::Debug) << "\nRecord: " << n.toStringView() << " '" << record->getId() << "'\n"
                                      << "Record flags: " << recordFlags(record->getFlags()) << '\n';
                    record->print();
                }

                if (record->getType().toInt() == ESM::REC_CELL && loadCells && interested)
                {
                    loadCell(info, record->cast<ESM::Cell>()->get(), esm, data);
                }

                if (data != nullptr)
                {
                    if (save)
                        data->mRecords.push_back(std::move(record));
                    ++data->mRecordStats[n.toInt()];
                }
            }
        }
        catch (const std::exception &e)
        {
            Log(Debug::Error) << "\nERROR:\n\n  " << e.what();
            if (data != nullptr)
                data->mRecords.clear();
            if (throwOnErrors)
                throw e;
            return 1;
        }

        return 0;
    }

    int load(const Arguments &info, ESMData *data, bool throwOnErrors)
    {
        if (info.mRawFormat.has_value() && info.mode == "dump")
        {
            switch (*info.mRawFormat)
            {
                case ESM::Format::Tes3:
                    printRawTes3(info.filename);
                    break;
                case ESM::Format::Tes4:
                    Log(Debug::Debug) << "Printing raw TES4 file is not supported: "
                                      << Files::pathToUnicodeString(info.filename) << "\n";
                    break;
            }
            return 0;
        }

        auto stream = Files::openBinaryInputFileStream(info.filename);
        if (!stream->is_open())
        {
            std::string error = std::string("Failed to open file: ") + std::strerror(errno);
            Log(Debug::Error) << error << '\n';
            if (throwOnErrors)
                throw std::exception(error.c_str());
            return -1;
        }

        const ESM::Format format = ESM::readFormat(*stream);
        stream->seekg(0);

        switch (format)
        {
            case ESM::Format::Tes3:
                return loadTes3(info, std::move(stream), data, throwOnErrors);
            default:
                break;
        }

        std::string error = std::string("Unsupported ESM format: ") + ESM::NAME(format).toString();
        Log(Debug::Debug) << error.c_str() << '\n';
        if (throwOnErrors)
            throw std::exception(error.c_str());
        return -1;
    }
}