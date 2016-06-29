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

#define ENABLE_EXTENSION_AFFINITY
#include <extensions/ocr-affinity.h>

#include "ocr.h"
#include "ocr-std.h"
#include "macros.h"

#include <stdlib.h>
#include <time.h>

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

ocrGuid_t shutdownEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    u32 sumPoints = 0;
    float PI = 0.0;
    int i = 0;
    for (i = 0 ; i < TASKS_COUNT; i++) {
        u32* taskOutput = depv[i].ptr;
        sumPoints += *taskOutput;
    }

    PI = 4.0f * sumPoints / (ITERS * TASKS_COUNT) ;
    PRINTF("PI equals %f \n" , PI);

    //destroy the workers data blocks
    for (i = 0 ; i < TASKS_COUNT; i++) {
        ocrDbDestroy(depv[i].guid);
    }

    ocrShutdown();
    return NULL_GUID;
}

ocrGuid_t mainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    ocrGuid_t workerEdtTemplateGuid, shutdownEdtTemplateGuid;
    ocrEdtTemplateCreate(&workerEdtTemplateGuid, workerEdt, 0 /*paramc*/, 1 /*depc*/);
    ocrEdtTemplateCreate(&shutdownEdtTemplateGuid, shutdownEdt, 0 /*paramc*/, TASKS_COUNT /*depc*/);

    ocrHint_t affinityHNT;
    ocrHintInit(&affinityHNT,OCR_HINT_EDT_T);
    ocrGuid_t myAffinity;
    ocrAffinityGetCurrent(&myAffinity);
    ocrSetHintValue(&affinityHNT, OCR_HINT_EDT_AFFINITY, ocrAffinityToHintValue(myAffinity));


    ocrGuid_t shutdownEdtGuid;
    ocrEdtCreate(&shutdownEdtGuid , shutdownEdtTemplateGuid , EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL,
            /*prop=*/EDT_PROP_NONE, &affinityHNT, NULL);

    //create the worker tasks
    int i;
    for (i = 0 ; i < TASKS_COUNT; i++) {
        ocrGuid_t workerOutputEventGuid;
        ocrGuid_t workerEdtGuid;
        ocrEdtCreate(&workerEdtGuid, workerEdtTemplateGuid, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL,
                            /*prop=*/EDT_PROP_NONE, &affinityHNT, &workerOutputEventGuid);

        //shutdownEdt depends on all workerEdts
        ocrAddDependence(workerOutputEventGuid, shutdownEdtGuid, i, DB_MODE_RO);

        //Start executing the worker EDT
        ocrAddDependence(NULL_GUID, workerEdtGuid, 0 , DB_DEFAULT_MODE);
    }

    return NULL_GUID;
}
