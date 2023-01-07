#ifndef MWTEXTLIB_TRANSLATIONSTATE_H
#define MWTEXTLIB_TRANSLATIONSTATE_H

#include <vector>
#include <memory>
#include "translationrecord.hpp"
#include <esmdefines.hpp>
#include <components/debug/debuglog.hpp>
#include "esmdata.hpp"

namespace mwtextlib
{
    struct TranslationRecordInfo
    {
        TranslationRecord *pointer = nullptr;
        char const *source = nullptr;
        char const *target = nullptr;
        char const *contextName = nullptr;
        char const *contextId = nullptr;
        char const *meta = nullptr;
        int index = -1;
        int type = 0;
        int maxLength = 0;
    };

    static_assert(sizeof(TranslationRecordInfo) == 64, "asd");

    class TranslationState
    {
        int position = -1;

        static std::string lastError;

        ESMData data;
        std::vector<std::unique_ptr<TranslationRecord>> records;

        std::filesystem::file_time_type lastWriteTime;

        void addTexts(EsmTool::RecordBase &record, CollectedTexts &texts, std::string const &contextName = "",
                      std::string const &contextId = "");

    public:
        ~TranslationState()
        {
            Log(Debug::Debug) << "State destroyed";
        }

        ESMData &getData()
        {
            return data;
        }

        void Add(std::unique_ptr<TranslationRecord> &&record)
        {
            records.push_back(std::move(record));
        }

        TranslationRecord *getNextRecord()
        {
            if (position + 1 < records.size())
            {
                ++position;
                return records[position].get();
            }

            return nullptr;
        }

        TranslationRecordInfo getNextRecordInfo()
        {
            if (position + 1 < records.size())
            {
                ++position;
                auto const &record = records[position];

                return {
                        record.get(),
                        record->source.c_str(),
                        record->target.c_str(),
                        record->contextName.c_str(),
                        record->contextId.c_str(),
                        record->meta.c_str(),
                        record->translationIndex,
                        record->translationType,
                        record->maxLength
                };
            }

            return { };
        }

        void reset()
        {
            position = -1;
        }

        void clear()
        {
            records.clear();
        }

        void populate();

        void applyChanges();

        static std::string const &getLastError()
        {
            return lastError;
        }

        static void const clearLastError()
        {
            lastError.clear();
        }

        static std::string setLastError(std::string const &error)
        {
            return lastError;
        }

        void setLastWriteTime(std::filesystem::file_time_type const &time)
        {
            lastWriteTime = time;
        }

        std::filesystem::file_time_type getLastWriteTime() const
        {
            return lastWriteTime;
        }
    };
}

MWTEXTLIB_API mwtextlib::TranslationRecordInfo
__cdecl TranslationState_GetNextRecordInfo(mwtextlib::TranslationState *state);
MWTEXTLIB_API void __cdecl TranslationState_Reset(mwtextlib::TranslationState *state);
MWTEXTLIB_API void __cdecl TranslationState_Dispose(mwtextlib::TranslationState *state);

#endif //MWTEXTLIB_TRANSLATIONSTATE_H
