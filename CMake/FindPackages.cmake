# generated by Buildyard, do not edit.

find_package(OpenMP )
if(OpenMP_FOUND)
  set(OpenMP_name OpenMP)
elseif(OPENMP_FOUND)
  set(OpenMP_name OPENMP)
endif()
if(OpenMP_name)
  add_definitions(-DVMMLIB_USE_OPENMP)
  link_directories(${${OpenMP_name}_LIBRARY_DIRS})
  include_directories(${${OpenMP_name}_INCLUDE_DIRS})
endif()
