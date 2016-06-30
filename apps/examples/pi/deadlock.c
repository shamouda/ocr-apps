#include "ocr.h"
#define ENABLE_EXTENSION_AFFINITY
#include <extensions/ocr-affinity.h>
#include <stdlib.h>

ocrGuid_t A_B_Edt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    PRINTF("A_B_Edt  started ...\n");
    ocrGuid_t X_Guid = depv[0].guid;
    ocrDbRelease(X_Guid);

    PRINTF("A_B_Edt (DB released) ...\n");
    sleep(2);
    PRINTF("A_B_Edt  ended ...\n");
    return NULL_GUID;
}


ocrGuid_t shutdownEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    PRINTF("shutdownEdt  started ...\n");

    ocrShutdown();
    
    PRINTF("shutdownEdt  ended ...\n");
    return NULL_GUID;
}

ocrGuid_t mainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    //create templates
    ocrGuid_t A_B_EdtTemplateGuid, shutdownEdtTemplateGuid;
    ocrEdtTemplateCreate(&A_B_EdtTemplateGuid, A_B_Edt, 0 /*paramc*/, 2 /*depc*/);
    ocrEdtTemplateCreate(&shutdownEdtTemplateGuid, shutdownEdt, 0 /*paramc*/, 2 /*depc*/);

    ocrGuid_t affinities[2];
    u64 affinityCount = 2;
    ocrAffinityGet(AFFINITY_PD, &affinityCount, affinities);


    //EDT Hint
    ocrHint_t p0EDT_AffinityHNT;
    ocrHintInit(&p0EDT_AffinityHNT,OCR_HINT_EDT_T);
    ocrSetHintValue(&p0EDT_AffinityHNT, OCR_HINT_EDT_AFFINITY, ocrAffinityToHintValue(affinities[0]));

    //DB hints
    ocrHint_t p0DB_AffinityHNT;
    ocrHintInit(&p0DB_AffinityHNT,OCR_HINT_DB_T);
    ocrSetHintValue(&p0DB_AffinityHNT, OCR_HINT_DB_AFFINITY, ocrAffinityToHintValue(affinities[0]));

    ocrHint_t p1DB_AffinityHNT;
    ocrHintInit(&p1DB_AffinityHNT,OCR_HINT_DB_T);
    ocrSetHintValue(&p1DB_AffinityHNT, OCR_HINT_DB_AFFINITY, ocrAffinityToHintValue(affinities[1]));

    //generate data blocks Y@P0  and X@P1
    ocrGuid_t X_DB_Guid, Y_DB_Guid;

    void *ptr;
    ocrDbCreate(&X_DB_Guid, &ptr, sizeof(u32), DB_PROP_NONE, &p1DB_AffinityHNT, NO_ALLOC);
    ocrDbCreate(&Y_DB_Guid, &ptr, sizeof(u32), DB_PROP_NONE, &p0DB_AffinityHNT, NO_ALLOC);

    //create EDTs A@P0 and B@P0

    ocrGuid_t A_EventGuid, B_EventGuid;
    ocrGuid_t A_EdtGuid, B_EdtGuid;
    ocrEdtCreate(&A_EdtGuid, A_B_EdtTemplateGuid, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL,
                      /*prop=*/EDT_PROP_NONE, &p0EDT_AffinityHNT, &A_EventGuid);

    ocrEdtCreate(&B_EdtGuid, A_B_EdtTemplateGuid, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL,
                          /*prop=*/EDT_PROP_NONE, &p0EDT_AffinityHNT, &B_EventGuid);


    ocrGuid_t shutdownEdtGuid;
    ocrEdtCreate(&shutdownEdtGuid , shutdownEdtTemplateGuid , EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL,
                /*prop=*/EDT_PROP_NONE, &p0EDT_AffinityHNT, NULL);

    ocrAddDependence(A_EventGuid, shutdownEdtGuid, 0, DB_MODE_RO);
    ocrAddDependence(B_EventGuid, shutdownEdtGuid, 1, DB_MODE_RO);


    //A and B depend on X (RW)  and on Y (WE)
    ocrAddDependence(X_DB_Guid, A_EdtGuid, 0, DB_MODE_RW);
    ocrAddDependence(Y_DB_Guid, A_EdtGuid, 1, DB_MODE_EW);


    ocrAddDependence(X_DB_Guid, B_EdtGuid, 0, DB_MODE_RW);
    ocrAddDependence(Y_DB_Guid, B_EdtGuid, 1, DB_MODE_EW);




    return NULL_GUID;
}
