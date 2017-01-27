PROJECT(vtkPlusMicronTrackerInterface)

SET (vtkPlusMicronTrackerInterface_INCLUDE_DIRS 
  ${vtkPlusMicronTrackerInterface_SOURCE_DIR} 
  ${vtkPlusMicronTrackerInterface_BINARY_DIR} 
  )

SET(vtkPlusMicronTrackerInterface_SRCS
  MicronTrackerInterface.h
  MicronTrackerInterface.cpp
  MicronTrackerLogger.h
  MicronTrackerLogger.cpp
  Cameras.h
  Cameras.cpp
  MCamera.h
  MCamera.cpp
  Collection.h
  Collection.cpp
  Facet.h
  Facet.cpp
  Marker.h
  Marker.cpp
  Markers.h
  Markers.cpp
  MTXPoint.h
  MTXPoint.cpp
  Persistence.h
  Persistence.cpp
  Vector.h
  Vector.cpp
  Xform3D.h
  Xform3D.cpp
  XPoints.h
  XPoints.cpp
)

SET (vtkPlusMicronTrackerInterface_INCLUDE_DIRS 
  ${vtkPlusMicronTrackerInterface_INCLUDE_DIRS} 
  ${MICRONTRACKER_INCLUDE_DIR}
  )

SET (Utilities_INCLUDE_DIRS ${Utilities_INCLUDE_DIRS} ${vtkPlusMicronTrackerInterface_INCLUDE_DIRS} CACHE INTERNAL "")

# Create a static library (to not have too many DLLs and also because
# the driver library interface havily uses STL containers, which are unsafe
# to use at a DLL interface)
ADD_LIBRARY(vtkPlusMicronTrackerInterface STATIC ${vtkPlusMicronTrackerInterface_SRCS})
target_include_directories(vtkPlusMicronTrackerInterface PUBLIC ${vtkPlusMicronTrackerInterface_INCLUDE_DIRS} )

# --------------------------------------------------------------------------
# Set external MS project 
SET (PLUSLIB_DEPENDENCIES ${PLUSLIB_DEPENDENCIES} vtkPlusMicronTrackerInterface CACHE INTERNAL "" FORCE)
LIST(REMOVE_DUPLICATES PLUSLIB_DEPENDENCIES)
# Add this variable to UsePlusLib.cmake.in INCLUDE_PLUSLIB_MS_PROJECTS macro
SET(vcProj_vtkPlusMicronTrackerInterface vtkPlusMicronTrackerInterface;"${PlusLib_BINARY_DIR}/src/Utilities/MicronTrackerInterface/vtkPlusMicronTrackerInterface.vcproj" CACHE INTERNAL "" FORCE) 