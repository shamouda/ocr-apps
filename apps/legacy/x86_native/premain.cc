#include <stdio.h>
#include <sys/ocr.h>
#include <assert.h>

//
// Stack unwinding support
//
// eh_frame_hdr and eh_frame list
// The sections are added by the ldscript and it creates
// the following symbols for access.
// Sizes may be 0.
//
typedef unsigned long * ulongptr_t;    // not provided by stdint.h

extern uint64_t __eh_frame_hdr_start;
extern uint64_t __eh_frame_hdr_end;
extern uint64_t __eh_frame_start;
extern uint64_t __eh_frame_end;

uint64_t __eh_frame_size;
uint64_t __eh_frame_hdr_size;

static struct eh_info {
    ulongptr_t hdr_start;
    ulongptr_t hdr_size;
    ulongptr_t frame_start;
    ulongptr_t frame_size;
} eh_object = {
    .hdr_start   = & __eh_frame_hdr_start,
    .hdr_size    = & __eh_frame_hdr_size,
    .frame_start = & __eh_frame_start,
    .frame_size  = & __eh_frame_size
};

_BEGIN_STD_C
void __get_eh_info( struct eh_info * info )
{
    __eh_frame_size = &__eh_frame_end - &__eh_frame_start;
    __eh_frame_hdr_size = &__eh_frame_hdr_end - &__eh_frame_hdr_start;

    *info = eh_object;
}
_END_STD_C

static struct _reent reent;
struct _reent *_get_reent()
{
    return & reent;
}

void ocrinit( int argc, char **argv, char **envp )
{
    ocrGuid_t   affinity;
    ocrConfig_t oc;
    //
    // initialize OCR
    //
    ocrParseArgs(argc, (const char **) argv, & oc);
    ocrInit(&_REENT->_ocr.legacyContext, & oc);
    //
    // get the affinity of this EDT
    //
    if (ocrAffinityGetCurrent(&affinity) != 0)
        assert(1);
}

void ocrfini( int argc, char **argv, char **envp )
{
    ocrFinalize(_REENT->_ocr.legacyContext);
}

__attribute__((section(".preinit_array"))) typeof(ocrinit) *__preinit = ocrinit;
__attribute__((section(".fini_array"))) typeof(ocrfini) *__fini = ocrfini;
