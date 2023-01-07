#ifndef MWTEXTLIB_TEXTINFO_H
#define MWTEXTLIB_TEXTINFO_H

#include <list>
#include <string>
#include "textholder.hpp"
#include <memory>

namespace mwtextlib
{
    class TextInfo
    {
    public:
        template <class T>
        TextInfo(int textType, std::reference_wrapper<T> value, int maxLength = 0, std::string const &metaJson = "") :
                textType(textType), maxLength(maxLength), metaJson(metaJson)
        {
            holder = std::make_shared<TextHolder<T>>(value);
        }

        int textType;
        std::shared_ptr<TextHolderBase> holder;
        std::string metaJson;
        int maxLength;
    };

    typedef std::list<TextInfo> CollectedTexts;
}

#endif //MWTEXTLIB_TEXTINFO_H
