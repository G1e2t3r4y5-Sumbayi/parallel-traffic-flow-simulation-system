/*
 Parallel Traffic Flow Simulation System – Integrated Version (FIXED)
 COMP 412 – Parallel and Distributed Systems
 Language: C
 Models: Shared Memory (OpenMP) + Simulated Distributed Intersections
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define ROADS 4
#define INTERSECTIONS 3
#define STEPS 100
#define MAX_VEHICLES 100

/* Shared memory structures */
int roadVehicles[ROADS];
int trafficLights[ROADS];
int intersectionBuffer[INTERSECTIONS];

void initialize_system() {
    for (int i = 0; i < ROADS; i++) {
        roadVehicles[i] = rand() % MAX_VEHICLES;
        trafficLights[i] = 1; /* green */
    }
    for (int i = 0; i < INTERSECTIONS; i++) {
        intersectionBuffer[i] = 0;
    }
}

//simulation function
void simulate_roads_parallel() {
    #pragma omp parallel for
    for (int i = 0; i < ROADS; i++) {
        if (trafficLights[i] && roadVehicles[i] > 0) {
            int moved = rand() % 5;
            if (moved > roadVehicles[i]) moved = roadVehicles[i];
            roadVehicles[i] -= moved;

            if (i < INTERSECTIONS) {
                #pragma omp atomic
                intersectionBuffer[i] += moved;
            }
        }
    }
}

void simulate_intersections_distributed() {
    #pragma omp parallel for
    for (int i = 0; i < INTERSECTIONS; i++) {
        int transfer = intersectionBuffer[i] / 2;

        #pragma omp critical
        {
            intersectionBuffer[i] -= transfer;
            if (i + 1 < ROADS)
                roadVehicles[i + 1] += transfer;
        }
    }
}

void update_traffic_lights() {
    for (int i = 0; i < ROADS; i++) {
        trafficLights[i] = !trafficLights[i];
    }
}

int main() {
    srand(time(NULL));
    initialize_system();

    double startTime = omp_get_wtime();

    for (int step = 0; step < STEPS; step++) {
        simulate_roads_parallel();
        simulate_intersections_distributed();
        update_traffic_lights();
    }

    double endTime = omp_get_wtime();

    printf("\nFINAL TRAFFIC STATE\n");
    for (int i = 0; i < ROADS; i++) {
        printf("Road %d: %d vehicles\n", i, roadVehicles[i]);
    }

    printf("Execution Time: %f seconds\n", endTime - startTime);
    return 0;
}
