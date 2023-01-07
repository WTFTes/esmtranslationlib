#include "translationrecord.hpp"
#include "translationstate.hpp"

using namespace mwtextlib;

const char *TranslationRecord_GetSource(TranslationRecord *record)
{
    TranslationState::clearLastError();

    return record->source.c_str();
}

const char *TranslationRecord_GetTarget(TranslationRecord *record)
{
    TranslationState::clearLastError();

    return record->target.c_str();
}

const char *TranslationRecord_GetContextName(TranslationRecord *record)
{
    TranslationState::clearLastError();

    return record->contextName.c_str();
}

const char *TranslationRecord_GetContextId(TranslationRecord *record)
{
    TranslationState::clearLastError();

    return record->contextId.c_str();
}

const char *TranslationRecord_GetMeta(TranslationRecord *record)
{
    TranslationState::clearLastError();

    return record->meta.c_str();
}

void TranslationRecord_SetSource(TranslationRecord *record, const char *value)
{
    TranslationState::clearLastError();

    record->source = value;
}

const char *TranslationRecord_SetTarget(TranslationRecord *record, const char *value)
{
    TranslationState::clearLastError();

    record->target = value;
    return record->target.c_str();
}
