#include "generator.hpp"
#include "loader.hpp"
#include "writer.hpp"
#include <fstream>

using namespace mwtextlib;

TranslationState *Translation_GetTexts(const wchar_t *fileName, const char *encoding)
{
    TranslationState::clearLastError();

#ifdef MWTEXTLIB_EXPORT
    Log::SetBuffer(std::make_shared<std::fstream>("debug.log", std::fstream::out | std::fstream::app));
#endif

    Arguments info;
    info.filename = fileName;
    info.encoding = encoding;
    info.mode = "clone";
    info.log_actions = true;
    if (info.encoding.empty())
        info.encoding = "win1251";

    auto state = new TranslationState();

    try
    {
        if (load(info, &state->getData(), true) != 0)
        {
            std::string error = "Open failed";
            Log(Debug::Error) << error;
            throw std::exception(error.c_str());
        }
    }
    catch (std::exception const &e)
    {
        TranslationState::setLastError(e.what());
        delete state;
        return nullptr;
    }

    state->populate();

    state->setLastWriteTime(std::filesystem::last_write_time(info.filename));

    return state;
}

int Translation_Save(TranslationState *state, const wchar_t *fileName, const char *encoding)
{
    TranslationState::clearLastError();

    Arguments info;
    info.outname = fileName;
    info.encoding = encoding;

    try
    {
        state->applyChanges();

        auto res = write(info, state->getData(), true);
        if (res == 0)
        {
            auto writeTime = state->getLastWriteTime();
            std::filesystem::last_write_time(info.outname, writeTime);
        }

        return res;
    }
    catch (std::exception const &e)
    {
        Log(Debug::Error) << "ERROR: " << e.what();
        TranslationState::setLastError(e.what());
    }

    return 0;
}

char const *Translation_GetLastError()
{
    return TranslationState::getLastError().c_str();
}
