#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

#include "solver.h"

using namespace std;

int Solver::m_height;
int Solver::m_objective_power;
int Solver::m_num_turbines;

Solver::Solver(int height, int power, int turbines)
{
    Solver::setHeight(height);
    Solver::setObjectivePower(power);
    Solver::setNumTurbines(turbines);
}

#ifdef _BRUTE_
void Solver::run()
{
    State *e = new State();

    State *maxe = new State();
    float max = 0.0f;

    e->bruteForce(Powers::getSize()-1, getObjectivePower(), getNumTurbines(), &maxe, &max);

    maxe->print();

    delete maxe;
    delete e;
}
#else
void Solver::run()
{
    int obj = getObjectivePower();
    int turb = getNumTurbines();

    float max = 0.0f;
    State *maxe = new State();
    State *e = new State();

    while ((Powers::getMaximumPower() * turb) >= obj)
    {
        e = new State();
        
        if (e->bruteForceOne(Powers::getSize()-1, obj, turb))
        {
            e = this->simulatedAnnealing(e);
            if (e->getEfficiency() > max)
            {
                max = e->getEfficiency();
                e->copyTo(maxe);
            }
        }

        delete e;
        turb--;
    }

    if (maxe)
       maxe->print();
}
#endif

State *Solver::simulatedAnnealing(State *e)
{
    float delta, max, nefi;
    State *ne, *maxe;

    max = e->getEfficiency();
    maxe = new State();
    e->copyTo(maxe);

    float temp = 0.1f;
    int amplitude = Powers::getSize() / 8;
    int i = 128;
    int bigi = i;
    while (amplitude > 0)
    {
        ne = e->getNextState(amplitude);

        if (ne == NULL)
            break;
        
        nefi = ne->getEfficiency();

        delta = nefi - e->getEfficiency();

        if ((delta > 0) || ( expf(delta/temp) > ((float)rand()/RAND_MAX) ))
        {
            if (nefi > max)
            {
                max = nefi;
                ne->copyTo(maxe);
            }

            ne->copyTo(e);
        }

        delete ne;
        
        temp = 0.95f * temp;

        i--;
        if (i <= 0)
        {
            amplitude--;
            i = bigi + (bigi / 4);
            bigi = i;
        }
    }

    delete e;

    return maxe;
}
