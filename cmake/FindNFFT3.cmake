function(find_nfft3)
   set(include_search_paths)
   if (DEFINED ENV{NFFT3_DIR})
      list(APPEND include_search_paths $ENV{NFFT3_DIR}/include)
   endif()
   if (DEFINED NFFT3_DIR)
      list(APPEND include_search_paths ${NFFT3_DIR}/include)
   endif()
   
   set(library_search_paths)
   if (DEFINED ENV{NFFT3_DIR})
      list(APPEND library_search_paths $ENV{NFFT3_DIR}/lib)
   endif()
   if (DEFINED NFFT3_DIR)
      list(APPEND library_search_paths ${NFFT3_DIR}/lib)
   endif()
   
   #message("DEBUG: include_search_paths='${include_search_paths}' library_search_paths='${library_search_paths}'")

   find_path(NFFT3_INCLUDE_DIR nfft3.h ${include_search_paths} DOC "NFFT3 include path")
   find_library(NFFT3_LIBRARIES nfft3 ${library_search_paths} DOC "NFFT3 library path")

   #message("DEBUG: NFFT3_LIBRARIES='${NFFT3_LIBRARIES}' NFFT3_INCLUDE_DIR='${NFFT3_INCLUDE_DIR}'")
endfunction()

find_nfft3()
#message("DEBUG: NFFT3_LIBRARIES='${NFFT3_LIBRARIES}' NFFT3_INCLUDE_DIR='${NFFT3_INCLUDE_DIR}'")
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NFFT3 DEFAULT_MSG NFFT3_LIBRARIES NFFT3_INCLUDE_DIR)
