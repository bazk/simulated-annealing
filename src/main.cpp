#include <iostream>
#include <time.h>
#include <cstdlib>
#include <cstdio>

#include "powers.h"
#include "solver.h"

using namespace std;

void parse_input(int height);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Numero errado de argumentos!" << endl;
        return -1;
    }

    srand(time(NULL));
    
    int height = atoi(argv[1]);
    int total_power = atoi(argv[2]);
    int num_turbines; scanf("%d", &num_turbines);

    parse_input(height);

#ifdef _DEBUG_
/*    printf("[DEBUG] CURVA COLINA:\n");

    Powers p;
    do
        printf("[DEBUG] %d\t%.3f\n", p.getPower(), p.getEfficiency());
    while (p.next());

    printf("[DEBUG]\n\n");*/
#endif

    Solver *solver = new Solver(height, total_power, num_turbines);

    solver->run();

    delete solver;

    return 0;
}

void parse_input(int height)
{
    int num_powers, num_heights;
    int tmp_height, height_index;
    int *powers;
    float tmp;
    int i, j;

    scanf("%d", &num_powers);
    Powers::setSize(num_powers);

    powers = (int*) malloc(num_powers * sizeof(int));
    for (i = 0; i < num_powers; i++)
        scanf("%d", &powers[i]);

    scanf("%d", &num_heights);
    for (i = 0; i < num_heights; i++)
    {
        scanf("%d", &tmp_height);

        if (tmp_height == height)
            height_index = i;
    }

    for (i = 0; i < num_powers; i++)
    {
        for (j = 0; j < num_heights; j++)
        {
            scanf("%f", &tmp);

            if (j == height_index)
                Powers::setItem(i, powers[i], tmp);
        }
    }

    free(powers);
}

