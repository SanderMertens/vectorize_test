#include <include/vectorize_test.h>
#include <stdlib.h>

/* Position component */
typedef struct Position {
    float x;
    float y;
} Position;

/* Speed component */
typedef float Speed;

/* Struct w Position, Speed + additional data to better mimic actual application */
typedef struct Entity {
    Position p;
    float size;
    float angle;
    Speed s;
    int mass;
    float foobar;
    Position v;
} Entity;

/** Run benchmarks.
 * For each scenario, run a cold and a warm test, which measure the difference
 * between whether data still has to be loaded from RAM vs whether the data is
 * already in the cache.
 */
void benchmark(int count) {
    printf("\n-- Preparing data for %d entities\n", count);

    /* Each attribute its own array */
    float *x = malloc(count * sizeof(float));
    float *y = malloc(count * sizeof(float));
    float *s = malloc(count * sizeof(float));

    /* Store Position and Speed in separate arrays */
    Position *positions = malloc(count * sizeof(Position));
    Speed *speeds = malloc(count * sizeof(Speed));

    /* Store entity structs in array */
    Entity *entities = malloc(count * sizeof(Entity));

    /* Store entities in separate blocks on the heap */
    Entity **entity_ptrs = malloc(count * sizeof(Entity*));
    void **garbage_ptrs = malloc(count * sizeof(void*));
    
    for (int i = 0; i < count; i ++) {
        entity_ptrs[i] = malloc(sizeof(Entity));

        /* Add garbage inbetween struct allocations. This better simulates 
         * actual OOP-style applications, since objects are not typically all
         * allocated at the same moment. 
         * Without this line, allocated structs most likely end up being in
         * consecutive memory, except for the tests with large numbers of 
         * entities.*/
        garbage_ptrs[i] = malloc(64); 
    }

    struct timespec start;

    printf("-- Start benchmarks\n");


    /* -- Attributes in separate arrays */
    timespec_gettime(&start);
#pragma clang loop vectorize(enable)
    for(int i = 0; i < count; i ++) {
        x[i] += s[i];
        y[i] += s[i];
    }
    printf("   SoA, cold:                    %f (V)\n", timespec_measure(&start));

    timespec_gettime(&start);
#pragma clang loop vectorize(enable)
    for(int i = 0; i < count; i ++) {
        x[i] += s[i];
        y[i] += s[i];
    }
    printf("   SoA, warm:                    %f (V)\n", timespec_measure(&start));


    /* -- Position in a separate struct */
    timespec_gettime(&start);
#pragma clang loop vectorize(enable)
    for(int i = 0; i < count; i ++) {
        positions[i].x += speeds[i];
        positions[i].y += speeds[i];
    }
    printf("   SoA (components), cold:       %f (V)\n", timespec_measure(&start));

    timespec_gettime(&start);
#pragma clang loop vectorize(enable)
    for(int i = 0; i < count; i ++) {
        positions[i].x += speeds[i];
        positions[i].y += speeds[i];
    }
    printf("   SoA (components), warm:       %f (V)\n", timespec_measure(&start));


    /* -- All attributes in one Entity struct (contains also other, unused data) */
    timespec_gettime(&start);
#pragma clang loop vectorize(disable)
    for(int i = 0; i < count; i ++) {
        entities[i].p.x += entities[i].s;
        entities[i].p.y += entities[i].s;
    }
    printf("   AoS, cold:                    %f\n", timespec_measure(&start));

    timespec_gettime(&start);
#pragma clang loop vectorize(disable)
    for(int i = 0; i < count; i ++) {
        entities[i].p.x += entities[i].s;
        entities[i].p.y += entities[i].s;
    }
    printf("   AoS, warm:                    %f\n", timespec_measure(&start));


    /* -- All attributes in one Entity struct, each separately allocated on heap */
    timespec_gettime(&start);
#pragma clang loop vectorize(disable)
    for(int i = 0; i < count; i ++) {
        Entity *e = entity_ptrs[i];
        e->p.x += e->s;
        e->p.y += e->s;
    }
    printf("   Heap blocks, cold:            %f\n", timespec_measure(&start));

    timespec_gettime(&start);
#pragma clang loop vectorize(disable)
    for(int i = 0; i < count; i ++) {
        Entity *e = entity_ptrs[i];
        e->p.x += e->s;
        e->p.y += e->s;
    }
    printf("   Heap blocks, warm:            %f\n", timespec_measure(&start));


    printf("-- Cleaning up data\n");
    free(x);
    free(y);
    free(s);
    free(positions);
    free(speeds);
    free(entities);
    for (int i = 0; i < count; i ++) {
        free(entity_ptrs[i]);
        free(garbage_ptrs[i]);
    }
    free(entity_ptrs);
    free(garbage_ptrs);
    printf("-- Benchmarks done\n");
}

int main(int argc, char *argv[]) {
    benchmark(100 * 1000);
    benchmark(1000 * 1000);
    benchmark(1000 * 1000);
    benchmark(10 * 1000 * 1000);
    benchmark(50 * 1000 * 1000);
    benchmark(100 * 1000 * 1000);
    benchmark(200 * 1000 * 1000);
    return 0;
}