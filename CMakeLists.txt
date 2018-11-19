project (FourierTransformerALPSCore)
cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake) 

# Set flags for Release and Debug builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -march=native -O3 -DNDEBUG -DBOOST_DISABLE_ASSERTS -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -march=native -O0")

#include(EnableGtests)
find_package(ALPSCore REQUIRED COMPONENTS hdf5 accumulators mc params gf)
include(find_blas_lapack)
find_package(Cluster REQUIRED)
#find_package(FFTW3 REQUIRED)
#find_package(NFFT3 REQUIRED)
find_package(BlasClasses REQUIRED)

#include_directories(${GTEST_INCLUDE_DIR})
include_directories(${CLUSTER_INCLUDE_DIR})
include_directories(${BLAS_CLASSES_INCLUDE_DIR})
#include_directories(${NFFT3_INCLUDE_DIR})
#include_directories(${FFTW3_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -g -march=native -O3 -DNDEBUG -DBOOST_DISABLE_ASSERTS")

#add_library(ftac_lib)

add_executable(FT_ALPSCore ftac_main.cpp)

#add_executable(AccumulatorTest accumulatortestmain.cpp)

target_link_libraries(FT_ALPSCore
${CLUSTER_LIBRARIES} 
${ALPSCore_LIBRARIES}
 ${LAPACK_LIBRARIES}
)
#${NFFT3_LIBRARIES}
#${FFTW3_LIBRARIES}
#)
#target_link_libraries(FT_ALPSCore ftac_lib
#)
#target_link_libraries(AccumulatorTest ctaux_lib
#        )
# Build and enable tests
option(Testing "Enable testing" ON)
if (Testing)
#    include(EnableGtests)
    enable_testing()
#    add_subdirectory(test)
endif (Testing)


