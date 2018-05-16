#ifndef STATS_H
#define STATS_H

struct Stat
{
    float value;
    float maxValue;

    Stat() {}
    Stat (float v, float mV)
    {
        SetStat(v, mV);
    }

    void SetStat(float v, float mV = 0) {
        value = v;
        if (mV == 0) {
            maxValue = v;
        } else {
            maxValue = mV;
        }
    }
};

struct Stats {
    Stat health;
    Stat magic;
    Stat stamina;
};

#endif // STATS_H
