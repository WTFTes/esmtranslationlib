#ifndef MWTEXTLIB_TRANSLATIONRECORD_H
#define MWTEXTLIB_TRANSLATIONRECORD_H

#include <string>
#include <esmdefines.hpp>
#include <apps/esmtool/record.hpp>
#include <iostream>
#include "textinfo.hpp"

namespace mwtextlib
{
    enum TextType
    {
        TextType_Text = 0,
        TextType_Html,
        TextType_Script
    };

    class TranslationState;

    struct TranslationRecord
    {
        TranslationRecord(int index, mwtextlib::TextInfo &info, std::string const &contextName,
                          std::string const &contextId):
                holder(info.holder)
        {
            this->translationIndex = index;
            this->translationType = info.textType;
            this->source = info.holder.get()->get();
            this->target = info.holder.get()->get();
            this->maxLength = info.maxLength;
            this->contextName = contextName;
            this->contextId = contextId;
            this->meta = info.metaJson;
        }

        void ApplyChanges()
        {
            if (source != target || source != holder->get())
                holder->set(target);
        }

        std::string source;
        std::string target;
        std::string contextName;
        std::string contextId;
        std::string meta;
        int translationIndex;
        int translationType;
        int maxLength;
        TranslationState *state;
        std::shared_ptr<TextHolderBase> holder;
    };
}

MWTEXTLIB_API const char *__cdecl TranslationRecord_GetSource(mwtextlib::TranslationRecord *);
MWTEXTLIB_API const char *__cdecl TranslationRecord_GetTarget(mwtextlib::TranslationRecord *);
MWTEXTLIB_API const char *__cdecl TranslationRecord_GetContextName(mwtextlib::TranslationRecord *);
MWTEXTLIB_API const char *__cdecl TranslationRecord_GetContextId(mwtextlib::TranslationRecord *);
MWTEXTLIB_API const char *__cdecl TranslationRecord_GetMeta(mwtextlib::TranslationRecord *);

MWTEXTLIB_API const char *__cdecl TranslationRecord_SetTarget(mwtextlib::TranslationRecord *, const char *);

#endif //MWTEXTLIB_TRANSLATIONRECORD_H
