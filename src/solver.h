#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "state.h"

class Solver
{
    private:
        static int m_height;
        static int m_objective_power;
        static int m_num_turbines;

        static void setHeight(int height) { m_height = height; };
        static void setObjectivePower(int power) { m_objective_power = power; };
        static void setNumTurbines(int num) { m_num_turbines = num; };
        
    public:
        Solver(int height, int power, int num_turbines);
        
        static int getHeight() { return m_height; };
        static int getObjectivePower() { return m_objective_power; };
        static int getNumTurbines() { return m_num_turbines; };

        void run();
        State *simulatedAnnealing(State *e);
};

#endif
