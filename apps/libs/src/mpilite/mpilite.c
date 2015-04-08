/*
* This file is subject to the license agreement located in the file LICENSE
* and cannot be distributed without it. This notice cannot be
* removed or modified.
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ocr.h>

#define ENABLE_EXTENSION_LEGACY 1
#define ENABLE_EXTENSION_RTITF 1
#include <extensions/ocr-legacy.h>
#include <extensions/ocr-runtime-itf.h>
#include <mpi_ocr.h>
#include "mpi.h"
#include <malloc.h>

#define MPI_INFO(s,w) {PRINTF("WARNING: %s, will return" #w "\n",(s));}
#define MPI_WARNING(s,w) {PRINTF("WARNING: %s, returning" #w "\n",(s)); return (w);}
#define MPI_ERROR(s) {PRINTF("ERROR: %s; exiting\n",s); exit(1);}

#define CHECK_RANGE(r,max,fn,kind)  if(r < 0 || r >= max) \
{\
    char msg[100];\
    sprintf(msg, "%s: %s %d is out of range [0..%d)",fn, kind, r,max);\
    MPI_WARNING(msg, MPI_ERR_ARG);\
}\


// Have to make sure to drag in mainEdt else mpi_ocr.o does not get

// included in the linked object. MPI_Init must be called by all MPI
// programs, and it uses __mpi_ocr_TRUE from mpi_ocr.o, which will drag in
// the .o file.

int MPI_Init(int *argc, char ***argv)
{
    getRankContext()->mpiInitialized = __mpi_ocr_TRUE();
    return MPI_SUCCESS;
}

int MPI_Init_thread( int *argc, char ***argv, int required, int *provided )
{
    int ret = MPI_SUCCESS;

    *provided = MPI_THREAD_SINGLE;
    if (required > *provided) {
        MPI_WARNING("INFO MPI_Init_thread required higher threading model.\n", MPI_ERR_ARG);
    }
    MPI_Init(argc, argv);
    //A threaded MPI program that does not call MPI_Init_thread is an incorrect program
    return ret;
}

int MPI_Initialized(int *flag)
{
    *flag = getRankContext()->mpiInitialized;
    return MPI_SUCCESS;
}

int MPI_Finalize(void)
{
#if 0    // Only needed for debugging....
    rankContextP_t rankContext = getRankContext();
    const u32 rank = rankContext->rank;
    PRINTF("MPI_Finalize: rank #%d: Finalized!\n", rank);
#endif    // end of debugging

    getRankContext()->mpiInitialized = FALSE;
    return MPI_SUCCESS;
}

int MPI_Comm_rank(MPI_Comm comm, int *rank)
{
    *rank = getRankContext()->rank;
    return(MPI_SUCCESS);
}

int MPI_Comm_size(MPI_Comm comm, int *size)
{
    *size = getRankContext()->numRanks;
    return(MPI_SUCCESS);
}

static inline u64 index(int source, int dest, int tag, int numRanks, int maxTag)
{
    return ((numRanks*source + dest)*(maxTag+1) + tag);
}

static char * get_op_string(MPI_Request req)
{
    if (req == MPI_REQUEST_NULL)
        return "null";

    switch(req->op)
    {
      case OP_ISEND:
        return "isend";
      case OP_IRECV:
        return "irecv";
      case OP_IPROBE:
        return "iprobe";
      default:
        return "unknown";
    }
}


static inline void initRequest(MPI_Request p, int op, int count, int datatype, int tag, int rank, int comm, void *buf)
{
    p->count = count;
    p->op = op;
    p->datatype = datatype;
    p->tag = tag;
    p->rank = rank;
    p->comm = comm;
    p->flag = 0;
    p->status = FFWD_MQ_INIT;
    p->buf = buf;
}

// Isend: fill out the request, but do nothing else. A later MPI_Test or
// MPI_Wait* will caus the send to occur
int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request *request)

{
#if 0    // Only needed for debugging....
    rankContextP_t rankContext = getRankContext();
    const u32 source = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;
    const u32 maxTag = rankContext->maxTag;
    PRINTF("MPI_Isend: rank #%d: Sending on index %d\n", source,
           index(source, dest, tag, numRanks, maxTag));
#endif    // end of debugging

    *request = (MPI_Request)malloc(sizeof(**request));
    initRequest(*request, OP_ISEND, count, datatype, tag, dest, comm, buf);

    return(MPI_SUCCESS);
}

// Do a send.
int MPI_Send(void *buf,int count, MPI_Datatype
              datatype, int dest, int tag, MPI_Comm comm)
{
    // TBD: use comm to determine real "rank"

    rankContextP_t rankContext = getRankContext();
    const u32 source = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;
    const u32 maxTag = rankContext->maxTag;
    const u64 totalSize = count * rankContext->sizeOf[datatype];

    //PRINTF("MPI_Send: rank #%d: Sending on index %d\n", source, index(source, dest, tag, numRanks, maxTag));
    CHECK_RANGE(dest, numRanks, "MPI_Send", "dest");
    CHECK_RANGE(tag, maxTag+1, "MPI_Send", "tag");


    messageContextP_t messageContext = getMessageContext();

    // need volatile so while loop keeps loading each iteration
    volatile ocrGuid_t *eventP =
        &(messageContext->messageEvents[index(source, dest, tag, numRanks, maxTag)]);

    while (NULL_GUID != *eventP);  // wait till slot is not busy

    // OK, the location is free: time to create the event and DB
    ocrEventCreate((ocrGuid_t *)eventP, OCR_EVENT_STICKY_T, TRUE);

    // Have to put the DB into a parallel array, because ocrWait does not
    // yet return the ptr value, so recipient can't get to the data!!!
    // Thus it has to be accessed from here by the receiver, who can check
    // that the DB guid delivered by ocrWait == the guid stashed in the array.
    ocrEdtDep_t *dataP =
        &(messageContext->messageData[index(source, dest, tag, numRanks, maxTag)]);

    mpiOcrMessageP_t ptr;

    ocrDbCreate(&(dataP->guid), (void **)&ptr, totalSize + sizeof(mpiOcrMessage_t),
                DB_PROP_NONE, NULL_GUID, NO_ALLOC);
    dataP->ptr = (void *)ptr;

    // fill message header
    ptr->header.count = count;
    ptr->header.datatype = datatype;
    ptr->header.source = source;
    ptr->header.dest = dest;
    ptr->header.tag = tag;
    ptr->header.comm = comm;
    ptr->header.totalSize = totalSize;

    // copy buf starting at .data
    memcpy(&(ptr->data), buf, totalSize);

    ocrDbRelease(dataP->guid); // make sure it's visible to receiver

    // OK, Send the DB. the Satisfy should also release it (?).
    ocrEventSatisfy(*eventP, dataP->guid);

    return MPI_SUCCESS;
}

// Irecv: fill out the request, but do nothing else. A later MPI_Test or
// MPI_Wait* will cause the recv to occur
int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
              int tag, MPI_Comm comm, MPI_Request *request)
{
    *request = (MPI_Request)malloc(sizeof(**request));
    initRequest(*request, OP_IRECV, count, datatype, tag, source, comm, buf);

    return MPI_SUCCESS;
}


int MPI_Recv(void *buf,int count, MPI_Datatype
              datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
{
    // TBD: use comm to determine real "rank"
    int ret = MPI_SUCCESS;

    rankContextP_t rankContext = getRankContext();
    const u32 dest = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;
    const u32 maxTag = rankContext->maxTag;
    const u64 totalSize = count * rankContext->sizeOf[datatype];

    messageContextP_t messageContext = getMessageContext();
    ocrGuid_t *eventP;

    // This large IF stmt will keep looking for the appropriate event to
    // wait on, and when the if completes, eventP will point at the non
    // NULL_GUID
    if (!(MPI_ANY_SOURCE == source || MPI_ANY_TAG == tag)) {

        CHECK_RANGE(source, numRanks, "MPI_Recv", "source");
        CHECK_RANGE(tag, maxTag+1, "MPI_Recv", "tag");

        // need volatile so while loop keeps loading each iteration
        volatile ocrGuid_t *vEventP =
            &(messageContext->messageEvents[index(source, dest, tag, numRanks, maxTag)]);

        while (NULL_GUID == *vEventP);  // wait till slot has event guid

        // OK, the location is full: time to wait on the event
        eventP = (ocrGuid_t *)vEventP;

    } else { // *ANY* for source, tag, or both

        int done = FALSE;  // used by one of the while loops below
        if (MPI_ANY_SOURCE == source && MPI_ANY_TAG == tag){

            while(!done){
                for (u32 source = 0; source < numRanks; source++) {
                    for (u32 tag = 0; tag <= maxTag; tag++){
                        eventP =
                            &(messageContext->messageEvents[index(source, dest, tag, numRanks, maxTag)]);
                        if (NULL_GUID != *eventP) {
                            done = TRUE;
                            break;
                        }
                    }
                    if (done) {
                        break;
                    }
                }
            }  // end while
        } // end both use ANY
        else if (MPI_ANY_SOURCE == source) {

            CHECK_RANGE(tag, maxTag+1, "MPI_Recv", "tag");

            while(!done){
                for (u32 source = 0; source < numRanks; source++) {
                    eventP =
                        &(messageContext->messageEvents[index(source, dest, tag, numRanks, maxTag)]);
                    if (NULL_GUID != *eventP){
                        done = TRUE;
                        break;
                    }
                }
            }

        } else { // ANY_TAG

            CHECK_RANGE(source, numRanks, "MPI_Recv", "source");

            while(!done){
                for (u32 tag = 0; tag <= maxTag; tag++){
                    eventP =
                        &(messageContext->messageEvents[index(source, dest, tag, numRanks, maxTag)]);
                    if (NULL_GUID != *eventP){
                        done = TRUE;
                        break;
                    }
                }
            }

        }
        //#endif
    }
    // Here *eventP is not NULL_GUID, so something to wait on
    ocrGuid_t DB; //= ocrWait(*eventP);
    void *myPtr;
    ocrGuid_t newDB;
    u64 dbSize;

    ocrLegacyBlockProgress(*eventP, &DB, &myPtr, &dbSize);
    //assert(DB==newDB);



    // The DB is in a parallel array, because ocrWait does not
    // yet return the ptr value, so recipient can't get to the data!!!
    // Thus it has to be accessed from here by the receiver, who can check
    // that the DB guid delivered by ocrWait == the guid stashed in the array.
    ocrEdtDep_t *dataP =
        &(messageContext->messageData[index(source, dest, tag, numRanks, maxTag)]);
    //    assert(dataP == myPtr);

    ocrGuid_t receivedGuid = dataP->guid;

    if (DB != receivedGuid)
        {
            char msg[100];
            sprintf((char *)&msg, "MPI_Recv: datablock from ocrWait %llx != datablock in array %llx ", DB, receivedGuid);
            MPI_WARNING(msg, MPI_ERR_INTERN);
        }

    mpiOcrMessageP_t ptr = (mpiOcrMessageP_t) dataP->ptr;

    assert(ptr == myPtr);

    // Destroy event and set array entry to NULL_GUID so sender can send
    // again.

    // OK< now we are finished with the ocrEdtDep_t in the data array, as
    // well as the event.
    // So we can delete the event, replace the array event entry with NULL_GUID
    // so the sender can send again, which also means the array data element is
    // available. Note: we are not done with the db, but we have extracted
    // the ptr and guid from the data array.

    ocrEventDestroy(*eventP);
    *eventP = NULL_GUID;

    // Check message header components against the args. If they're wrong,
    // then the system got the wrong message!

    // checked explicitly below    ptr->header.count = count;
    // OK if user is dishonest about datatype as long as the recv size
    // isn't smaller than send size
    // ptr->header.datatype != datatype;

    assert(ptr->header.source == source || MPI_ANY_SOURCE == source);
    assert(ptr->header.dest == dest);
    assert(ptr->header.tag == tag || MPI_ANY_TAG == tag);

    // comm probably doesn't matter as long as src/dest match
    // ptr->header.comm      != comm;
    // OK, checked by count comparison ptr->header.totalSize != totalSize;

    u64 sizeToCopy = MIN(ptr->header.totalSize, totalSize);

    // Make sure receiving buffer is big enough, else warning with truncation
    if (count < ptr->header.count)
        {
            char msg[100];
            sprintf((char*)&msg, "MPI_Recv: count %d < message count %d ", count, ptr->header.count);
            MPI_INFO(msg, ret = MPI_ERR_TRUNCATE);
        }
    else if (totalSize < ptr->header.totalSize)
        {
            char msg[100];
            sprintf((char*)&msg, "MPI_Recv: buf size %d < message size %d ", totalSize,
                    ptr->header.totalSize);
            MPI_INFO(msg, ret = MPI_ERR_TRUNCATE);
        }

    // copy into buf starting at .data
    memcpy(buf, &(ptr->data), sizeToCopy);

    if (status != MPI_STATUS_IGNORE)
        {
            // fill in status struct
            status->mq_status.count    = ptr->header.count;
            status->mq_status.datatype = ptr->header.datatype;
            status->mq_status.source   = ptr->header.source;
            status->mq_status.tag      = ptr->header.tag;
        }

    // OK, finished with DB, delete it
    ocrDbDestroy(receivedGuid);

    return ret;
}

int MPI_Sendrecv(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                 int dest, int sendtag,
                 void *recvbuf, int recvcount, MPI_Datatype recvtype,
                 int source, int recvtag,
                 MPI_Comm comm, MPI_Status *status)
{
    int retSend = MPI_Send(sendbuf, sendcount, sendtype, dest, sendtag, comm);
    int retRecv = MPI_Recv(recvbuf, recvcount, recvtype, source, recvtag,
                           comm, status);
    if (MPI_SUCCESS == retSend){
        return retRecv;
    } else {
        return retSend;
    }

}

int MPI_Wait(MPI_Request *request, MPI_Status *status)
{
    // for debugging
    rankContextP_t rankContext = getRankContext();
    const u32 rank = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;
    const u32 maxTag = rankContext->maxTag;



    MPI_Request r;

    if (NULL == request || NULL == (r = *request))
        {
            //MPI_REQUEST_NULL is legitimate.
            //MPI_WARNING("MPI_Wait: bad value for request", MPI_ERR_REQUEST);
            return MPI_SUCCESS;
        }

    //PRINTF("MPI_Wait: rank #%d: request->op=%s\n", rank, get_op_string(r));

    int ret = MPI_SUCCESS;
    switch(r->op)
        {
        case OP_ISEND:
            {
                ret = MPI_Send(r->buf,r-> count,r-> datatype, r->rank, r->tag,
                               r->comm);
                break;
            }
        case OP_IRECV:
            {
                ret = MPI_Recv(r->buf,r-> count,r-> datatype, r->rank, r->tag, r->comm, status);
                break;
            }
        case OP_IPROBE:
            {
                free(r);
                r = MPI_REQUEST_NULL;

                // does a return
                MPI_WARNING("MPI_Wait: MPI_Probe NYI",MPI_ERR_INTERN);
            }
        default:
            {
                free(r);
                r = MPI_REQUEST_NULL;
                // does a return
                MPI_WARNING("MPI_Wait: unknown request operation type",MPI_ERR_REQUEST);
            }
        }

    free (r);
    r = MPI_REQUEST_NULL;
    return ret;
}

int MPI_Waitall(int count, MPI_Request *array_of_requests,
    MPI_Status *array_of_statuses)
{
    u32 i;

    //PRINTF("MPI_Waitall: %d aor %p aos %p\n", count, array_of_requests, array_of_statuses);

    if (array_of_statuses == MPI_STATUSES_IGNORE)
        for(i=0;i<count;i++) {
            MPI_Wait(&array_of_requests[i], MPI_STATUS_IGNORE);
        }
    else
        for(i=0;i<count;i++) {
            MPI_Wait(&array_of_requests[i], &array_of_statuses[i]);
        }

    return MPI_SUCCESS;
}


int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
{
    *flag = TRUE;
    return MPI_Wait(request, status);
}

int MPI_Testall(int count, MPI_Request *array_of_requests, int *flag,
                MPI_Status *array_of_statuses)
{
    int i;
    int temp;
#ifdef DEBUG_MPI
    printf("testall: %d aor %p aos %p\n", count, array_of_requests, array_of_statuses);
#endif
    *flag = 1;  // True if all requests have completed; false otherwise (logical)
    if (array_of_statuses == MPI_STATUSES_IGNORE)
        for(i=0;i<count;i++) {
            MPI_Test(&array_of_requests[i], &temp, MPI_STATUS_IGNORE);
            *flag = *flag && temp;
        }
    else
        for(i=0;i<count;i++) {
            MPI_Test(&array_of_requests[i], &temp, &array_of_statuses[i]);
            *flag = *flag && temp;
        }
    return MPI_SUCCESS;
}


int MPI_Get_count(
                  MPI_Status *status,
                  MPI_Datatype datatype,
                  int *count )
{
    if (datatype != status->mq_status.datatype)
        {
            *count = MPI_UNDEFINED;
            MPI_WARNING("MPI_Get_count: datatype differs between status and datatype arguments", MPI_ERR_TYPE);
        }

    if (status == MPI_STATUS_IGNORE) {
        *count = MPI_UNDEFINED;
        MPI_WARNING("MPI_Get_count status MPI_STATUS_IGNORE?", MPI_ERR_ARG);
    }
    *count = status->mq_status.count;
    return MPI_SUCCESS;
}


int MPI_Reduce(void *sendbuf, void *recvbuf, int count,
    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{

  // TBD: use comm to determine real "rank"
  //  const u32 rank = comm->group->rank;
  //  const u32 numRanks = comm->group->size;


    rankContextP_t rankContext = getRankContext();
    const u32 rank = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;
    const u32 maxTag = rankContext->maxTag;
    const u64 totalSize = count * rankContext->sizeOf[datatype];


    if (sendbuf == MPI_IN_PLACE) {
        sendbuf = recvbuf;
    }

    if (root == rank) {
        u32 i, j;
        memcpy(recvbuf, sendbuf, totalSize);

        void *p = malloc(totalSize);    // temporary buffer for recv
        if (!p)
            MPI_ERROR("Temporary buffer allocation for MPI_Reduce failed.");

        for(i=0;i<numRanks;i++) {
            if (i==root)
                continue;
            MPI_Recv(p, count, datatype, i, 0 /*tag*/, comm, MPI_STATUS_IGNORE);

            if (datatype == MPI_INT) {
                int *a = recvbuf;
                int *b = p;
                if (op == MPI_SUM) {
                    for(j=0;j<count;j++) {
                        a[j] += b[j];
                    }
                } else if (op == MPI_MIN) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]>b[j]) ? b[j] : a[j];
                    }
                } else if (op == MPI_MAX) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]<b[j]) ? b[j] : a[j];
                    }
// no MINLOC/MAXLOC
                } else if (op == MPI_PROD) {
                    for(j=0;j<count;j++) {
                        a[j] *= b[j];
                    }
                } else if (op == MPI_LAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] && b[j];
                    }
                } else if (op == MPI_BAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] & b[j];
                    }
                } else if (op == MPI_LOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] || b[j];
                    }
                } else if (op == MPI_BOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] | b[j];
                    }
                } else if (op == MPI_LXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j] && b[j]) || (!a[j] && !b[j]);
                                        }
                } else if (op == MPI_BXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] ^ b[j];
                    }
                } else {
                    char msg[100];
                    sprintf((char *)&msg, "MPI_Reduce: Unsupported MPI_Op: %d, type: %d\n", op, datatype);
                    MPI_ERROR(msg);
                }
            } else if (datatype == MPI_LONG) {
                long *a = recvbuf;
                long *b = p;
                if (op == MPI_SUM) {
                    for(j=0;j<count;j++) {
                        a[j] += b[j];
                    }
                } else if (op == MPI_MIN) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]>b[j]) ? b[j] : a[j];
                    }
                } else if (op == MPI_MAX) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]<b[j]) ? b[j] : a[j];
                    }
// no MINLOC/MAXLOC
                } else if (op == MPI_PROD) {
                    for(j=0;j<count;j++) {
                        a[j] *= b[j];
                    }
                } else if (op == MPI_LAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] && b[j];
                    }
                } else if (op == MPI_BAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] & b[j];
                    }
                } else if (op == MPI_LOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] || b[j];
                    }
                } else if (op == MPI_BOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] | b[j];
                    }
                } else if (op == MPI_LXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j] && b[j]) || (!a[j] && !b[j]);
                    }
                } else if (op == MPI_BXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] ^ b[j];
                    }
                } else {
                    char msg[100];
                    sprintf((char *)&msg, "MPI_Reduce: Unsupported MPI_Op: %d, type: %d\n", op, datatype);
                    MPI_ERROR(msg);
                }
            } else if (datatype == MPI_LONG_LONG || datatype == MPI_LONG_LONG_INT) {
                long long *a = recvbuf;
                long long *b = p;
                if (op == MPI_SUM) {
                    for(j=0;j<count;j++) {
                        a[j] += b[j];
                    }
                } else if (op == MPI_MIN) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]>b[j]) ? b[j] : a[j];
                    }
                } else if (op == MPI_MAX) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]<b[j]) ? b[j] : a[j];
                    }
// no MINLOC/MAXLOC
                } else if (op == MPI_PROD) {
                    for(j=0;j<count;j++) {
                        a[j] *= b[j];
                    }
                } else if (op == MPI_LAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] && b[j];
                    }
                } else if (op == MPI_BAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] & b[j];
                    }
                } else if (op == MPI_LOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] || b[j];
                    }
                } else if (op == MPI_BOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] | b[j];
                    }
                } else if (op == MPI_LXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j] && b[j]) || (!a[j] && !b[j]);
                    }
                } else if (op == MPI_BXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] ^ b[j];
                    }
                } else {
                    char msg[100];
                    sprintf((char *)&msg, "MPI_Reduce: Unsupported MPI_Op: %d, type: %d\n", op, datatype);
                    MPI_ERROR(msg);
                }
            } else if (datatype == MPI_UNSIGNED) {
                unsigned *a = recvbuf;
                unsigned *b = p;
                if (op == MPI_SUM) {
                    for(j=0;j<count;j++) {
                        a[j] += b[j];
                    }
                } else if (op == MPI_MIN) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]>b[j]) ? b[j] : a[j];
                    }
                } else if (op == MPI_MAX) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]<b[j]) ? b[j] : a[j];
                    }
// no MINLOC/MAXLOC
                } else if (op == MPI_PROD) {
                    for(j=0;j<count;j++) {
                        a[j] *= b[j];
                    }
                } else if (op == MPI_LAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] && b[j];
                    }
                } else if (op == MPI_BAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] & b[j];
                    }
                } else if (op == MPI_LOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] || b[j];
                    }
                } else if (op == MPI_BOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] | b[j];
                    }
                } else if (op == MPI_LXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j] && b[j]) || (!a[j] && !b[j]);
                    }
                } else if (op == MPI_BXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] ^ b[j];
                    }
                } else {
                    char msg[100];
                    sprintf((char *)&msg, "MPI_Reduce: Unsupported MPI_Op: %d, type: %d\n", op, datatype);
                    MPI_ERROR(msg);
                }
            } else if (datatype == MPI_UNSIGNED_LONG) {
                unsigned long *a = recvbuf;
                unsigned long *b = p;
                if (op == MPI_SUM) {
                    for(j=0;j<count;j++) {
                        a[j] += b[j];
                    }
                } else if (op == MPI_MIN) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]>b[j]) ? b[j] : a[j];
                    }
                } else if (op == MPI_MAX) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]<b[j]) ? b[j] : a[j];
                    }
// no MINLOC/MAXLOC
                } else if (op == MPI_PROD) {
                    for(j=0;j<count;j++) {
                        a[j] *= b[j];
                    }
                } else if (op == MPI_LAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] && b[j];
                    }
                } else if (op == MPI_BAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] & b[j];
                    }
                } else if (op == MPI_LOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] || b[j];
                    }
                } else if (op == MPI_BOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] | b[j];
                    }
                } else if (op == MPI_LXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j] && b[j]) || (!a[j] && !b[j]);
                    }
                } else if (op == MPI_BXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] ^ b[j];
                    }
                } else {
                    char msg[100];
                    sprintf((char *)&msg, "MPI_Reduce: Unsupported MPI_Op: %d, type: %d\n", op, datatype);
                    MPI_ERROR(msg);
                }
            } else if (datatype == MPI_UNSIGNED_LONG_LONG) {
                unsigned long long *a = recvbuf;
                unsigned long long *b = p;
                if (op == MPI_SUM) {
                    for(j=0;j<count;j++) {
                        a[j] += b[j];
                    }
                } else if (op == MPI_MIN) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]>b[j]) ? b[j] : a[j];
                    }
                } else if (op == MPI_MAX) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]<b[j]) ? b[j] : a[j];
                    }
// no MINLOC/MAXLOC
                } else if (op == MPI_PROD) {
                    for(j=0;j<count;j++) {
                        a[j] *= b[j];
                    }
                } else if (op == MPI_LAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] && b[j];
                    }
                } else if (op == MPI_BAND) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] & b[j];
                    }
                } else if (op == MPI_LOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] || b[j];
                    }
                } else if (op == MPI_BOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] | b[j];
                    }
                } else if (op == MPI_LXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j] && b[j]) || (!a[j] && !b[j]);
                    }
                } else if (op == MPI_BXOR) {
                    for(j=0;j<count;j++) {
                        a[j] = a[j] ^ b[j];
                    }
                } else {
                    char msg[100];
                    sprintf((char *)&msg, "MPI_Reduce: Unsupported MPI_Op: %d, type: %d\n", op, datatype);
                    MPI_ERROR(msg);
                }
            } else if (datatype == MPI_FLOAT) {
                float *a = recvbuf;
                float *b = p;
                if (op == MPI_SUM) {
                    for(j=0;j<count;j++) {
                        a[j] += b[j];
                    }
                } else if (op == MPI_MIN) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]>b[j]) ? b[j] : a[j];
                    }
                } else if (op == MPI_MAX) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]<b[j]) ? b[j] : a[j];
                    }
// no MINLOC/MAXLOC
                } else if (op == MPI_PROD) {
                    for(j=0;j<count;j++) {
                        a[j] *= b[j];
                    }
// no LAND/BAND/LOR/BOR/LXOR/BXOR
                } else {
                    char msg[100];
                    sprintf((char *)&msg, "MPI_Reduce: Unsupported MPI_Op: %d, type: %d\n", op, datatype);
                    MPI_ERROR(msg);
                }
            } else if (datatype == MPI_DOUBLE) {
                double *a = recvbuf;
                double *b = p;
                if (op == MPI_SUM) {
                    for(j=0;j<count;j++) {
                        a[j] += b[j];
                    }
                } else if (op == MPI_MIN) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]>b[j]) ? b[j] : a[j];
                    }
                } else if (op == MPI_MAX) {
                    for(j=0;j<count;j++) {
                        a[j] = (a[j]<b[j]) ? b[j] : a[j];
                    }
// no MINLOC/MAXLOC
                } else if (op == MPI_PROD) {
                    for(j=0;j<count;j++) {
                        a[j] *= b[j];
                    }
// no LAND/BAND/LOR/BOR/LXOR/BXOR
                } else {
                    char msg[100];
                    sprintf((char *)&msg, "MPI_Reduce: Unsupported MPI_Op: %d, type: %d\n", op, datatype);
                    MPI_ERROR(msg);
                }
            } else if (datatype == MPI_DOUBLE_INT) {
                double_int *a = recvbuf;
                double_int *b = p;
                if (op == MPI_MINLOC) {
                    for(j=0;j<count;j++) {
                        if (a[j].a > b[j].a) {
                            a[j].a = b[j].a;
                            a[j].b = b[j].b;
                        } else if (a[j].a == b[j].a) {  // get min index if equals
                            if (a[j].b > b[j].b)
                                a[j].b = b[j].b;
                        }
                    }
                } else if (op == MPI_MAXLOC) {
                    for(j=0;j<count;j++) {
                        if (a[j].a < b[j].a) {
                            a[j].a = b[j].a;
                            a[j].b = b[j].b;
                        } else if (a[j].a == b[j].a) {  // get min index if equal
                            if (a[j].b > b[j].b)
                                a[j].b = b[j].b;
                        }
                    }
                } else {
                    char msg[100];
                    sprintf((char *)&msg, "MPI_Reduce: Unsupported MPI_Op: %d, type: %d\n", op, datatype);
                    MPI_ERROR(msg);
                }
            } else {
                char msg[100];
                sprintf((char *)&msg, "MPI_Reduce: Unsupported MPI_Op: %d, type: %d\n", op, datatype);
                MPI_ERROR(msg);
            }
        } // for

        free(p);
    } else {
        MPI_Send(sendbuf, count, datatype, root, 0 /*tag*/, comm);
    }

    return MPI_SUCCESS;
}

#define ALLREDUCE_ROOT 0
int MPI_Allreduce ( void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{

    if (sendbuf == MPI_IN_PLACE) {
        sendbuf = recvbuf;
    }

    MPI_Reduce(sendbuf, recvbuf, count, datatype, op, ALLREDUCE_ROOT, comm);
    MPI_Bcast(recvbuf, count, datatype, ALLREDUCE_ROOT, comm);

    return MPI_SUCCESS;
}


#if 1   // this works
int MPI_Bcast (void *buffer, int count, MPI_Datatype datatype, int root,
               MPI_Comm comm )
{
    // TBD: use comm to determine real "rank"
    //  const u32 rank = comm->group->rank;
    //  const u32 numRanks = comm->group->size;

    rankContextP_t rankContext = getRankContext();
    const u32 rank = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;

    if (numRanks == 1)  // e.g. comm_self
        return MPI_SUCCESS;

    //PRINTF("MPI_Bcast buffer %p, count %d, type %d, rank %d\n", buffer,
    //       count, datatype, rank);

    if (root >= numRanks) {
        MPI_ERROR("MPI_Bcast: root >= group size? invalid rank\n");
    }
    if (root == rank) {
        u32 i;
        for(i=0;i<numRanks;i++) {
            if (i != root) {
                MPI_Send(buffer, count, datatype, i, 0 /* tag */, comm);
            }
        }
    } else {
        MPI_Recv(buffer, count, datatype, root, 0 /* tag */, comm, MPI_STATUS_IGNORE);
    }
    return MPI_SUCCESS;
}
#else  //this works, too.
int MPI_Bcast (void *buffer, int count, MPI_Datatype datatype, int root,
               MPI_Comm comm )
{
    // TBD: use comm to determine real "rank"
    //  const u32 rank = comm->group->rank;
    //  const u32 numRanks = comm->group->size;
    rankContextP_t rankContext = getRankContext();
    const u32 rank = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;


    if (numRanks == 1)  // e.g. comm_self
        return MPI_SUCCESS;

    //PRINTF("MPI_Bcast buffer %p , count %d , type %d, root %d\n", buffer,
    //       count, datatype, root);

    if (root >= numRanks) {
        MPI_ERROR("MPI_Bcast: root >= group size? invalid rank\n");
    }
    if (root == rank) {
        u32 i;
        MPI_Request req[numRanks];
        for(i=0;i<numRanks;i++) {
            if (i != root) {
                MPI_Isend(buffer, count, datatype, i, 0 /* tag */, comm, &req[i]);
            } else {
                req[i] = (MPI_Request)MPI_REQUEST_NULL;
            }
        }
        MPI_Waitall(numRanks, req, MPI_STATUSES_IGNORE);

    } else {
        MPI_Recv(buffer, count, datatype, root, 0 /* tag */, comm, MPI_STATUS_IGNORE);

    }
    return MPI_SUCCESS;
}
#endif


int MPI_Barrier(MPI_Comm comm)
{
    long buf[1];
    long sum=0;
    long one=1;

    MPI_Reduce(&sum, &one, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(buf, 1, MPI_LONG, 0 /*root*/, MPI_COMM_WORLD);

    return MPI_SUCCESS;
}

int MPI_Scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
                MPI_Comm comm)
{
    rankContextP_t rankContext = getRankContext();
    const u32 rank = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;
    const u64 sendSize = sendcount * rankContext->sizeOf[sendtype];
    const u64 recvSize = recvcount * rankContext->sizeOf[recvtype];

    if (recvSize < sendSize) {
        char msg[100];
        sprintf((char*)&msg, "MPI_Scatter: recv size %d < send size %d ", recvSize, sendSize);
        MPI_WARNING(msg, MPI_ERR_TRUNCATE);
    }
    if (root == rank) {
         void *target = (char *)recvbuf;
        memcpy(target, sendbuf + sendSize*root, sendSize);

        for(int i=0; i<numRanks; i++) {
            if (i == root)
                continue;
            void *p = (char *)sendbuf + sendSize*i;
            MPI_Send(p, sendcount, sendtype, i, 0 /* i tag*/, comm);
        }
    } else {
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, MPI_STATUS_IGNORE);
    }
    return MPI_SUCCESS;
}

int MPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs,
                 MPI_Datatype sendtype, void *recvbuf, int recvcount,
                 MPI_Datatype recvtype,
                 int root, MPI_Comm comm)
{
    rankContextP_t rankContext = getRankContext();
    const u32 rank = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;
    const u64 sendSize =             rankContext->sizeOf[sendtype];
    const u64 recvSize = recvcount * rankContext->sizeOf[recvtype];

    if (root == rank) {
        void *target = (char *)recvbuf;
        memcpy(target, sendbuf + sendSize * displs[root], sendSize * sendcounts[root]);

        for(int i=0; i<numRanks; i++) {
            if (i == root)
                continue;
            void *p = (char *)sendbuf + sendSize * displs[i];
            MPI_Send(p, sendcounts[i], sendtype, i, 0 /*i tag*/, comm);
        }
    } else {
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, MPI_STATUS_IGNORE);
    }
    return MPI_SUCCESS;
}

int MPI_Gather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
               MPI_Comm comm)
{
    rankContextP_t rankContext = getRankContext();
    const u32 rank = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;
    const u64 sendSize = sendcount * rankContext->sizeOf[sendtype];
    const u64 recvSize = recvcount * rankContext->sizeOf[recvtype];

    if (recvSize < sendSize) {
        char msg[100];
        sprintf((char*)&msg, "MPI_Gather: recv size %d < send size %d ", recvSize, sendSize);
        MPI_WARNING(msg, MPI_ERR_TRUNCATE);
    }

    if (root == rank) {
        void *target = (char *)recvbuf + recvSize*root;
        memcpy(target, sendbuf, sendSize);

        for(int i=0; i<numRanks; i++) {
            if (i == root)
                continue;
            void *p = (char *)recvbuf + recvSize*i;
            MPI_Recv(p, recvcount, recvtype, i, 0 /* i tag*/, comm, MPI_STATUS_IGNORE);
        }
    } else {
        MPI_Send(sendbuf, sendcount, sendtype, root, 0, comm);
    }
    return MPI_SUCCESS;
}

int MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, const int *recvcounts, const int *displs,
                MPI_Datatype recvtype, int root, MPI_Comm comm)
{
    rankContextP_t rankContext = getRankContext();
    const u32 rank = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;
    const u64 sendSize = sendcount * rankContext->sizeOf[sendtype];
    const u64 recvSize =             rankContext->sizeOf[recvtype];

    if (root == rank) {
        void *target = (char *)recvbuf + recvSize * displs[root];
        memcpy(target, sendbuf, sendSize);

        for(int i=0; i<numRanks; i++) {
            if (i == root)
                continue;
            void *p = (char *)recvbuf + recvSize * displs[i];
            MPI_Recv(p, recvcounts[i], recvtype, i, 0 /* i tag*/, comm, MPI_STATUS_IGNORE);
        }
    } else {
        MPI_Send((void *)sendbuf, sendcount, sendtype, root, 0, comm);
    }
    return MPI_SUCCESS;
}

#define ALLGATHER_ROOT  0
int MPI_Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                   void *recvbuf, const int *recvcounts, const int *displs,
                   MPI_Datatype recvtype, MPI_Comm comm)
{
    rankContextP_t rankContext = getRankContext();
    const u32 numRanks = rankContext->numRanks;

    MPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, ALLGATHER_ROOT, comm);

    // total bcast count is displacement start of last rank + count of the
    // last rank
    MPI_Bcast(recvbuf, displs[numRanks-1]+recvcounts[numRanks-1] /* assumes..  */ , recvtype, ALLGATHER_ROOT, comm);
    return MPI_SUCCESS;
}



int MPI_Allgather(void *sendbuf, int  sendcount,
                  MPI_Datatype sendtype, void *recvbuf, int recvcount,
                  MPI_Datatype recvtype, MPI_Comm comm)
{
    rankContextP_t rankContext = getRankContext();
    const u32 numRanks = rankContext->numRanks;

    MPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, ALLGATHER_ROOT, comm);
    MPI_Bcast(recvbuf, recvcount*numRanks, recvtype, ALLGATHER_ROOT, comm);
    return MPI_SUCCESS;
}


int MPI_Alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                 void *recvbuf, int recvcount, MPI_Datatype recvtype,
                 MPI_Comm comm)
{
    rankContextP_t rankContext = getRankContext();
    const u32 rank = rankContext->rank;
    const u32 numRanks = rankContext->numRanks;
    const u64 sendSize = sendcount * rankContext->sizeOf[sendtype];
    const u64 recvSize = recvcount * rankContext->sizeOf[recvtype];

    // Send before receiving so don't hang!
    for(int i=0; i<numRanks; i++) {
        if (i == rank) {
        } else {
            MPI_Send((void *)sendbuf, sendcount, sendtype, i, 0 /* i */, comm);
        }
        sendbuf = (char *)sendbuf + sendSize;
    }
    for(int i=0; i<numRanks; i++) {
        if (i == rank) {
            memcpy(recvbuf, (char*)sendbuf + sendSize*i, recvSize);
        } else {
            MPI_Recv(recvbuf, recvcount, recvtype, i, 0 /* rank */, comm, MPI_STATUS_IGNORE);
        }
        recvbuf = (char *)recvbuf + recvSize;
    }
    return MPI_SUCCESS;

}

int MPI_Abort(MPI_Comm comm, int errorcode)
{
    exit(errorcode);
    return 0;
}

double MPI_Wtime( void )	// Time in seconds since an arbitrary time in the past.
{
#if 0
    struct timeval now;
    int rc;
    double ret;
    rc = gettimeofday(&now, NULL);	// rc always 0 for now
    ret = (double) now.tv_sec + (double) now.tv_usec * 1.0e-6;
    return ret;
#endif
    // cheat, race condition, but will be monotonically increasing so
    // people won't get div/by/zero if they do ratios of time diffs
    static double ret = 0;

    return (ret += 0.001);
}

int MPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm)
{
    // TBD: cheat!!
    *newcomm = comm;

    return MPI_SUCCESS;
}
