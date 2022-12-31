#ifndef ESMTRANSLATIONLIB_TEXTHOLDER_H
#define ESMTRANSLATIONLIB_TEXTHOLDER_H

#include <string>

namespace EsmTranslationLib
{
    class TextHolderBase
    {
    public:
        virtual ~TextHolderBase()
        {
        }

        virtual std::string get() = 0;

        virtual void set(std::string const &value) = 0;
    };

    template <class T>
    class TextHolder : public TextHolderBase
    {
    public:
        TextHolder(std::reference_wrapper<T> value): value(value)
        {
        }

        std::string get() override;

        void set(std::string const &value) override;

        std::reference_wrapper<T> value;
    };
}

#endif //ESMTRANSLATIONLIB_TEXTHOLDER_H
