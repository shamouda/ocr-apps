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


#define COLS 5
#define ROWS 5

#define BELOW_EQUATION(i, j, above,left) (0.25*(i+j+above+left))
#define RIGHT_EQUATION(i, j, above,left) (0.50*(i+j+above+left))

//row affinity
#define AFFINITY_MODE_ROW

u64 getAffinityCount() {
	u64 affinityCount;
	ocrAffinityCount(AFFINITY_PD, &affinityCount);
	return affinityCount;
}

ocrHint_t getAffinity(int i, int j, int affinityCount) {
#ifdef AFFINITY_MODE_ROW
	int rank = i % affinityCount;
#else
	int rank = j % affinityCount;
#endif
	ocrGuid_t aff;
	ocrAffinityGetAt(AFFINITY_PD, rank, &aff);
	ocrHint_t hint;
	ocrHintInit(&hint,OCR_HINT_EDT_T);
	ocrSetHintValue(&hint, OCR_HINT_EDT_AFFINITY, ocrAffinityToHintValue(aff));
	return hint;
}

typedef struct{
    u64 i;
    u64 j;
    ocrGuid_t below;
    ocrGuid_t right;
}TileEdtPRM_t;

typedef struct{
    ocrGuid_t below;
    ocrGuid_t right;
}Tile_t;

ocrGuid_t tileEdt ( u32 paramc, u64* paramv, u32 depc , ocrEdtDep_t depv[]) {
	PRINTF("tileEdt paramc=%d\n", paramc);
    u64* leftVal = (u64*)depv[0].ptr;
	u64* aboveVal = (u64*)depv[1].ptr;
	
	PRINTF("tileEdt - before casting \n");
	TileEdtPRM_t *paramIn = (TileEdtPRM_t *)paramv;
	PRINTF("tileEdt - after casting \n");
	/* Unbox parameters */
	u64 i = (u64) paramIn->i;
	u64 j = (u64) paramIn->j;
	PRINTF("tileEdt - started (i=%d) (j=%d) \n", i, j);
	/* Run a smith-waterman on the local tile */
    //step();
    u64 belowVal = BELOW_EQUATION(i,j,*aboveVal,*leftVal);
    u64 rightVal = RIGHT_EQUATION(i,j,*aboveVal,*leftVal);
    /******************************************/
    ocrGuid_t right = paramIn->right;
    ocrGuid_t below = paramIn->below;

    /* Allocate datablock for right column of the local tile */
	ocrGuid_t rightDBGuid;
	u64 *rightDBGuid_data ;
	ocrDbCreate( &rightDBGuid, (void *)&rightDBGuid_data, sizeof(u64), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
	*rightDBGuid_data = rightVal;
	ocrDbRelease(rightDBGuid);
	ocrEventSatisfy(right, rightDBGuid);

    /* Allocate datablock for bottom row of the local tile */
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
	    PRINTF("calling shutdown\n");
    	ocrShutdown();
    }
    return NULL_GUID;
}


static void initialize_border_values( Tile_t** tile_matrix ) {
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


ocrGuid_t mainEdt ( u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {

    TileEdtPRM_t edtParamv;
    u64 i, j;

    ocrGuid_t db_tmp;
    Tile_t** tile_matrix;

	ocrDbCreate(&db_tmp, (void **)&tile_matrix, sizeof(Tile_t*)*(ROWS+1), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
	for ( i = 0; i < ROWS+1; ++i ) {
	    ocrDbCreate(&db_tmp, (void **)&tile_matrix[i], sizeof(Tile_t)*(COLS+1), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
	    for ( j = 0; j < COLS+1; ++j ) {
	        /* Create readiness events for every tile */
            ocrEventCreate(&(tile_matrix[i][j].below ), OCR_EVENT_STICKY_T, EVT_PROP_TAKES_ARG);
            ocrEventCreate(&(tile_matrix[i][j].right ), OCR_EVENT_STICKY_T, EVT_PROP_TAKES_ARG);
	    }
	}

	initialize_border_values(tile_matrix);
	ocrGuid_t tileEdt_template_guid;
	ocrEdtTemplateCreate(&tileEdt_template_guid, tileEdt, 1 /*paramc*/, 2 /*depc*/);


    for ( i = 1; i < ROWS+1; ++i ) {
        for ( j = 1; j < COLS+1; ++j ) {
        	PRINTF("createEdt ( %d, %d ) \n", i , j );
            /* Box function paramIn and put them on the heap for lifetime */

        	edtParamv.i = i;
        	edtParamv.j = j;
            // forcefully pass guids we need to satisfy on completion
            edtParamv.right = tile_matrix[i][j].right;
            edtParamv.below   = tile_matrix[i][j].below;
            /* Create an event-driven tasks of smith_waterman tasks */
            ocrGuid_t task_guid;
            ocrEdtCreate(&task_guid, tileEdt_template_guid,
                        EDT_PARAM_DEF, (u64 *)&edtParamv /*paramv*/,
                        EDT_PARAM_DEF, NULL /*depv*/,
                        EDT_PROP_NONE, NULL_HINT /*hint*/, NULL /*outputEvent*/);

            /* add dependency to the EDT from the west tile's right column ready event */
            ocrAddDependence(tile_matrix[i][j-1].right, task_guid, 0, DB_MODE_CONST);

            /* add dependency to the EDT from the north tile's bottom row ready event */
            ocrAddDependence(tile_matrix[i-1][j].below, task_guid, 1, DB_MODE_CONST);
        }
    }


    return NULL_GUID;
}
