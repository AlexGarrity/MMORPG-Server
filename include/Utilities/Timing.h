#ifndef TIMING_H
#define TIMING_H

#include <SFML/System/Clock.hpp>

class Timing
{
public:
    static void Update();

    static float deltaTime;

    static long GetElapsedTime();

protected:

private:
    static sf::Clock clock;
    static double elapsedTime;
};

#endif // TIMING_H
