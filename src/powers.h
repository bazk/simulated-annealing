#ifndef _POWERS_H_
#define _POWERS_H_

#include <vector>

using namespace std;

typedef struct 
{
    int power;
    float efficiency;
} power_t;

class Powers
{
    private:
        static vector<power_t> m_powers;

        int m_currentIndex;
        bool m_reverse;

    public:
        static void setSize(int size) { m_powers.resize(size); };
        static int getSize() { return m_powers.size(); };

        static bool setItem(int pos, int power, float effi);
        static int getItemPower(int pos);
        static float getItemEfficiency(int pos);

        static int getMinimumPower();
        static int getMaximumPower();

        static int getHighestPower() { return m_powers.back().power; };
        static int getLowestPower() { return m_powers.front().power; };

        Powers(bool reverse = false);
        void reset();
        bool next();
        int peek();
        int index() { return this->m_currentIndex; };
        void setIndex(int i) { this->m_currentIndex = i; };
        bool isLast();

        int getPower() { return Powers::m_powers[this->m_currentIndex].power; };
        float getEfficiency() { return Powers::m_powers[this->m_currentIndex].efficiency; };
};

#endif
