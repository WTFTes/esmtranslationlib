#include "texts.hpp"
#include "translationrecord.hpp"
#include <nlohmann/json.hpp>
#include <components/misc/strings/format.hpp>

namespace mwtextlib
{
    template <>
    std::string getUniqId(ESM::Cell &record)
    {
        //return left.mWorldspace == right.mWorldspace && left.mPaged == right.mPaged
        //       && (!left.mPaged || (left.mIndex.mX == right.mIndex.mX && left.mIndex.mY == right.mIndex.mY));
        return Misc::StringUtils::format("%s_%u_%i_%i", record.mCellId.mWorldspace.getRefIdString().c_str(),
                                         record.mCellId.mPaged ? 1 : 0,
                                         record.mCellId.mPaged ? record.mCellId.mIndex.mX : 0,
                                         record.mCellId.mPaged ? record.mCellId.mIndex.mY : 0);
    }

    template <>
    std::string getUniqId(ESM::DialInfo &record)
    {
        return Misc::StringUtils::format("%s_%s_%s",
                                         record.mId.getRefIdString(),
                                         record.mPrev.empty() ? "EMPTY" : record.mPrev.getRefIdString(),
                                         record.mNext.empty() ? "EMPTY" : record.mNext.getRefIdString());
    }

    template <>
    std::string getUniqId(ESM::Skill &record)
    {
        return record.mId.getRefIdString();
    }

    template <>
    std::string getUniqId(ESM::MagicEffect &record)
    {
        return record.mId.getRefIdString();
    }

    template <>
    std::string getUniqId(EsmTool::RecordBase &record)
    {
        switch (record.getType().toInt())
        {
            case ESM::REC_CELL:
                return getUniqId(record.cast<ESM::Cell>()->get());
            case ESM::REC_INFO:
                return getUniqId(record.cast<ESM::DialInfo>()->get());
            case ESM::REC_SKIL:
                return getUniqId(record.cast<ESM::Skill>()->get());
            case ESM::REC_MGEF:
                return getUniqId(record.cast<ESM::MagicEffect>()->get());
            default:
                break;
        }

        return record.getId();
    }

    template <>
    CollectedTexts collectTexts(ESM::Activator &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Potion &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Armor &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Apparatus &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::BodyPart &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::Book &record)
    {
        return {
                { TextType_Text, std::ref(record.mName) },
                { TextType_Html, std::ref(record.mText) }
        };
    }

    template <>
    CollectedTexts collectTexts(ESM::BirthSign &record)
    {
        return {
                { TextType_Text, std::ref(record.mName) },
                { TextType_Text, std::ref(record.mDescription) }
        };
    }

    template <>
    CollectedTexts collectTexts(ESM::Cell &record)
    {
        return {
                { TextType_Text, std::ref(record.mName) },
                { TextType_Text, std::ref(record.mRegion) },
        };
    }

    template <>
    CollectedTexts collectTexts(ESM::Class &record)
    {
        return {
                { TextType_Text, std::ref(record.mName) },
                { TextType_Text, std::ref(record.mDescription) }
        };
    }

    template <>
    CollectedTexts collectTexts(ESM::Clothing &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Container &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Creature &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Dialogue &record)
    {
        switch (record.mType)
        {
            case ESM::Dialogue::Topic: // "контракты Горнодобывающей Компании Кальдеры"
                return {{ TextType_Text, std::ref(record.mId) }};
            case ESM::Dialogue::Voice: // "Hit" "Alarm"
            case ESM::Dialogue::Greeting: // "Greeting 0"
            case ESM::Dialogue::Persuasion: // "Intimidate Success"
            case ESM::Dialogue::Journal: // "A1_6_AddhiranirrInformant"
            default:
                break;
        }

        return { };
    }

    template <>
    CollectedTexts collectTexts(ESM::Door &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Enchantment &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::Faction &record)
    {
        CollectedTexts texts = {{ TextType_Text, std::ref(record.mName) }};
        for (auto& mRank : record.mRanks)
            texts.emplace_back(TextType_Text, std::ref(mRank), 32);

        return texts;
    }

    template <>
    CollectedTexts collectTexts(ESM::Global &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::GameSetting &record)
    {
        if (record.mValue.getType() == ESM::VT_String)
            return {{ TextType_Text, std::ref(record.mValue) }};

        return { };
    }

    template <>
    CollectedTexts collectTexts(ESM::DialInfo &record)
    {
        nlohmann::json metaJson = {
                { "dialogue",      record.mDialogue->mId.getRefIdString() },
                { "dialogue_type", (int) record.mDialogue->mType },
                { "prev_id",       record.mPrev.getRefIdString() },
                { "next_id",       record.mNext.getRefIdString() },
                { "script",        record.mResultScript }
        };

        auto meta = metaJson.dump();
        return {
                { TextType_Text,   std::ref(record.mResponse),     0, meta },
                { TextType_Script, std::ref(record.mResultScript), 0, meta },
        };
    }

    template <>
    CollectedTexts collectTexts(ESM::Ingredient &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Land &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::CreatureLevList &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::ItemLevList &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::Light &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Lockpick &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Probe &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Repair &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::LandTexture &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::MagicEffect &record)
    {
        return {{ TextType_Text, std::ref(record.mDescription) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Miscellaneous &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::NPC &record)
    {
        nlohmann::json json = {
                { "race", record.mRace.getRefIdString() }
        };
        return {{ TextType_Text, std::ref(record.mName), 0, json.dump() }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Pathgrid &record)
    {
        return {{ TextType_Text, std::ref(record.mCell) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Race &record)
    {
        return {
                { TextType_Text, std::ref(record.mName) },
                { TextType_Text, std::ref(record.mDescription) }
        };
    }

    template <>
    CollectedTexts collectTexts(ESM::Region &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Script &record)
    {
        return {{ TextType_Script, std::ref(record.mScriptText) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::Skill &record)
    {
        return {{ TextType_Text, std::ref(record.mDescription) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::SoundGenerator &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::Sound &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::Spell &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(ESM::StartScript &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::Static &record)
    {
        return { }; // empty
    }

    template <>
    CollectedTexts collectTexts(ESM::Weapon &record)
    {
        return {{ TextType_Text, std::ref(record.mName) }};
    }

    template <>
    CollectedTexts collectTexts(EsmTool::RecordBase &record)
    {
        switch (record.getType().toInt())
        {
            case ESM::REC_ACTI:
                return collectTexts(record.cast<ESM::Activator>()->get());
            case ESM::REC_ALCH:
                return collectTexts(record.cast<ESM::Potion>()->get());
            case ESM::REC_ARMO:
                return collectTexts(record.cast<ESM::Armor>()->get());
            case ESM::REC_APPA:
                return collectTexts(record.cast<ESM::Apparatus>()->get());
            case ESM::REC_BODY:
                return collectTexts(record.cast<ESM::BodyPart>()->get());
            case ESM::REC_BOOK:
                return collectTexts(record.cast<ESM::Book>()->get());
            case ESM::REC_BSGN:
                return collectTexts(record.cast<ESM::BirthSign>()->get());
            case ESM::REC_CELL:
                return collectTexts(record.cast<ESM::Cell>()->get());
            case ESM::REC_CLAS:
                return collectTexts(record.cast<ESM::Class>()->get());
            case ESM::REC_CLOT:
                return collectTexts(record.cast<ESM::Clothing>()->get());
            case ESM::REC_CONT:
                return collectTexts(record.cast<ESM::Container>()->get());
            case ESM::REC_CREA:
                return collectTexts(record.cast<ESM::Creature>()->get());
            case ESM::REC_DIAL:
                return collectTexts(record.cast<ESM::Dialogue>()->get());
            case ESM::REC_DOOR:
                return collectTexts(record.cast<ESM::Door>()->get());
            case ESM::REC_ENCH:
                return collectTexts(record.cast<ESM::Enchantment>()->get());
            case ESM::REC_FACT:
                return collectTexts(record.cast<ESM::Faction>()->get());
            case ESM::REC_GLOB:
                return collectTexts(record.cast<ESM::Global>()->get());
            case ESM::REC_GMST:
                return collectTexts(record.cast<ESM::GameSetting>()->get());
            case ESM::REC_INFO:
                return collectTexts(record.cast<ESM::DialInfo>()->get());
            case ESM::REC_INGR:
                return collectTexts(record.cast<ESM::Ingredient>()->get());
            case ESM::REC_LAND:
                return collectTexts(record.cast<ESM::Land>()->get());
            case ESM::REC_LEVC:
                return collectTexts(record.cast<ESM::CreatureLevList>()->get());
            case ESM::REC_LEVI:
                return collectTexts(record.cast<ESM::ItemLevList>()->get());
            case ESM::REC_LIGH:
                return collectTexts(record.cast<ESM::Light>()->get());
            case ESM::REC_LOCK:
                return collectTexts(record.cast<ESM::Lockpick>()->get());
            case ESM::REC_PROB:
                return collectTexts(record.cast<ESM::Probe>()->get());
            case ESM::REC_REPA:
                return collectTexts(record.cast<ESM::Repair>()->get());
            case ESM::REC_LTEX:
                return collectTexts(record.cast<ESM::LandTexture>()->get());
            case ESM::REC_MGEF:
                return collectTexts(record.cast<ESM::MagicEffect>()->get());
            case ESM::REC_MISC:
                return collectTexts(record.cast<ESM::Miscellaneous>()->get());
            case ESM::REC_NPC_:
                return collectTexts(record.cast<ESM::NPC>()->get());
            case ESM::REC_PGRD:
                return collectTexts(record.cast<ESM::Pathgrid>()->get());
            case ESM::REC_RACE:
                return collectTexts(record.cast<ESM::Race>()->get());
            case ESM::REC_REGN:
                return collectTexts(record.cast<ESM::Region>()->get());
            case ESM::REC_SCPT:
                return collectTexts(record.cast<ESM::Script>()->get());
            case ESM::REC_SKIL:
                return collectTexts(record.cast<ESM::Skill>()->get());
            case ESM::REC_SNDG:
                return collectTexts(record.cast<ESM::SoundGenerator>()->get());
            case ESM::REC_SOUN:
                return collectTexts(record.cast<ESM::Sound>()->get());
            case ESM::REC_SPEL:
                return collectTexts(record.cast<ESM::Spell>()->get());
            case ESM::REC_SSCR:
                return collectTexts(record.cast<ESM::StartScript>()->get());
            case ESM::REC_STAT:
                return collectTexts(record.cast<ESM::Static>()->get());
            case ESM::REC_WEAP:
                return collectTexts(record.cast<ESM::Weapon>()->get());
        }

        return { };
    }
}