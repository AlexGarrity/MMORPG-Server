#ifndef STATS_H
#define STATS_H

class Stat
{
private:
    float value;
    float maxValue;

public:
    Stat();
    Stat (float v, float mV);
};

class Stats
{
public:
    Stats();
    virtual ~Stats();

protected:

private:
    Stat health;
    Stat magic;
    Stat stamina;
};

#endif // STATS_H
