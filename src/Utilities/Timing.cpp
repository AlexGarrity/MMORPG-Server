#include "Timing.h"

float Timing::deltaTime = 0;
double Timing::elapsedTime = 0;

sf::Clock Timing::clock;

void Timing::Update()
{
    deltaTime = clock.restart().asSeconds();
    elapsedTime += deltaTime;
}

long Timing::GetElapsedTime()
{
    return (long)elapsedTime;
}
