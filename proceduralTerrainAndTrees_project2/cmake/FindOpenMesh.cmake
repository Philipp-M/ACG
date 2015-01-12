# - Try to locate OpenMesh
# This module defines:
#
#  OpenMesh_INCLUDE_DIR
#  OpenMesh_LIBRARY
#  OpenMesh_FOUND
#

FIND_PATH(OpenMesh_INCLUDE_DIR NAMES OpenMesh/Core/Mesh/BaseMesh.hh OpenMesh/Tools/Utils/MeshCheckerT.hh)

FIND_LIBRARY(OpenMesh_LIBRARY NAMES OpenMeshCore OpenMeshTools PATH_SUFFIXES OpenMesh/)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenMesh REQUIRED_VARS OpenMesh_INCLUDE_DIR OpenMesh_LIBRARY)

MARK_AS_ADVANCED(OpenMesh_INCLUDE_DIR OpenMesh_LIBRARY)
