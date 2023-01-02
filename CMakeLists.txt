# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

cmake_minimum_required(VERSION 3.1...3.23)

project( main )


# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()

# include for local directory
#add_subdirectory(modules)
#add_subdirectory(algorithms)
#list(APPEND EXTRA_LIBS convexHull)
#list(APPEND EXTRA_LIBS edgeChange)
#list(APPEND EXTRA_LIBS edgePointPair)
#list(APPEND EXTRA_LIBS genericUtil)
#list(APPEND EXTRA_LIBS geoUtil)
#list(APPEND EXTRA_LIBS incremental)
#list(APPEND EXTRA_LIBS localSearch)
#list(APPEND EXTRA_LIBS simulatedAnnealing)
#list(APPEND EXTRA_LIBS timeManager)

# include for local package


# Creating entries for target: main
# ############################

add_executable( main  main.cpp outputInfo.cpp runCases.cpp edgeChange.cpp convexHull.cpp edgePointPair.cpp genericUtil.cpp geoUtil.cpp incremental.cpp localSearch.cpp simulatedAnnealing.cpp timeManager.cpp mainUtil.cpp)

add_to_cached_list( CGAL_EXECUTABLE_TARGETS main )

# Link the executable to CGAL and third-party libraries
target_link_libraries(main PRIVATE CGAL::CGAL ${EXTRA_LIBS})

