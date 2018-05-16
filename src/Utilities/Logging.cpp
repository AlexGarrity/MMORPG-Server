#include "Logging.h"

Logging::Logging()
{
    //ctor
}

Logging::~Logging()
{
    //dtor
}

void Logging::Log (std::string message, Severity s = Severity::Info)
{
            std::cout << "(" << ISOTime() << ") - \t" << message << std::endl;
}

std::string Logging::ISOTime()
{
    long time = Timing::GetElapsedTime();
    unsigned int seconds = time % 60;
    unsigned int minutes = time / 60;
    unsigned int hours = time / 3600;
    return (FormatValue(hours) + ":" + FormatValue(minutes) + ":" + FormatValue(seconds));
}

std::string Logging::FormatValue(unsigned int value)
{
    std::string s;
    s = std::to_string(value);
    if (s.size() == 1) {
        s.insert(0, "0" , 1);
    }
    return s;
}
