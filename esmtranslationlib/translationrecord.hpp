#ifndef ESMTRANSLATIONLIB_TRANSLATIONRECORD_H
#define ESMTRANSLATIONLIB_TRANSLATIONRECORD_H

#include <string>
#include <esmdefines.hpp>
#include <apps/esmtool/record.hpp>
#include <iostream>
#include "textinfo.hpp"

namespace EsmTranslationLib
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
        TranslationRecord(int index, EsmTranslationLib::TextInfo &info, std::string const &contextName,
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

ESMTRANSLATIONLIB_API const char *__cdecl TranslationRecord_GetSource(EsmTranslationLib::TranslationRecord *);
ESMTRANSLATIONLIB_API const char *__cdecl TranslationRecord_GetTarget(EsmTranslationLib::TranslationRecord *);
ESMTRANSLATIONLIB_API const char *__cdecl TranslationRecord_GetContextName(EsmTranslationLib::TranslationRecord *);
ESMTRANSLATIONLIB_API const char *__cdecl TranslationRecord_GetContextId(EsmTranslationLib::TranslationRecord *);
ESMTRANSLATIONLIB_API const char *__cdecl TranslationRecord_GetMeta(EsmTranslationLib::TranslationRecord *);

ESMTRANSLATIONLIB_API const char *__cdecl TranslationRecord_SetTarget(EsmTranslationLib::TranslationRecord *, const char *);

#endif //ESMTRANSLATIONLIB_TRANSLATIONRECORD_H
