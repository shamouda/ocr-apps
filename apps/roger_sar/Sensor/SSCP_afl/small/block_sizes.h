#if 1
#define BACK_PROJ_ASYNC_BLOCK_SIZE 32
#define AFFINE_ASYNC_2_BLOCK_SIZE 32
#define CCD_ASYNC_BLOCK_SIZE 31
#define CFAR_ASYNC_BLOCK_SIZE 25
#else
#define BACK_PROJ_ASYNC_BLOCK_SIZE 4
#define AFFINE_ASYNC_2_BLOCK_SIZE 4
#define CCD_ASYNC_BLOCK_SIZE 4
#define CFAR_ASYNC_BLOCK_SIZE 5
#endif
