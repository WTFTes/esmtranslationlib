#include "textholder.hpp"
#include "components/esm/refid.hpp"
#include <components/esm3/variant.hpp>

using namespace mwtextlib;

template <>
void TextHolder<std::string>::set(std::string const &value)
{
    this->value.get() = value;
}

template <>
std::string TextHolder<std::string>::get()
{
    return value.get();
}

template <>
void TextHolder<ESM::Variant>::set(std::string const &value)
{
    if (this->value.get().getType() == ESM::VarType::VT_String)
        this->value.get().setString(value);
}

template <>
std::string TextHolder<ESM::Variant>::get()
{
    if (this->value.get().getType() == ESM::VarType::VT_String)
        return this->value.get().getString();

    return "";
}

template <>
void TextHolder<ESM::RefId>::set(std::string const &value)
{
    this->value.get() = ESM::RefId::stringRefId(value);
}

template <>
std::string TextHolder<ESM::RefId>::get()
{
    return this->value.get().getRefIdString();
}
