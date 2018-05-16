#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <string>

#include "Timing.h"

class Logging
{
public:
    Logging();
    virtual ~Logging();

    static enum class Severity {Debug, Info, Important};

    static void Log (std::string message, Severity s);
protected:

private:
    static std::string ISOTime();
    static std::string FormatValue(unsigned int value);
};

#endif // LOGGING_H
