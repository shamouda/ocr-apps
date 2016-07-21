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
    ocrGuid_t above;
    ocrGuid_t left;
}Tile_t;

ocrGuid_t tileEdt ( u32 paramc, u64* paramv, u32 depc /*2*/, ocrEdtDep_t depv[]) {
	TileEdtPRM_t *parameters = (TileEdtPRM_t *)paramv;
	u64 i = (u64) parameters->i;
	u64 j = (u64) parameters->j;
	ocrGuid_t belowEvt = parameters->below;
	ocrGuid_t rightEvt = parameters->right;

	/******************************************/
	u64 aboveVal = *((u64*)depv[0].ptr);
	u64 leftVal = *((u64*)depv[1].ptr);

    u64 belowVal = BELOW_EQUATION(i,j,aboveVal,leftVal);
    u64 rightVal = RIGHT_EQUATION(i,j,aboveVal,leftVal);

	ocrGuid_t belowDBGuid, rightDBGuid;
	u32 *belowPtr, *rightPtr ;
	ocrDbCreate(&belowDBGuid, (void **) &belowPtr, sizeof(u32), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
	ocrDbCreate(&rightDBGuid, (void **) &rightPtr, sizeof(u32), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
	*belowPtr = belowVal;
	*rightPtr = rightVal;

	/******************************************/
	ocrEventSatisfy(belowEvt, belowDBGuid);
	ocrEventSatisfy(rightEvt, rightDBGuid);

    if ( i == ROWS && j == COLS ) {
	    PRINTF("calling shutdown\n");
    	ocrShutdown();
    }
}


static void initialize_border_values( Tile_t** tile_matrix ) {
	u64 i, j;
	/******    FIXME: what about Tile[0][0]     *******/

    /* Create datablocks for the bottom right elements and bottom rows for tiles[0][j]
     * and Satisfy the bottom row event for tile[0][j] with the respective datablock */
    for ( j = 1; j < COLS + 1; ++j ) {
        ocrGuid_t db_guid_0_j_brow;
        void* db_guid_0_j_brow_data;
        ocrDbCreate( &db_guid_0_j_brow, &db_guid_0_j_brow_data, sizeof(u64), DB_PROP_NONE, NULL_HINT, NO_ALLOC );
        u64* allocated = (u64*)db_guid_0_j_brow_data;
        allocated[0] = 0;

        ocrDbRelease(db_guid_0_j_brow);
        ocrEventSatisfy(tile_matrix[0][j].above, db_guid_0_j_brow);
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
        ocrEventSatisfy(tile_matrix[i][0].left, db_guid_i_0_rc);
    }
}


ocrGuid_t mainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {

	ocrGuid_t db_tmp;
	Tile_t** tile_matrix;
    int i,j;

	ocrDbCreate(&db_tmp, (void **)&tile_matrix, sizeof(Tile_t*)*(ROWS+1), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
	for ( i = 0; i < ROWS+1; ++i ) {
	    ocrDbCreate(&db_tmp, (void **)&tile_matrix[i], sizeof(Tile_t)*(COLS+1), DB_PROP_NONE, NULL_HINT, NO_ALLOC);
	    for ( j = 0; j < COLS+1; ++j ) {
	        /* Create readiness events for every tile */
	        ocrEventCreate(&(tile_matrix[i][j].above ), OCR_EVENT_STICKY_T, EVT_PROP_TAKES_ARG);
	        ocrEventCreate(&(tile_matrix[i][j].left ) , OCR_EVENT_STICKY_T, EVT_PROP_TAKES_ARG);
	    }
	}

	TileEdtPRM_t edtParam;

	initialize_border_values(tile_matrix);
	ocrGuid_t edt_template_guid;
	ocrEdtTemplateCreate(&edt_template_guid, tileEdt, 1 /*paramc*/, 2 /*depc*/);


    for ( i = 1; i < ROWS+1; ++i ) {
        for ( j = 1; j < COLS+1; ++j ) {
            /* Box function parameters and put them on the heap for lifetime */

        	edtParam.i = i;
        	edtParam.j = j;
            // forcefully pass guids we need to satisfy on completion
        	edtParam.right = tile_matrix[i][j].left;
        	edtParam.below   = tile_matrix[i][j].above;
            /* Create an event-driven tasks of smith_waterman tasks */
            ocrGuid_t task_guid;
            ocrEdtCreate(&task_guid, edt_template_guid,
                        EDT_PARAM_DEF, (u64 *)&edtParam /*paramv*/,
                        EDT_PARAM_DEF, NULL /*depv*/,
                        EDT_PROP_NONE, NULL_HINT /*hint*/, NULL /*outputEvent*/);

            /* add dependency to the EDT from the west tile's right column ready event */
            ocrAddDependence(tile_matrix[i][j-1].left, task_guid, 0, DB_MODE_CONST);

            /* add dependency to the EDT from the north tile's bottom row ready event */
            ocrAddDependence(tile_matrix[i-1][j].above, task_guid, 1, DB_MODE_CONST);
        }
    }


    return NULL_GUID;
}
