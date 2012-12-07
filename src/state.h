#ifndef _STATE_H_
#define _STATE_H_

#include <vector>
//#include "solver.h"
#include "powers.h"

using namespace std;

typedef int *state_vector_t;

class Powers;

class State
{
    private:

        state_vector_t m_state_vector;

        state_vector_t getStateVector() { return this->m_state_vector; };
        

    public:
        State();
        ~State();
        void copyTo(State *e);

        bool distributePower(int index, int powerLeft, int turbinesLeft);
        static State *getInitialState();
        State *getNextState(int iterations = 1);

        void bruteForce(int index, int powerLeft, int turbinesLeft, State **maxe, float *max);
        bool bruteForceOne(int index, int powerLeft, int turbinesLeft);

        bool fix(int howmuch, int excludeIndex);

        int getItem(int pos);
        void setItem(int pos, int value);

        void print();
        int getTotalPower();
        float getEfficiency();
        //bool isValid();
};

#endif
