include(CheckLibraryExists)
include(CheckCXXCompilerFlag)

if(LIBCXX_TARGET STREQUAL "XSTG" )
 message(STATUS "Configuring for XSTG")
 set(LIBCXX_HAS_STDCXX11_FLAG TRUE)
 set(LIBCXX_HAS_STDCXX1Y_FLAG TRUE)
 set(LIBCXX_HAS_FPIC_FLAG FALSE)
 set(LIBCXX_HAS_FNO_OMIT_FRAME_POINTER_FLAG TRUE)
 set(LIBCXX_HAS_FNODEFAULTLIBS_FLAG TRUE)
 set(LIBCXX_HAS_FNOSTDINCXX_FLAG TRUE)
 set(LIBCXX_HAS_WALL_FLAG TRUE)
 set(LIBCXX_HAS_W_FLAG TRUE)
 set(LIBCXX_HAS_WNO_UNUSED_PARAMETER_FLAG TRUE)
 set(LIBCXX_HAS_WWRITE_STRINGS_FLAG TRUE)
 set(LIBCXX_HAS_WNO_LONG_LONG_FLAG TRUE)
 set(LIBCXX_HAS_PEDANTIC_FLAG TRUE)
 set(LIBCXX_HAS_WERROR_FLAG TRUE)
 set(LIBCXX_HAS_WNO_ERROR_FLAG TRUE)
 set(LIBCXX_HAS_FNO_EXCEPTIONS_FLAG TRUE)
 set(LIBCXX_HAS_FNO_RTTI_FLAG TRUE)
 set(LIBCXX_HAS_GLINE_TABLES_ONLY_FLAG TRUE)
 set(LIBCXX_HAS_WX_FLAG FALSE)
 set(LIBCXX_HAS_NO_WX_FLAG FALSE)
 set(LIBCXX_HAS_EHSC_FLAG FALSE)
 set(LIBCXX_HAS_NO_EHS_FLAG FALSE)
 set(LIBCXX_HAS_NO_EHA_FLAG FALSE)
 set(LIBCXX_HAS_NO_GR_FLAG FALSE)
else()
# Check compiler flags
check_cxx_compiler_flag(-std=c++11              LIBCXX_HAS_STDCXX11_FLAG)
check_cxx_compiler_flag(-std=c++1y              LIBCXX_HAS_STDCXX1Y_FLAG)
check_cxx_compiler_flag(-fPIC                   LIBCXX_HAS_FPIC_FLAG)
check_cxx_compiler_flag(-fno-omit-frame-pointer LIBCXX_HAS_FNO_OMIT_FRAME_POINTER_FLAG)
check_cxx_compiler_flag(-nodefaultlibs          LIBCXX_HAS_NODEFAULTLIBS_FLAG)
check_cxx_compiler_flag(-nostdinc++             LIBCXX_HAS_NOSTDINCXX_FLAG)
check_cxx_compiler_flag(-Wall                   LIBCXX_HAS_WALL_FLAG)
check_cxx_compiler_flag(-W                      LIBCXX_HAS_W_FLAG)
check_cxx_compiler_flag(-Wno-unused-parameter   LIBCXX_HAS_WNO_UNUSED_PARAMETER_FLAG)
check_cxx_compiler_flag(-Wwrite-strings         LIBCXX_HAS_WWRITE_STRINGS_FLAG)
check_cxx_compiler_flag(-Wno-long-long          LIBCXX_HAS_WNO_LONG_LONG_FLAG)
check_cxx_compiler_flag(-pedantic               LIBCXX_HAS_PEDANTIC_FLAG)
check_cxx_compiler_flag(-Werror                 LIBCXX_HAS_WERROR_FLAG)
check_cxx_compiler_flag(-Wno-error              LIBCXX_HAS_WNO_ERROR_FLAG)
check_cxx_compiler_flag(-fno-exceptions         LIBCXX_HAS_FNO_EXCEPTIONS_FLAG)
check_cxx_compiler_flag(-fno-rtti               LIBCXX_HAS_FNO_RTTI_FLAG)
check_cxx_compiler_flag(-gline-tables-only      LIBCXX_HAS_GLINE_TABLES_ONLY_FLAG)
check_cxx_compiler_flag(/WX                     LIBCXX_HAS_WX_FLAG)
check_cxx_compiler_flag(/WX-                    LIBCXX_HAS_NO_WX_FLAG)
check_cxx_compiler_flag(/EHsc                   LIBCXX_HAS_EHSC_FLAG)
check_cxx_compiler_flag(/EHs-                   LIBCXX_HAS_NO_EHS_FLAG)
check_cxx_compiler_flag(/EHa-                   LIBCXX_HAS_NO_EHA_FLAG)
check_cxx_compiler_flag(/GR-                    LIBCXX_HAS_NO_GR_FLAG)

# Check libraries
check_library_exists(pthread pthread_create "" LIBCXX_HAS_PTHREAD_LIB)
check_library_exists(c printf "" LIBCXX_HAS_C_LIB)
check_library_exists(m ccos "" LIBCXX_HAS_M_LIB)
check_library_exists(rt clock_gettime "" LIBCXX_HAS_RT_LIB)
check_library_exists(gcc_s __gcc_personality_v0 "" LIBCXX_HAS_GCC_S_LIB)
endif()
