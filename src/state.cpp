#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

#include "state.h"

using namespace std;

State::State()
{
    this->m_state_vector = (int*) malloc(Powers::getSize() * sizeof(int));
    for (int i = 0; i < Powers::getSize(); i++)
        this->m_state_vector[i] = 0;
}

State::~State()
{
    free(this->m_state_vector);
}

void State::copyTo(State *e)
{
    if (e == NULL)
        return;

    memcpy( e->m_state_vector, // destination
            this->m_state_vector, // source
            Powers::getSize() * sizeof(int));
}

int State::getItem(int pos)
{
    return this->getStateVector()[pos];
}

void State::setItem(int pos, int value)
{
    this->getStateVector()[pos] = value;
}

void State::print()
{
    Powers p(true);
    
    int mul_sum = 0;
    int pow_sum = 0;
    float efi_sum = 0.0f;

    for (int i = 0; i < Powers::getSize(); i++)
    {
        mul_sum += this->getItem(i);
        pow_sum += this->getItem(i) * Powers::getItemPower(i);
        efi_sum += this->getItem(i) * Powers::getItemPower(i) * Powers::getItemEfficiency(i);
    }

    efi_sum /= pow_sum;

    cout << "[ " << mul_sum << " " << pow_sum << " " << fixed << setprecision(4) << efi_sum << " ]  =>  ";

    do
    {
        if (this->getItem(p.index()) != 0)
            cout << this->getItem(p.index()) << "*" << p.getPower() << "  ";
    }
    while (p.next());
    
    cout << endl;
}

int State::getTotalPower()
{
    int i, sum = 0;

    for (i = 0; i < Powers::getSize(); i++)
        sum += this->getItem(i) * Powers::getItemPower(i);

    return sum;
}

float State::getEfficiency()
{
    int i;
    float sum = 0.0f;

    for (i = 0; i < Powers::getSize(); i++)
        sum += this->getItem(i) * Powers::getItemPower(i) * Powers::getItemEfficiency(i);

    sum /= this->getTotalPower();

    return sum;
}

bool State::distributePower(int index, int powerLeft, int turbinesLeft)
{
#ifdef _DEBUG_
    //cout << "[DEBUG] Call: distributePower(index = " << index << ", powerLeft = " << powerLeft << ", turbinesLeft = " << turbinesLeft << ")" << endl;
#endif
    
    if (powerLeft == 0)
        return true;

    if ((index == 0) || (turbinesLeft == 0))
        return false;

    int power = Powers::getItemPower(index);

    if (power <= powerLeft)
    {
        int mult = powerLeft / power;

        if (mult > turbinesLeft)
            return false;

        this->setItem(index, mult);
        powerLeft %= power;
        turbinesLeft -= mult;
        
        if (powerLeft == 0)
            return true;

        if (index == 0)
            return false;

        if (powerLeft < Powers::getItemPower(index-1))
        {
            mult--;
            this->setItem(index, mult);
            powerLeft += power;
            turbinesLeft++;
        }
    }
    else
        this->setItem(index, 0);

    return distributePower(index-1, powerLeft, turbinesLeft);
}

State *State::getInitialState()
{
    State *e = new State();

//    if (!e->distributePower(Powers::getSize() - 1, Solver::getObjectivePower(), Solver::getNumTurbines()))
  //  {
        delete e;
        return NULL;
    //}

    return e;
}

State *State::getNextState(int iterations)
{
    State *e = new State();
    this->copyTo(e);

    bool increase;
    int index;
    int howmuch = 0;

    int n = 0;
    while (n < 100)
    {
        index = rand() % Powers::getSize();

        if (e->getItem(index) == 0)
            continue;

        if (index < iterations)
            increase = true;
        else if (index > ((Powers::getSize() - 1) - iterations))
            increase = false;
        else
            increase = ((rand()%2) == 0);

        e->setItem(index, e->getItem(index) - 1);
        if (increase)
        {
            e->setItem(index+iterations, e->getItem(index+iterations)+1);
            howmuch = Powers::getItemPower(index) - Powers::getItemPower(index+iterations);
        }
        else
        {
            e->setItem(index-iterations, e->getItem(index-iterations)+1);
            howmuch = Powers::getItemPower(index) - Powers::getItemPower(index-iterations);
        }

        if (e->fix(howmuch, index))
            return e;
        else
        {
            e->setItem(index, e->getItem(index) + 1);
            if (increase)
                e->setItem(index+iterations, e->getItem(index+iterations)-1);
            else
                e->setItem(index-iterations, e->getItem(index-iterations)-1);
        }

        n++;
    }

    delete e;
    return NULL;
}

bool State::fix(int howmuch, int excludeIndex)
{
    if (howmuch == 0)
        return true;

    for (int index = Powers::getSize()-1; index >= 0; index--)
    {
        if (this->getItem(index) == 0)
            continue;

        int power = Powers::getItemPower(index);
        int desl = (howmuch > 0) ? 1 : -1;

        int miss = 0;

        while ( ((index+desl) < Powers::getSize()) && ((index+desl) >= 0) && (miss <= 10) )
        {
            if ((index+desl) != excludeIndex)
            {
                int newpower = Powers::getItemPower(index+desl);
    
                if ((newpower - power) == howmuch)
                {
                    this->setItem(index, this->getItem(index) - 1);
                    this->setItem(index+desl, this->getItem(index+desl) + 1);
                    return true;
                }
            }
            else
            {
                miss++;
            }
    
            if (howmuch > 0)
                desl++;
            else
                desl--;
        }
    }

    return false;
}


void State::bruteForce(int index, int powerLeft, int turbinesLeft, State **maxe, float *max)
{
    if ((turbinesLeft < 0) || (index < 0))
        return;

    int power = Powers::getItemPower(index);

    State *ne = new State();
    this->copyTo(ne);

    for (int i = turbinesLeft; i >= 0; i--)
    {
        ne->setItem(index, i);

        if ((powerLeft - (i * power)) == 0)
        {
            if (ne->getEfficiency() > (*max))
            {
                (*max) = ne->getEfficiency();
                ne->copyTo(*maxe);
            }
        }
        else
            ne->bruteForce(index-1, powerLeft - (i*power), turbinesLeft - i, maxe, max);
    }

    delete ne;
}

bool State::bruteForceOne(int index, int powerLeft, int turbinesLeft)
{
    if ((turbinesLeft < 0) || (index < 0))
        return false;

    int power = Powers::getItemPower(index);

    for (int i = turbinesLeft; i >= 0; i--)
    {
        this->setItem(index, i);

        if ((powerLeft - (i * power)) == 0)
        {
            if ((turbinesLeft - i) == 0)
                return true;
        }
        else
        {
            if (this->bruteForceOne(index-1, powerLeft - (i*power), turbinesLeft - i))
                return true;
        }
    }

    return false;
}
