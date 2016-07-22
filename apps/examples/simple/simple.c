/*
 * simple.c
￼￼￼￼￼ */

#define ENABLE_EXTENSION_AFFINITY
#include <extensions/ocr-affinity.h>

#include "ocr.h"
#include "ocr-std.h"
#include "macros.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>

#define LOCAL_EQUATION(i, j, above,left) (1*(i+j+above+left))
#define BELOW_EQUATION(i, j, above,left) (0.25*(i+j+above+left))
#define RIGHT_EQUATION(i, j, above,left) (0.50*(i+j+above+left))

u64 getAffinityCount() {
    u64 affinityCount;
    ocrAffinityCount(AFFINITY_PD, &affinityCount);
    PRINTF("Affinity count %d \n", affinityCount);
    return affinityCount;
}

ocrHint_t getAffinity(int i, int j, int affinityCount, int type) {
    int rank = i % affinityCount;
    ocrGuid_t aff;
    ocrAffinityGetAt(AFFINITY_PD, rank, &aff);
    ocrHint_t hint;
    if (type == 0 ) {//EDT hint
        ocrHintInit(&hint,OCR_HINT_EDT_T);
        ocrSetHintValue(&hint, OCR_HINT_EDT_AFFINITY, ocrAffinityToHintValue(aff));
    }
    else{
        ocrHintInit(&hint,OCR_HINT_DB_T);
        ocrSetHintValue(&hint, OCR_HINT_DB_AFFINITY, ocrAffinityToHintValue(aff));
    }
    return hint;
}

ocrHint_t getEDTAffinity(int i, int j, int affinityCount) {
    return getAffinity(i,j,affinityCount, 0);
}

ocrHint_t getDBAffinity(int i, int j, int affinityCount) {
    return getAffinity(i,j,affinityCount, 1);
}

int currentAffinity() {
    // Create EDT hints
    ocrGuid_t* affinities;
    u64 affinityCount;
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
             break;
         }
    }
    return location;
}

typedef struct{
    u64 i;
    u64 j;
    u64 COLS;
    u64 ROWS;
    u64 VICTIM;
    ocrGuid_t below;
    ocrGuid_t right;
    /* ---  resilience ----*/

    u8 recovering;
    ocrGuid_t tileEdt_template_guid;
    ocrGuid_t aboveDep0;
    ocrGuid_t aboveDep1;
    ocrGuid_t aboveSatBelow;
    ocrGuid_t aboveSatRight;
}TileEdtPRM_t;

typedef struct{
    ocrGuid_t below;
    ocrGuid_t right;
}Tile_t;

void killAtAffinity(int victim) {
    int currAffinity = currentAffinity();
    if (currAffinity == victim) {
        assert ( false && "killing here" );
    }
}


ocrGuid_t recreateAbove(TileEdtPRM_t *paramIn) {
	int RANKS = getAffinityCount();
	ocrGuid_t tileEdt_template_guid = paramIn->tileEdt_template_guid;

	/*compute above param from current EDT paramIn*/
	u64 above_i = paramIn->i - 1;
    u64 above_j = paramIn->j;

	TileEdtPRM_t aboveParamv;
	aboveParamv.i = above_i;
	aboveParamv.j = above_j;
	aboveParamv.recovering = 1;
	aboveParamv.ROWS = paramIn->ROWS;
	aboveParamv.COLS = paramIn->COLS;
	aboveParamv.VICTIM = paramIn->VICTIM;
    // forcefully pass guids we need to satisfy on completion
	aboveParamv.right = paramIn->aboveSatRight;
	aboveParamv.below   = paramIn->aboveSatBelow;

	aboveParamv.tileEdt_template_guid = tileEdt_template_guid;
	aboveParamv.aboveDep0 = NULL_GUID;
    aboveParamv.aboveDep1 = NULL_GUID;
    aboveParamv.aboveSatBelow = NULL_GUID;
    aboveParamv.aboveSatRight = NULL_GUID;


	ocrGuid_t task_guid;
	ocrHint_t hint = getEDTAffinity(paramIn->i,paramIn->j,RANKS);
	ocrEdtCreate(&task_guid, tileEdt_template_guid,
	            EDT_PARAM_DEF, (u64 *)&aboveParamv /*paramv*/,
	            EDT_PARAM_DEF, NULL /*depv*/,
	            EDT_PROP_NONE, &hint /*hint*/, NULL /*outputEvent*/);


    /* add dependency to the EDT from the west */
    ocrAddDependence(paramIn->aboveDep0, task_guid, 0, DB_MODE_CONST);

    /* add dependency to the EDT from the north */
    ocrAddDependence(paramIn->aboveDep1, task_guid, 1, DB_MODE_CONST);

    ocrAddEventSatisfier(task_guid,paramIn->aboveDep0);

    ocrAddEventSatisfier(task_guid,paramIn->aboveDep1);

    return task_guid;

}
ocrGuid_t recreateMe(TileEdtPRM_t *paramIn, u32 depc, ocrEdtDep_t depv[]) {
	paramIn->recovering = 1;
	ocrGuid_t task_guid;
    ocrHint_t hint = getEDTAffinity(paramIn->i,paramIn->j,RANKS);
	ocrEdtCreate(&task_guid, paramIn->tileEdt_template_guid,
	            EDT_PARAM_DEF, (u64 *)&paramIn /*paramv*/,
		        EDT_PARAM_DEF, NULL /*depv*/,
		        EDT_PROP_NONE, &hint /*hint*/, NULL /*outputEvent*/);

 	/* left dependence is assumed to never fail (produced by same process in a row distribution) */
	ocrAddDependence(depv[0].guid, task_guid, 0, DB_MODE_CONST);

	/* re-depend on the above tile below value */
	ocrAddDependence(paramIn->aboveSatBelow, task_guid, 1, DB_MODE_CONST);

	ocrAddEventSatisfier(task_guid,paramIn->aboveSatBelow);
}

ocrGuid_t tileEdt ( u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[] ) {
    TileEdtPRM_t *paramIn = (TileEdtPRM_t *)paramv;
    /* Unbox parameters */
    u64 i = (u64) paramIn->i;
    u64 j = (u64) paramIn->j;
    u64 ROWS = (u64) paramIn->ROWS;
    u64 COLS = (u64) paramIn->COLS;
    u64 VICTIM = (u64) paramIn->VICTIM;
    ocrGuid_t right = paramIn->right;
    ocrGuid_t below = paramIn->below;

    killAtAffinity(VICTIM);

    if (paramIn->recovering) {
    	PRINTF("Here[%d]  tileEdt(%d,%d) recovering \n", currentAffinity(), i, j );
    }
    else {
        PRINTF("Here[%d]  tileEdt(%d,%d) checking the dependencies \n", currentAffinity(), i, j );
    }

    bool depSuccess = true;
    u64 di = 0;
    for ( ; di < depc; di++) {
        if (ocrGuidIsFailure(depv[di].guid)) {
              PRINTF("Here[%d]  tileEdt(%d,%d)   dep(%d) is a FAILURE_GUID ... \n", currentAffinity(), i, j , di);
              depSuccess = false;
        }
    }

    if (!depSuccess) {
    	if (di == 1) { /* above EDT failed  */
            recreateAbove(paramIn);
            recreateMe(paramIn, depc, depv);
    	}
        PRINTF("Here[%d]  tileEdt(%d,%d)   returns ....................> \n", currentAffinity(), i, j );
        return FAILURE_GUID;
    }

    u64* leftVal = (u64*)depv[0].ptr;
    u64* aboveVal = (u64*)depv[1].ptr;

    /* Run computation on local tile */
    u64 belowVal = BELOW_EQUATION(i,j,*aboveVal,*leftVal);
    u64 rightVal = RIGHT_EQUATION(i,j,*aboveVal,*leftVal);
    u64 localVal = LOCAL_EQUATION(i,j,*aboveVal,*leftVal);
    /* Satisfy the right and below events */
    PRINTF("Here[%d] tileEdt  :<- (i=%d) (j=%d) (above=%d) (left=%d) (localScore:%d)  :-> (toRight=%d) (toBottom=%d) \n", currentAffinity(), i, j,*aboveVal,*leftVal, localVal, rightVal, belowVal);

    /* Allocate datablock for rightValue */
    ocrGuid_t rightDBGuid;
    u64 *rightDBGuid_data ;
    ocrDbCreate( &rightDBGuid, (void *)&rightDBGuid_data, sizeof(u64), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
    *rightDBGuid_data = rightVal;
    ocrDbRelease(rightDBGuid);
    ocrEventSatisfy(right, rightDBGuid);

    /* Allocate datablock for below value */
    ocrGuid_t belowDBGuid;
    u64* belowDBGuid_data = NULL;
    ocrDbCreate( &belowDBGuid, (void *)&belowDBGuid_data, sizeof(u64), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
    *belowDBGuid_data = belowVal;
    ocrDbRelease(belowDBGuid);
    ocrEventSatisfy(below, belowDBGuid);

    /* We can also free all the input DBs we get */
    ocrDbDestroy(depv[0].guid);
    ocrDbDestroy(depv[1].guid);
    
    if ( i == ROWS && j == COLS ) {
        PRINTF("Shutting down,  score = %d \n", localVal);
        ocrShutdown();
    }
    return NULL_GUID;
}

static void initialize_border_tiles( Tile_t** tile_matrix , int ROWS, int COLS) {
    u64 i, j;
    /* Create datablocks for the bottom right elements and bottom rows for tiles[0][j]
     * and Satisfy the bottom row event for tile[0][j] with the respective datablock */
    for ( j = 1; j < COLS + 1; ++j ) {
        ocrGuid_t db_guid_0_j_below;
        void* db_guid_0_j_below_data;
        ocrDbCreate( &db_guid_0_j_below, &db_guid_0_j_below_data, sizeof(u64), DB_PROP_NONE, NULL_HINT, NO_ALLOC );
        u64* allocated = (u64*)db_guid_0_j_below_data;
        allocated[0] = 0;

        ocrDbRelease(db_guid_0_j_below);
        //PRINTF("satisfy (%d, %d).below ...\n", 0, j);
        ocrEventSatisfy(tile_matrix[0][j].below, db_guid_0_j_below);
    }

    /* Create datablocks for the right columns for tiles[i][0]
     * and Satisfy the right column event for tile[i][0] with the respective datablock */
    for ( i = 1; i < ROWS + 1; ++i ) {
        ocrGuid_t db_guid_i_0_rc;
        void* db_guid_i_0_rc_data;
        ocrDbCreate( &db_guid_i_0_rc, &db_guid_i_0_rc_data, sizeof(u64), DB_PROP_NONE, NULL_HINT, NO_ALLOC );
        u64* allocated = (u64*)db_guid_i_0_rc_data;
        allocated[0] = 0;

        ocrDbRelease(db_guid_i_0_rc);
        ocrEventSatisfy(tile_matrix[i][0].right, db_guid_i_0_rc);
    }
}

//no dependencies
//params includes the parameters of the failed EDT
ocrGuid_t restartEdt ( u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {

}

ocrGuid_t mainEdt ( u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    int RANKS = getAffinityCount();
    int ROWS = 3, COLS = 3;
    int VICTIM = -1;
    u32 input;
    u32 argc = getArgc(depv[0].ptr);

    if((argc != 4)) {
        PRINTF("Usage: simple <rows> <cols> <victim>, defaulting to 3 X 3 , no victim \n");
    } else {
        ROWS = atoi(getArgv(depv[0].ptr, 1));
        COLS = atoi(getArgv(depv[0].ptr, 2));
        VICTIM = atoi(getArgv(depv[0].ptr, 3));
    }

    PRINTF("working with %d X %d   nRanks=%d \n", ROWS, COLS, RANKS);

    TileEdtPRM_t edtParamv;
    u64 i, j;

    ocrGuid_t db_tmp;
    Tile_t** tile_matrix;

    ocrHint_t db_hint = getDBAffinity(1,2,RANKS);
    ocrDbCreate(&db_tmp, (void **)&tile_matrix, sizeof(Tile_t*)*(ROWS+1), DB_PROP_NONE, &db_hint, NO_ALLOC);
    for ( i = 0; i < ROWS+1; ++i ) {
        ocrDbCreate(&db_tmp, (void **)&tile_matrix[i], sizeof(Tile_t)*(COLS+1), DB_PROP_NONE, &db_hint, NO_ALLOC);
        for ( j = 0; j < COLS+1; ++j ) {
            /* Create readiness events for every tile */
            ocrEventCreate(&(tile_matrix[i][j].below ), OCR_EVENT_STICKY_T, EVT_PROP_TAKES_ARG);
            ocrEventCreate(&(tile_matrix[i][j].right ), OCR_EVENT_STICKY_T, EVT_PROP_TAKES_ARG);
        }
    }

    initialize_border_tiles(tile_matrix, ROWS, COLS);
    ocrGuid_t tileEdt_template_guid;
    ocrEdtTemplateCreate(&tileEdt_template_guid, tileEdt, PRMNUM(TileEdt) /*paramc*/, 2 /*depc*/);


    for ( i = 1; i < ROWS+1; ++i ) {
        for ( j = 1; j < COLS+1; ++j ) {
            //PRINTF("createEdt ( %d, %d ) \n", i , j );
            /* Box function paramIn and put them on the heap for lifetime */
            edtParamv.i = i;
            edtParamv.j = j;
            edtParamv.recovering = 0;
            edtParamv.ROWS = ROWS;
            edtParamv.COLS = COLS;
            edtParamv.VICTIM = VICTIM;
            // forcefully pass guids we need to satisfy on completion
            edtParamv.right = tile_matrix[i][j].right;
            edtParamv.below   = tile_matrix[i][j].below;

            edtParamv.tileEdt_template_guid = tileEdt_template_guid;
            edtParamv.aboveDep0 = tile_matrix[i-1][j-1].right;
            if (i == 1) {
                edtParamv.aboveDep1 = tile_matrix[i-2][j].below;
            }
            else {
            	edtParamv.aboveDep1 = NULL_GUID;
            }
            edtParamv.aboveSatBelow = tile_matrix[i-1][j].below;
            edtParamv.aboveSatRight = tile_matrix[i-1][j].right;

            /* Create an event-driven tasks */
            ocrGuid_t task_guid;
            ocrHint_t hint = getEDTAffinity(i,j,RANKS);
            ocrEdtCreate(&task_guid, tileEdt_template_guid,
                        EDT_PARAM_DEF, (u64 *)&edtParamv /*paramv*/,
                        EDT_PARAM_DEF, NULL /*depv*/,
                        EDT_PROP_NONE, &hint /*hint*/, NULL /*outputEvent*/);

            /* add dependency to the EDT from the west */
            ocrAddDependence(tile_matrix[i][j-1].right, task_guid, 0, DB_MODE_CONST);

            /* add dependency to the EDT from the north */
            ocrAddDependence(tile_matrix[i-1][j].below, task_guid, 1, DB_MODE_CONST);

            ocrAddEventSatisfier(task_guid,tile_matrix[i][j].right);

            ocrAddEventSatisfier(task_guid,tile_matrix[i][j].below);

        }
    }

    return NULL_GUID;
}
