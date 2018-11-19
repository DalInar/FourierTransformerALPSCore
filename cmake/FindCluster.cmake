include(FindPackageHandleStandardArgs)

function(find_cluster)
   set(include_search_paths)
   set(library_search_paths)

   if (DEFINED LIBCLUSTER_DIR)
      list(APPEND include_search_paths ${LIBCLUSTER_DIR})
      list(APPEND library_search_paths ${LIBCLUSTER_DIR})
   endif()

   if (DEFINED DMFT_SRC_DIR)
      list(APPEND include_search_paths ${DMFT_SRC_DIR}/libcluster)
      list(APPEND library_search_paths ${DMFT_SRC_DIR}/libcluster)
   endif()

   if (DEFINED DMFT_BUILD_DIR)
      list(APPEND include_search_paths ${DMFT_BUILD_DIR}/../libcluster ${DMFT_BUILD_DIR}/libcluster)
      list(APPEND library_search_paths ${DMFT_BUILD_DIR}/libcluster)
   endif()

   if (DEFINED DMFT_DIR)
      list(APPEND include_search_paths ${DMFT_DIR}/include)
      list(APPEND library_search_paths ${DMFT_DIR}/lib)
   endif()

   if (DEFINED ENV{DMFT_DIR})
      list(APPEND include_search_paths $ENV{DMFT_DIR}/include)
      list(APPEND library_search_paths $ENV{DMFT_DIR}/lib)
   endif()

   find_path(CLUSTER_INCLUDE_DIR "cluster.h" PATHS ${include_search_paths} DOC "Cluster transform include path")
   find_library(CLUSTER_LIBRARIES "cluster" PATHS ${library_search_paths} DOC "Cluster transform library path")
   
endfunction()

#message(STATUS "Searching in: INCLUDE_SEARCH_PATHS=${INCLUDE_SEARCH_PATHS}")
#message(STATUS "Searching in: LIBRARY_SEARCH_PATHS=${LIBRARY_SEARCH_PATHS}")

find_cluster()

find_package_handle_standard_args(Cluster DEFAULT_MSG CLUSTER_INCLUDE_DIR CLUSTER_LIBRARIES)
