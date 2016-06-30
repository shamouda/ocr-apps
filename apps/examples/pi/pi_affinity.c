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

/******************************   Utility Functions   ***********************************/

void printCurrentAffinity(char* edtName) {
	// Create EDT hints
	ocrGuid_t* affinities;
	u64 affinityCount;
	////////getAffinityGuids(&affinityCount, &affinities);

	ocrAffinityCount(AFFINITY_PD, &affinityCount);
	ocrGuid_t affDbGuid;
	ocrDbCreate(&affDbGuid, (void **) &affinities, affinityCount*sizeof(ocrGuid_t), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
	ocrAffinityGet(AFFINITY_PD, &affinityCount, affinities);
	ocrDbRelease(affDbGuid);

	ocrGuid_t curAffinity;
	ocrAffinityGetCurrent(&curAffinity);

	int location=0;
	for (; location < affinityCount; location++) {
         if (ocrGuidIsEq(curAffinity,affinities[location])){
        	 PRINTF("EDT[%s]  Location[%d] \n", edtName, location);
         }
	}
}

void getEdtAffinityHints(ocrHint_t* p0Hint, ocrHint_t* p1Hint, ocrGuid_t* affinities) {
	ocrHintInit(p0Hint,OCR_HINT_EDT_T);
	ocrSetHintValue(p0Hint, OCR_HINT_EDT_AFFINITY, ocrAffinityToHintValue(affinities[0]));

	ocrHintInit(p1Hint,OCR_HINT_EDT_T);
	ocrSetHintValue(p1Hint, OCR_HINT_EDT_AFFINITY, ocrAffinityToHintValue(affinities[1]));
}

void getDbAffinityHints(ocrHint_t* p0Hint, ocrHint_t* p1Hint, ocrGuid_t* affinities) {
	ocrHintInit(p0Hint,OCR_HINT_DB_T);
	ocrSetHintValue(p0Hint, OCR_HINT_DB_AFFINITY, ocrAffinityToHintValue(affinities[0]));

	ocrHintInit(p1Hint,OCR_HINT_DB_T);
	ocrSetHintValue(p1Hint, OCR_HINT_DB_AFFINITY, ocrAffinityToHintValue(affinities[1]));
}

void getAffinityGuids(u64* affinityCount, ocrGuid_t* affinities) {
	ocrAffinityCount(AFFINITY_PD, affinityCount);
	ocrGuid_t guid;
	ocrDbCreate(&guid, (void **) &affinities, (*affinityCount)*sizeof(ocrGuid_t), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
	ocrAffinityGet(AFFINITY_PD, affinityCount, affinities);
	ocrDbRelease(guid);

	PRINTF("Affinities count = %d \n ", *affinityCount);
	int r = 0;
	for (; r < *affinityCount; r++) {
	    PRINTF("Affinity[%d] = "GUIDF"\n", r , GUIDA(affinities[r]));
	}
}

/******************************   Utility Functions End ***********************************/


ocrGuid_t workerEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
	printCurrentAffinity("workerEdt");

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
	printCurrentAffinity("shutdownEdt");

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
	printCurrentAffinity("mainEdt");

    /********* Prepare Affinity Hints  *************/
    ocrGuid_t* affinities;
    u64 affinityCount;
    ////////getAffinityGuids(&affinityCount, &affinities);

    ocrAffinityCount(AFFINITY_PD, &affinityCount);
    ocrGuid_t affDbGuid;
    ocrDbCreate(&affDbGuid, (void **) &affinities, affinityCount*sizeof(ocrGuid_t), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
    ocrAffinityGet(AFFINITY_PD, &affinityCount, affinities);
    ocrDbRelease(affDbGuid);
    /*
    PRINTF("Affinities count = %d \n", affinityCount);
    int r = 0;
    for (; r < affinityCount; r++) {
        PRINTF("Affinity[%d] = "GUIDF"\n", r , GUIDA(affinities[r]));
    }
    */
    ocrHint_t p0EdtAffinityHint, p1EdtAffinityHint;
    getEdtAffinityHints(&p0EdtAffinityHint, &p1EdtAffinityHint, affinities);


    /********* Prepare Templates and EDTs  *************/
    ocrGuid_t workerEdtTemplateGuid, shutdownEdtTemplateGuid;
    ocrEdtTemplateCreate(&workerEdtTemplateGuid, workerEdt, 0 /*paramc*/, 1 /*depc*/);
    ocrEdtTemplateCreate(&shutdownEdtTemplateGuid, shutdownEdt, 0 /*paramc*/, TASKS_COUNT /*depc*/);

    ocrGuid_t shutdownEdtGuid;
    ocrEdtCreate(&shutdownEdtGuid , shutdownEdtTemplateGuid , EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL,
            /*prop=*/EDT_PROP_NONE, &p0EdtAffinityHint, NULL);

    //create the worker tasks
    int i;
    for (i = 0 ; i < TASKS_COUNT; i++) {
        ocrGuid_t workerOutputEventGuid;
        ocrGuid_t workerEdtGuid;
        ocrEdtCreate(&workerEdtGuid, workerEdtTemplateGuid, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL,
                            /*prop=*/EDT_PROP_NONE, &p1EdtAffinityHint, &workerOutputEventGuid);

        //shutdownEdt depends on all workerEdts
        ocrAddDependence(workerOutputEventGuid, shutdownEdtGuid, i, DB_MODE_RO);

        //Start executing the worker EDT
        ocrAddDependence(NULL_GUID, workerEdtGuid, 0 , DB_DEFAULT_MODE);
    }

    return NULL_GUID;
}
