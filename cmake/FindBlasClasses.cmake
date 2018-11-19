include(FindPackageHandleStandardArgs)

function(find_dmft_blas)
   set(include_search_paths)

   if (DEFINED BLAS_CLASSES_DIR)
      list(APPEND include_search_paths ${BLAS_CLASSES_DIR})
   endif()

   if (DEFINED DMFT_SRC_DIR)
      list(APPEND include_search_paths ${DMFT_SRC_DIR}/blas_classes)
   endif()

   if (DEFINED DMFT_DIR)
      list(APPEND include_search_paths ${DMFT_DIR}/include)
   endif()

   if (DEFINED ENV{DMFT_DIR})
      list(APPEND include_search_paths $ENV{DMFT_DIR}/include)
   endif()

   find_path(BLAS_CLASSES_INCLUDE_DIR blasheader.h PATHS ${include_search_paths} DOC "BLAS classes include path")

endfunction()

find_dmft_blas()
find_package_handle_standard_args(BlasClasses DEFAULT_MSG BLAS_CLASSES_INCLUDE_DIR)
