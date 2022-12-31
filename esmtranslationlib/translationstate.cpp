#include "translationstate.hpp"
#include "texts.hpp"
#include <nlohmann/json.hpp>

using namespace EsmTranslationLib;

std::string TranslationState::lastError;

TranslationRecordInfo TranslationState_GetNextRecordInfo(TranslationState *state)
{
    TranslationState::clearLastError();

    return state->getNextRecordInfo();
}

void TranslationState_Reset(TranslationState *state)
{
    TranslationState::clearLastError();

    state->reset();
}

void TranslationState_Dispose(TranslationState *state)
{
    TranslationState::clearLastError();

    state->clear();
    delete state;
}

void TranslationState::populate()
{
    for (auto &record: data.mRecords)
    {
        auto texts = collectTexts(*record.get());

        addTexts(*record, texts);
    }
}

void TranslationState::addTexts(EsmTool::RecordBase &record, CollectedTexts &texts, std::string const &contextName,
                                std::string const &contextId)
{
    int counter = 0;
    for (auto &textInfo: texts)
    {
        if (textInfo.holder->get().size() > 0)
        {
            auto uniqId = contextId.empty() ? getUniqId(record) : contextId;
            Add(std::make_unique<TranslationRecord>(counter, textInfo,
                                                    contextName.empty() ? record.getType().toString() : contextName,
                                                    uniqId));
        }
        ++counter;
    }
}

void TranslationState::applyChanges()
{
    for (auto &record: records)
    {
        record->ApplyChanges();
    }
}
