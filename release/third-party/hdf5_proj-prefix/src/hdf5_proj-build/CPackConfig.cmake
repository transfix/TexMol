# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


SET(CPACK_BINARY_BUNDLE "")
SET(CPACK_BINARY_CYGWIN "")
SET(CPACK_BINARY_DEB "OFF")
SET(CPACK_BINARY_DRAGNDROP "")
SET(CPACK_BINARY_NSIS "OFF")
SET(CPACK_BINARY_OSXX11 "")
SET(CPACK_BINARY_PACKAGEMAKER "")
SET(CPACK_BINARY_RPM "OFF")
SET(CPACK_BINARY_STGZ "ON")
SET(CPACK_BINARY_TBZ2 "OFF")
SET(CPACK_BINARY_TGZ "ON")
SET(CPACK_BINARY_TZ "ON")
SET(CPACK_BINARY_ZIP "")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_COMPONENTS_ALL "configinstall;cppheaders;cpplibraries;hdfdocuments;headers;libraries")
SET(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE "ON")
SET(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
SET(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "help@hdfgroup.org")
SET(CPACK_DEBIAN_PACKAGE_SECTION "Libraries")
SET(CPACK_GENERATOR "STGZ;TGZ;TZ")
SET(CPACK_INSTALL_CMAKE_PROJECTS "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build;HDF5;ALL;/")
SET(CPACK_INSTALL_PREFIX "/workspace/cvcsoft/TexMol/release")
SET(CPACK_MODULE_PATH "/workspace/cvcsoft/TexMol/third-party/hdf5/config/cmake")
SET(CPACK_NSIS_DISPLAY_NAME "HDF5")
SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
SET(CPACK_NSIS_PACKAGE_NAME "HDF5")
SET(CPACK_OUTPUT_CONFIG_FILE "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/CPackConfig.cmake")
SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/RELEASE.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HDF5 built using CMake")
SET(CPACK_PACKAGE_FILE_NAME "HDF5-1.8.10-Linux")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "HDF5")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "HDF5-1.8.10-STATIC")
SET(CPACK_PACKAGE_NAME "HDF5")
SET(CPACK_PACKAGE_RELOCATABLE "true")
SET(CPACK_PACKAGE_VENDOR "HDF_Group")
SET(CPACK_PACKAGE_VERSION "1.8.10")
SET(CPACK_PACKAGE_VERSION_MAJOR "1.8")
SET(CPACK_PACKAGE_VERSION_MINOR "10")
SET(CPACK_PACKAGE_VERSION_PATCH "")
SET(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
SET(CPACK_RESOURCE_FILE_LICENSE "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/COPYING")
SET(CPACK_RESOURCE_FILE_README "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/RELEASE.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "/opt/apps/ossw/applications/cmake/cmake-2.8.9/sl6/share/cmake-2.8/Templates/CPack.GenericWelcome.txt")
SET(CPACK_RPM_COMPONENT_INSTALL "ON")
SET(CPACK_RPM_PACKAGE_DESCRIPTION "The HDF5 technology suite includes:

    * A versatile data model that can represent very complex data objects and a wide variety of metadata.

    * A completely portable file format with no limit on the number or size of data objects in the collection.

    * A software library that runs on a range of computational platforms, from laptops to massively parallel systems, and implements a high-level API with C, C++, Fortran 90, and Java interfaces.

    * A rich set of integrated performance features that allow for access time and storage space optimizations.

    * Tools and applications for managing, manipulating, viewing, and analyzing the data in the collection.

The HDF5 data model, file format, API, library, and tools are open and distributed without charge.
")
SET(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
SET(CPACK_RPM_PACKAGE_LICENSE "BSD-style")
SET(CPACK_RPM_PACKAGE_RELOCATABLE "ON")
SET(CPACK_RPM_PACKAGE_SUMMARY "HDF5 is a unique technology suite that makes possible the management of extremely large and complex data collections.")
SET(CPACK_RPM_PACKAGE_URL "http://www.hdfgroup.org")
SET(CPACK_SET_DESTDIR "OFF")
SET(CPACK_SOURCE_CYGWIN "")
SET(CPACK_SOURCE_GENERATOR "TGZ;TBZ2;TZ")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/CPackSourceConfig.cmake")
SET(CPACK_SOURCE_TBZ2 "ON")
SET(CPACK_SOURCE_TGZ "ON")
SET(CPACK_SOURCE_TZ "ON")
SET(CPACK_SOURCE_ZIP "OFF")
SET(CPACK_SYSTEM_NAME "Linux")
SET(CPACK_TOPLEVEL_TAG "Linux")

# Configuration for component "hdfapplications"

SET(CPACK_COMPONENTS_ALL configinstall cppheaders cpplibraries hdfdocuments headers libraries)
SET(CPACK_COMPONENT_HDFAPPLICATIONS_DISPLAY_NAME "HDF5 Applications")
SET(CPACK_COMPONENT_HDFAPPLICATIONS_GROUP Applications)
SET(CPACK_COMPONENT_HDFAPPLICATIONS_DEPENDS libraries)

# Configuration for component "libraries"

SET(CPACK_COMPONENTS_ALL configinstall cppheaders cpplibraries hdfdocuments headers libraries)
SET(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "HDF5 Libraries")
SET(CPACK_COMPONENT_LIBRARIES_GROUP Runtime)

# Configuration for component "headers"

SET(CPACK_COMPONENTS_ALL configinstall cppheaders cpplibraries hdfdocuments headers libraries)
SET(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "HDF5 Headers")
SET(CPACK_COMPONENT_HEADERS_GROUP Development)
SET(CPACK_COMPONENT_HEADERS_DEPENDS libraries)

# Configuration for component "hdfdocuments"

SET(CPACK_COMPONENTS_ALL configinstall cppheaders cpplibraries hdfdocuments headers libraries)
SET(CPACK_COMPONENT_HDFDOCUMENTS_DISPLAY_NAME "HDF5 Documents")
SET(CPACK_COMPONENT_HDFDOCUMENTS_GROUP Documents)

# Configuration for component "configinstall"

SET(CPACK_COMPONENTS_ALL configinstall cppheaders cpplibraries hdfdocuments headers libraries)
SET(CPACK_COMPONENT_CONFIGINSTALL_DISPLAY_NAME "HDF5 CMake files")
SET(CPACK_COMPONENT_CONFIGINSTALL_GROUP Development)
SET(CPACK_COMPONENT_CONFIGINSTALL_DEPENDS libraries)

# Configuration for component "cpplibraries"

SET(CPACK_COMPONENTS_ALL configinstall cppheaders cpplibraries hdfdocuments headers libraries)
SET(CPACK_COMPONENT_CPPLIBRARIES_DISPLAY_NAME "HDF5 C++ Libraries")
SET(CPACK_COMPONENT_CPPLIBRARIES_GROUP Runtime)
SET(CPACK_COMPONENT_CPPLIBRARIES_DEPENDS libraries)

# Configuration for component "cppheaders"

SET(CPACK_COMPONENTS_ALL configinstall cppheaders cpplibraries hdfdocuments headers libraries)
SET(CPACK_COMPONENT_CPPHEADERS_DISPLAY_NAME "HDF5 C++ Headers")
SET(CPACK_COMPONENT_CPPHEADERS_GROUP Development)
SET(CPACK_COMPONENT_CPPHEADERS_DEPENDS cpplibraries)
