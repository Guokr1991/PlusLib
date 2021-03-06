PROJECT(PlusLib)

CMAKE_MINIMUM_REQUIRED(VERSION 3.3.0)

SET(CMAKE_CXX_STANDARD 11)

# ------------------------------------------------------------------------
# Set project version number
SET(PLUSLIB_VERSION_MAJOR "2")
SET(PLUSLIB_VERSION_MINOR "5")
SET(PLUSLIB_VERSION_PATCH "0")
SET(PLUSLIB_VERSION ${PLUSLIB_VERSION_MAJOR}.${PLUSLIB_VERSION_MINOR}.${PLUSLIB_VERSION_PATCH})

# Use solution folders.
IF(MSVC OR ${CMAKE_GENERATOR} MATCHES "Xcode")
  SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
  SET_PROPERTY(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake Targets")
  SET_PROPERTY(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "AutoGen (Qt) Targets")
ENDIF()

MESSAGE(STATUS "Configuring PLUS toolkit version ${PLUSLIB_VERSION}.")

IF(MSVC OR ${CMAKE_GENERATOR} MATCHES "Xcode")
  IF( ${CMAKE_GENERATOR} MATCHES "Win64" )
    SET(PLUSLIB_PLATFORM "Win64" CACHE STRING "Name of the platform this instance of PlusLib is built on")
  ELSE()
    SET(PLUSLIB_PLATFORM "Win32" CACHE STRING "Name of the platform this instance of PlusLib is built on")
  ENDIF()
ELSE()
  IF(APPLE)
    SET (PLUSLIB_PLATFORM "OSX" CACHE STRING "Name of the platform this instance of PlusLib is built on")
  ELSE()
    SET (PLUSLIB_PLATFORM "Linux" CACHE STRING "Name of the platform this instance of PlusLib is built on")
  ENDIF()
ENDIF()

MESSAGE(STATUS "Detected platform: ${PLUSLIB_PLATFORM}.")

# --------------------------------------------------------------------------
# System Settings
IF(WIN32)
  # This method adds the necessary compiler flag
  SET (RUNTIME_MINIMUM_WINDOWS_VERSION "0x0602")
  ADD_DEFINITIONS(-D_MSC_PLATFORM_TOOLSET_$(PlatformToolset))
  
  # Correct the incoming windows sdk variables, as they have been manipulated to be passed to this project
  # as # separated strings
  STRING(REPLACE "#" ";" WINDOWS_SDK_INCLUDE_DIRS "${WINDOWS_SDK_INCLUDE_DIRS}")
  SET(WINDOWS_SDK_INCLUDE_DIRS ${WINDOWS_SDK_INCLUDE_DIRS} CACHE PATH "Path to the Windows SDK include dirs" FORCE)
  STRING(REPLACE "#" ";" WINDOWS_SDK_LIBRARY_DIRS "${WINDOWS_SDK_LIBRARY_DIRS}")
  SET(WINDOWS_SDK_LIBRARY_DIRS ${WINDOWS_SDK_LIBRARY_DIRS} CACHE PATH "Path to the Windows SDK library dirs" FORCE)
ENDIF()

# --------------------------------------------------------------------------
# User options
#

# Standard CMake option for testing.
OPTION(BUILD_TESTING "Build test programs" ON)

# --------------------------------------------------------------------------
# Testing
#

IF (PLUS_OFFLINE_BUILD)
  SET(PLUSLIB_REVISION "NA")
  SET(PLUSLIB_SHORT_REVISION "NA")
ELSE()
  # Need git for the automatic update of the repository
  FIND_PACKAGE(Git)
  IF(Git_FOUND)
    EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD OUTPUT_VARIABLE PLUSLIB_REVISION WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    STRING(REGEX REPLACE "\n$" "" PLUSLIB_REVISION "${PLUSLIB_REVISION}")
    STRING(REGEX REPLACE "\r$" "" PLUSLIB_REVISION "${PLUSLIB_REVISION}")
    
    MESSAGE(STATUS "Current git hash is ${PLUSLIB_REVISION}")

    EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD OUTPUT_VARIABLE PLUSLIB_SHORT_REVISION WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    STRING(REGEX REPLACE "\n$" "" PLUSLIB_SHORT_REVISION "${PLUSLIB_SHORT_REVISION}")
    STRING(REGEX REPLACE "\r$" "" PLUSLIB_SHORT_REVISION "${PLUSLIB_SHORT_REVISION}")
  ELSE()
    MESSAGE(WARNING "Git tool not found. Cannot retrieve revision information from repository")
    SET(PLUSLIB_REVISION "NA")
    SET(PLUSLIB_SHORT_REVISION "NA")
  ENDIF()
ENDIF()

INCLUDE(CTest)

# Clean up the IDE folder name for CTest dashboard targets
IF(BUILD_TESTING)
  FOREACH(mode Experimental Nightly Continuous NightlyMemoryCheck)
    IF(TARGET ${mode})
      SET_PROPERTY(TARGET ${mode} PROPERTY FOLDER "CTest Targets")
    ENDIF()
  ENDFOREACH()
ENDIF()

# --------------------------------------------------------------------------
# Set up paths for data and SDKs
#
SET(PLUSLIB_TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools)

SET(ATC_TRAKSTAR_BASE_DIR ${PLUSLIB_TOOLS_DIR}/Ascension/trakSTAR_940041_RevE)
SET(ATC_TRAKSTAR_INCLUDE_DIR ${ATC_TRAKSTAR_BASE_DIR}/inc CACHE PATH "Path to the Ascension trakSTAR 3D Guidance include folder (where the ATC3DG.h file is located)" FORCE )
SET(ATC_TRAKSTAR_LIB_DIR ${ATC_TRAKSTAR_BASE_DIR}/lib CACHE PATH "Path to the Ascension trakSTAR 3D Guidance lib folder" FORCE )
SET(ATC_TRAKSTAR_BINARY_DIR ${ATC_TRAKSTAR_BASE_DIR}/bin CACHE PATH "Path to the Ascension trakSTAR 3D Guidance dll folder" FORCE )
SET(ATC_MEDSAFE_BASE_DIR ${PLUSLIB_TOOLS_DIR}/Ascension/medSAFE_940033_Rev_F)
SET(ATC_MEDSAFE_INCLUDE_DIR ${ATC_MEDSAFE_BASE_DIR}/inc CACHE PATH "Path to the Ascension medSAFE 3D Guidance include folder (where the ATC3DGm.h file is located)" FORCE )
SET(ATC_MEDSAFE_LIB_DIR ${ATC_MEDSAFE_BASE_DIR}/lib CACHE PATH "Path to the Ascension medSAFE 3D Guidance lib folder" FORCE )
SET(ATC_MEDSAFE_BINARY_DIR ${ATC_MEDSAFE_BASE_DIR}/bin CACHE PATH "Path to the Ascension medSAFE 3D Guidance dll folder" FORCE )
MARK_AS_ADVANCED(ATC_TRAKSTAR_BASE_DIR
  ATC_TRAKSTAR_INCLUDE_DIR
  ATC_TRAKSTAR_LIB_DIR
  ATC_TRAKSTAR_BINARY_DIR
  ATC_MEDSAFE_BASE_DIR
  ATC_MEDSAFE_INCLUDE_DIR
  ATC_MEDSAFE_LIB_DIR
  ATC_MEDSAFE_BINARY_DIR
  )
IF(PLUS_USE_Ascension3DG)
  ADD_SUBDIRECTORY(${ATC_TRAKSTAR_BASE_DIR})
ENDIF()
IF(PLUS_USE_Ascension3DGm)
 ADD_SUBDIRECTORY(${ATC_MEDSAFE_BASE_DIR})
ENDIF()

SET(PHIDGET_SPATIAL_BASE_DIR ${PLUSLIB_TOOLS_DIR}/Phidget/PhidgetSpatial-2.1.8)
SET(PHIDGET_SPATIAL_INCLUDE_DIR ${PHIDGET_SPATIAL_BASE_DIR} CACHE PATH "Path to the Phidget Spatial include folder" FORCE )
IF(MSVC AND ${CMAKE_GENERATOR} MATCHES "Win64")
  SET(PHIDGET_SPATIAL_LIB_DIR ${PHIDGET_SPATIAL_BASE_DIR}/x64 CACHE PATH "Path to the Phidget Spatial 64-bit lib folder" FORCE )
  SET(PHIDGET_SPATIAL_BINARY_DIR ${PHIDGET_SPATIAL_BASE_DIR}/x64 CACHE PATH "Path to the Phidget Spatial 64-bit dll folder" FORCE )
ELSE()
  SET(PHIDGET_SPATIAL_LIB_DIR ${PHIDGET_SPATIAL_BASE_DIR}/x86 CACHE PATH "Path to the Phidget Spatial 32-bit lib folder" FORCE )
  SET(PHIDGET_SPATIAL_BINARY_DIR ${PHIDGET_SPATIAL_BASE_DIR}/x86 CACHE PATH "Path to the Phidget Spatial 32-bit dll folder" FORCE )
ENDIF()
MARK_AS_ADVANCED(PHIDGET_SPATIAL_BASE_DIR
  PHIDGET_SPATIAL_INCLUDE_DIR
  PHIDGET_SPATIAL_LIB_DIR
  PHIDGET_SPATIAL_BINARY_DIR
  )
IF(PLUS_USE_PHIDGET_SPATIAL_TRACKER)
  ADD_SUBDIRECTORY(tools/Phidget/PhidgetSpatial-2.1.8)
ENDIF()

SET(EPIPHAN_SDK_BASE_DIR ${PLUSLIB_TOOLS_DIR}/Epiphan)
SET(EPIPHAN_INCLUDE_DIR ${EPIPHAN_SDK_BASE_DIR} CACHE PATH "Path to the Epiphan SDK include directory." FORCE )
IF(MSVC AND ${CMAKE_GENERATOR} MATCHES "Win64")
  SET(EPIPHAN_BINARY_DIR ${EPIPHAN_SDK_BASE_DIR}/x64 CACHE PATH "Directory containing Epiphan shared library file." FORCE)
  SET(EPIPHAN_LIB_DIR ${EPIPHAN_SDK_BASE_DIR}/x64 CACHE PATH "Directory containing Epiphan static library file." FORCE)
ELSE()
  SET(EPIPHAN_BINARY_DIR ${EPIPHAN_SDK_BASE_DIR}/Win32 CACHE PATH "Directory containing Epiphan shared library file." FORCE)
  SET(EPIPHAN_LIB_DIR ${EPIPHAN_SDK_BASE_DIR}/Win32 CACHE PATH "Directory containing Epiphan static library file." FORCE)
ENDIF()
MARK_AS_ADVANCED(EPIPHAN_SDK_BASE_DIR
  EPIPHAN_INCLUDE_DIR
  EPIPHAN_BINARY_DIR
  EPIPHAN_LIB_DIR
  )
IF(PLUS_USE_EPIPHAN)
  ADD_SUBDIRECTORY(tools/Epiphan)
ENDIF()

SET(USDIGITAL_SEI_BASE_DIR ${PLUSLIB_TOOLS_DIR}/UsDigital/SEI_5.22)
SET(USDIGITAL_SEI_INCLUDE_DIR ${USDIGITAL_SEI_BASE_DIR}/inc CACHE PATH "Path to the US Digital SEI include directory (needed for the CIVCO stepper)." FORCE )
SET(USDIGITAL_SEI_BINARY_DIR ${USDIGITAL_SEI_BASE_DIR}/bin CACHE PATH "Path to the US Digital SEI binary directory (needed for the CIVCO stepper)." FORCE )
SET(USDIGITAL_SEI_LIB_DIR ${USDIGITAL_SEI_BASE_DIR}/lib CACHE PATH "Path to the US Digital SEI library directory (needed for the CIVCO stepper)." FORCE )
MARK_AS_ADVANCED(USDIGITAL_SEI_BASE_DIR
  USDIGITAL_SEI_INCLUDE_DIR
  USDIGITAL_SEI_BINARY_DIR
  USDIGITAL_SEI_LIB_DIR
  )
IF(PLUS_USE_BRACHY_TRACKER)
  ADD_SUBDIRECTORY(tools/UsDigital/SEI_5.22)
ENDIF()

IF(WIN32)
  SET(NVIDIA_DVP_BASE_DIR ${PLUSLIB_TOOLS_DIR}/NVidia/dvp170)
  SET(NVIDIA_DVP_INCLUDE_DIR ${NVIDIA_DVP_BASE_DIR}/include CACHE PATH "Path to the NVidia DVP include directory." FORCE )
  IF(MSVC AND ${CMAKE_GENERATOR} MATCHES "Win64")
    SET(NVIDIA_DVP_BINARY_DIR ${NVIDIA_DVP_BASE_DIR}/bin/x64 CACHE PATH "Directory containing NVidia DVP shared library file." FORCE)
    SET(NVIDIA_DVP_LIB_DIR ${NVIDIA_DVP_BASE_DIR}/lib/x64 CACHE PATH "Directory containing NVidia DVP static library file." FORCE)
  ELSE()
    SET(NVIDIA_DVP_BINARY_DIR ${NVIDIA_DVP_BASE_DIR}/bin/win32 CACHE PATH "Directory containing NVidia DVP shared library file." FORCE)
    SET(NVIDIA_DVP_LIB_DIR ${NVIDIA_DVP_BASE_DIR}/lib/win32 CACHE PATH "Directory containing NVidia DVP static library file." FORCE)
  ENDIF()
  MARK_AS_ADVANCED(NVIDIA_DVP_BASE_DIR
    NVIDIA_DVP_INCLUDE_DIR
    NVIDIA_DVP_BINARY_DIR
    NVIDIA_DVP_LIB_DIR
    )
  IF(PLUS_USE_NVIDIA_DVP)
    ADD_SUBDIRECTORY(tools/NVidia/dvp170)
  ENDIF()
ENDIF()

# --------------------------------------------------------------------------
# Subdirs
#
ADD_SUBDIRECTORY(src)

#-----------------------------------------------------------------------------
# Generate convenience files for automatic build, test, and packaging
#
IF(WIN32)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/BuildAndTest.bat.in
    ${CMAKE_CURRENT_BINARY_DIR}/BuildAndTest.bat
    )

  # Documentation generator
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/CreateApiReference.bat.in
    ${CMAKE_CURRENT_BINARY_DIR}/CreateApiReference.bat
    )
ELSE()
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/BuildAndTest.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/BuildAndTest.sh
    )
    file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/BuildAndTest.sh
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
      FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ WORLD_READ)
ENDIF()