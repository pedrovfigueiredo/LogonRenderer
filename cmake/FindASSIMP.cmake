# - Try to find Assimp
# Once done, this will define
#
# ASSIMP_FOUND - system has Assimp
# ASSIMP_INCLUDE_DIR - the Assimp include directories
# ASSIMP_LIBRARIES - link these to use Assimp

FIND_PATH( ASSIMP_INCLUDE_DIR assimp/mesh.h
/usr/include
/usr/local/include
/opt/local/include
"${LogonRenderer_SOURCE_DIR}/contrib"
)

# DOESNT SUPPPORT X32 SYSTEMS, ONLY X64

FIND_LIBRARY( ASSIMP_LIBRARY assimp libassimp libassimp.dylib libassimp.4 libassimp.4.dylib libassimp.4.0.1 libassimp.4.0.1.dylib
/usr/lib64
/usr/local/lib
/opt/local/lib
"${LogonRenderer_SOURCE_DIR}/lib"
)

IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
SET( ASSIMP_FOUND TRUE )
SET( ASSIMP_LIBRARIES ${ASSIMP_LIBRARY} )
ENDIF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
IF(ASSIMP_FOUND)
IF(NOT ASSIMP_FIND_QUIETLY)
MESSAGE(STATUS "Found ASSIMP: ${ASSIMP_LIBRARY}")
ENDIF(NOT ASSIMP_FIND_QUIETLY)
ELSE(ASSIMP_FOUND)
IF(ASSIMP_FIND_REQUIRED)
MESSAGE(FATAL_ERROR "Could not find libASSIMP")
ENDIF(ASSIMP_FIND_REQUIRED)
ENDIF(ASSIMP_FOUND)
