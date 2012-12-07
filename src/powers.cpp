#include "powers.h"

vector<power_t> Powers::m_powers;

Powers::Powers(bool reverse)
{
    this->m_reverse = reverse;
    this->reset();
}

void Powers::reset()
{
    if (this->m_reverse)
        this->m_currentIndex = Powers::getSize() - 1;
    else
        this->m_currentIndex = 0;
}

bool Powers::next()
{
    if (this->m_reverse)
    {
        if (this->m_currentIndex <= 0)
            return false;

        this->m_currentIndex--;
    }
    else
    {
        if ((this->m_currentIndex+1) >= Powers::getSize())
            return false;

        this->m_currentIndex++;
    }
    
    return true;
}

int Powers::peek()
{
    if (this->isLast())
        return 0;

    if (this->m_reverse)
        return Powers::m_powers[this->m_currentIndex-1].power;
    else
        return Powers::m_powers[this->m_currentIndex+1].power;
}

bool Powers::isLast()
{
    if (this->m_reverse)
        return (this->m_currentIndex <= 0) ? true : false;
    else
        return ((this->m_currentIndex+1) >= Powers::getSize()) ? true : false;
}

bool Powers::setItem(int pos, int power, float effi)
{
    if (pos >= Powers::getSize())
        return false;

    Powers::m_powers[pos].power = power;
    Powers::m_powers[pos].efficiency = effi;
    
    return true;
}

int Powers::getItemPower(int pos)
{
    return Powers::m_powers[pos].power;
}

float Powers::getItemEfficiency(int pos)
{
    return Powers::m_powers[pos].efficiency;
}

int Powers::getMinimumPower()
{
    return Powers::m_powers[0].power;
}

int Powers::getMaximumPower()
{
    return Powers::m_powers[Powers::getSize() - 1].power;
}
