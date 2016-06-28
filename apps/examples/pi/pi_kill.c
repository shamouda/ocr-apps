/*
 * pi.c
 * Parallel Monte Carlo PI simulation  (https://en.wikipedia.org/wiki/Monte_Carlo_method)
 * Originally written in Jun 2016 by Sara Hamouda
 *
 * Implements the following dependence graph :
 *                  mainEdt
 *    /       /        |        \        \
 *  workerEdt  ....................   workerEdt
 *    \       \        |         /        /
 *                 shutdownEdt
￼￼￼￼￼ */

#include "ocr.h"
#include "ocr-std.h"
#include "macros.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define ITERS 10000000
#define TASKS_COUNT 5

ocrGuid_t workerEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    struct drand48_data drand_buf;
    int seed = time(NULL);
    srand48_r (seed, &drand_buf);

    u32 total = 0;
    int i = 0;
    double x;
    double y;
    for (i = 0; i < ITERS; i++) {
        drand48_r (&drand_buf, &x);
        drand48_r (&drand_buf, &y);
        if (x*x + y*y <= 1.0) {
            total++;
        }
    }

    //generate the output data block
    ocrGuid_t workerOutputGuid;
    u32 *ptr;
    ocrDbCreate(&workerOutputGuid, (void **) &ptr, sizeof(u32), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
    *ptr = total;

    return workerOutputGuid;
}

void workerErrorHandler(ocrGuid_t event, u32 errorCode, u32 slot) {
    //A worker EDT has failed, satisfy its output event with a suitable value
    ocrGuid_t defaultOutputGuid;
    u32 *ptr;
    ocrDbCreate(&defaultOutputGuid, (void **) &ptr, sizeof(u32), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
    *ptr = -1;  // return -1 to indicate a failed EDT output

    ocrEventSatisfySlot(event, defaultOutputGuid, slot);
}

ocrGuid_t killEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
	//sleep for some time before killing the EDT's process
	usleep(100);
    assert(false);
    return NULL_GUID;
}


ocrGuid_t shutdownEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    u32 sumPoints = 0;
    u32 successfulTasksCount = 0;
    float PI = 0.0;
    int i = 0;
    for (i = 0 ; i < TASKS_COUNT; i++) {
        u32* taskOutput = depv[i].ptr;
        if (taskOutput != -1) {
        	sumPoints += *taskOutput;
        	successfulTasksCount ++;
        }
    }

    PI = 4.0f * sumPoints / (ITERS * successfulTasksCount) ;
    PRINTF("PI equals %f \n" , PI);

    //destroy the workers data blocks
    for (i = 0 ; i < TASKS_COUNT; i++) {
        ocrDbDestroy(depv[i].guid);
    }

    ocrShutdown();
    return NULL_GUID;
}

ocrGuid_t mainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    ocrGuid_t workerEdtTemplateGuid, shutdownEdtTemplateGuid, killEdtTemplateGuid;
    ocrEdtTemplateCreate(&workerEdtTemplateGuid, workerEdt, 0 /*paramc*/, 1 /*depc*/);
    ocrEdtTemplateCreate(&killEdtTemplateGuid, killEdt, 0 /*paramc*/, 1 /*depc*/);
    ocrEdtTemplateCreate(&shutdownEdtTemplateGuid, shutdownEdt, 0 /*paramc*/, TASKS_COUNT + 1 /*depc*/);

    ocrGuid_t shutdownEdtGuid;
    ocrEdtCreate(&shutdownEdtGuid , shutdownEdtTemplateGuid , EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL,
            /*prop=*/EDT_PROP_NONE, NULL_HINT, NULL);

    //create the worker tasks
    int i;
    for (i = 0 ; i <= TASKS_COUNT; i++) {
        ocrGuid_t workerOutputEventGuid;
        ocrGuid_t workerEdtGuid;
        ocrGuid_t templateGuid;
        if (i==0) {
        	templateGuid = killEdtTemplateGuid;
        }
        else{
        	templateGuid = workerEdtTemplateGuid;
        }

        ocrEdtCreateR(&workerEdtGuid, templateGuid, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL,
                                 /*prop=*/EDT_PROP_NONE, NULL_HINT, &workerOutputEventGuid, workerErrorHandler);

        //shutdownEdt depends on all workerEdts and a killEdt
        ocrAddDependence(workerOutputEventGuid, shutdownEdtGuid, i, DB_MODE_RO);

        //Start executing the worker/kill EDT
        ocrAddDependence(NULL_GUID, workerEdtGuid, 0 , DB_DEFAULT_MODE);
    }

    return NULL_GUID;
}
