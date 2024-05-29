#include "log.hpp"

namespace WebCpp
{
    void Log::setLevel(logLevel n) noexcept
    {
        this->level = n;
    }

    void Log::log(std::string_view l) noexcept
    {
        std::ostringstream log;
        if (this->level == logLevel::LogLevelLog)
        {
            log << "[LOG] ";
            log << l;
            this->msg = log.str();
            this->print();
        }
    }

    void Log::warn(std::string_view w) noexcept
    {
        std::ostringstream warn;
        if (this->level <= logLevel::LogLevelLog && this->level > logLevel::LogLevelError)
        {
            warn << "[WARN] ";
            warn << w;
            this->msg = warn.str();
            this->print();
        }
    }

    void Log::error(std::string_view e) noexcept
    {
        std::ostringstream error;

        error << "[ERROR] ";
        error << e;
        this->msg = error.str();
        this->print();
    }

    

}