function(find_fftw3)
   set(include_search_paths)
   if (DEFINED ENV{FFTW3_DIR})
      list(APPEND include_search_paths $ENV{FFTW3_DIR}/include)
   endif()
   if (DEFINED FFTW3_DIR)
      list(APPEND include_search_paths ${FFTW3_DIR}/include)
   endif()
   
   set(library_search_paths)
   if (DEFINED ENV{FFTW3_DIR})
      list(APPEND library_search_paths $ENV{FFTW3_DIR}/lib)
   endif()
   if (DEFINED FFTW3_DIR)
      list(APPEND library_search_paths ${FFTW3_DIR}/lib)
   endif()
   
   #message("DEBUG: include_search_paths='${include_search_paths}' library_search_paths='${library_search_paths}'")

   find_path(FFTW3_INCLUDE_DIR fftw3.h ${include_search_paths} DOC "FFTW3 include path")
   find_library(FFTW3_LIBRARIES fftw3 ${library_search_paths} DOC "FFTW3 library path")

   #message("DEBUG: FFTW3_LIBRARIES='${FFTW3_LIBRARIES}' FFTW3_INCLUDE_DIR='${FFTW3_INCLUDE_DIR}'")
endfunction()

find_fftw3()
#message("DEBUG: FFTW3_LIBRARIES='${FFTW3_LIBRARIES}' FFTW3_INCLUDE_DIR='${FFTW3_INCLUDE_DIR}'")

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FFTW3 DEFAULT_MSG FFTW3_LIBRARIES FFTW3_INCLUDE_DIR)
