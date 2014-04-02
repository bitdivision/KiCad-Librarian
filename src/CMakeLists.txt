# Build file for CMake, see http://www.cmake.org/
#
# When compiling with MingW, set "-Wl,--subsystem,windows" in the
# CMAKE_EXE_LINKER_FLAGS field (do not add the quotes).

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(kicadlibrarian)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(LIBRARIAN_USE_CURL "TRUE" CACHE BOOL "Whether to include the remote repository functions, which require CURL")

FIND_PACKAGE(wxWidgets COMPONENTS core base adv QUIET)
IF(NOT wxWidgets_FOUND)
    FIND_PACKAGE(wxWidgets COMPONENTS mono REQUIRED)
ENDIF(NOT wxWidgets_FOUND)
INCLUDE("${wxWidgets_USE_FILE}")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/" "${PROJECT_SOURCE_DIR}/../cmake/")
FIND_PACKAGE(libhpdf)
IF(NOT LIBHPDF_FOUND)
    FIND_PACKAGE(libhpdfs REQUIRED)
ENDIF(NOT LIBHPDF_FOUND)

FIND_PACKAGE(ZLIB REQUIRED)

IF(WIN32)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF(WIN32)
INCLUDE_DIRECTORIES("${LIBHPDF_INCLUDE_DIRS}" "${ZLIB_INCLUDE_DIRS}")

IF(LIBRARIAN_USE_CURL)
    FIND_PACKAGE(CURL REQUIRED)
    INCLUDE_DIRECTORIES("${CURL_INCLUDE_DIRS}")
ELSE(LIBRARIAN_USE_CURL)
    ADD_DEFINITIONS(-DNO_CURL)
ENDIF(LIBRARIAN_USE_CURL)

if("${CMAKE_BUILD_TYPE}" MATCHES "Release")
    SET(CPACK_STRIP_FILES TRUE)
endif("${CMAKE_BUILD_TYPE}" MATCHES "Release")


SET(LIBMAN_SRCS librarymanager.cpp          librarymanager.h
                libmngr_dlgnewfootprint.cpp libmngr_dlgnewfootprint.h
                libmngr_dlgnewsymbol.cpp    libmngr_dlgnewsymbol.h
                libmngr_dlgoptions.cpp      libmngr_dlgoptions.h
                libmngr_dlgreport.cpp       libmngr_dlgreport.h
                libmngr_dlgtemplate.cpp     libmngr_dlgtemplate.h
                libmngr_frame.cpp           libmngr_frame.h
                libmngr_paths.cpp           libmngr_paths.h
                libmngr_gui_base.cpp        libmngr_gui_base.h
                cxffont.cpp                 cxffont.h
                libraryfunctions.cpp        libraryfunctions.h
                pdfreport.cpp               pdfreport.h
                rpn.cpp                     rpn.h
                )
IF(LIBRARIAN_USE_CURL)
    SET(LIBMAN_SRCS ${LIBMAN_SRCS}
                libmngr_dlgremotelink.cpp   libmngr_dlgremotelink.h
                remotelink.cpp              remotelink.h
                )
ENDIF(LIBRARIAN_USE_CURL)
IF(WIN32)
    SET(LIBMAN_SRCS ${LIBMAN_SRCS} librarymanager.rc)
ENDIF(WIN32)

ADD_EXECUTABLE(kicadlibrarian ${LIBMAN_SRCS})
IF(LIBRARIAN_USE_CURL)
    TARGET_LINK_LIBRARIES(kicadlibrarian ${wxWidgets_LIBRARIES}
                                     ${LIBHPDF_LIBRARIES}
                                     ${ZLIB_LIBRARIES}
				     ${CURL_LIBRARIES})
ELSE(LIBRARIAN_USE_CURL)
    TARGET_LINK_LIBRARIES(kicadlibrarian ${wxWidgets_LIBRARIES}
                                     ${LIBHPDF_LIBRARIES}
                                     ${ZLIB_LIBRARIES})
ENDIF(LIBRARIAN_USE_CURL)

##### Installation instructions

INSTALL(TARGETS kicadlibrarian
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    COMPONENT application)
INSTALL(FILES ${CMAKE_BINARY_DIR}/../doc/kicadlibrarian.pdf
              ${CMAKE_BINARY_DIR}/../doc/LibraryFileFormats.pdf
              ${CMAKE_BINARY_DIR}/../doc/LICENSE
              ${CMAKE_BINARY_DIR}/../doc/NOTICE
        DESTINATION ${CMAKE_INSTALL_PREFIX}/doc
        COMPONENT documentation)
INSTALL(FILES ${CMAKE_BINARY_DIR}/../font/newstroke.cxf
        DESTINATION ${CMAKE_INSTALL_PREFIX}/font
        COMPONENT font)
FILE(GLOB TPL_FILES "${CMAKE_BINARY_DIR}/../template/*")
INSTALL(FILES ${TPL_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/template
        COMPONENT templates)

SET(DEB_DESKTOP_DIR     "/usr/share/applications")
SET(DEB_MIME_DIR        "/usr/share/mime/packages")
SET(DEB_PIXMAPS_DIR     "/usr/share/pixmaps")

INSTALL(FILES ${CMAKE_BINARY_DIR}/kicadlibrarian.desktop DESTINATION ${DEB_DESKTOP_DIR})
INSTALL(FILES ${CMAKE_BINARY_DIR}/kicadlibrarian.xml DESTINATION ${DEB_MIME_DIR})
INSTALL(FILES ${CMAKE_BINARY_DIR}/kicadlibrarian32.png DESTINATION ${DEB_PIXMAPS_DIR})

##### Packaging instructions

SET(CPACK_GENERATOR DEB)
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
IF(LIBRARIAN_USE_CURL)
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxgtk2.8-0 (>= 2.8.10-1), libhpdf-2.2.1 (>= 2.2.1-1), libcurl3 (>= 7.29.0-1), libc6 (>= 2.11)")
ELSE(LIBRARIAN_USE_CURL)
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxgtk2.8-0 (>= 2.8.10-1), libhpdf-2.2.1 (>= 2.2.1-1), libc6 (>= 2.11)")
ENDIF(LIBRARIAN_USE_CURL)
SET(CPACK_PACKAGE_NAME "KiCadLibrarian")
SET(CPACK_PACKAGE_VENDOR "CompuPhase")
SET(CPACK_DEBIAN_PACKAGE_LICENSE "Apache v2")
SET(CPACK_PACKAGE_CONTACT "Thiadmer Riemersma <thiadmer@compuphase.com>")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A utility to maintain footprint and symbol libraries, for the KiCad EDA suite.")
SET(CPACK_PACKAGE_DESCRIPTION "With this utility, you can copy schematic symbols and footprints from one library to another, make adjustements to the symbols or footprints, and create reports of the libraries. The Librarian includes a template-based wizard to quickly create new footprints from scratch, but it can also modify existing footprints.")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "6")
SET(CPACK_PACKAGE_VERSION_PATCH "5030")
SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "kicadlibrarian")
SET(CPACK_COMPONENTS_ALL application documentation templates)
SET(CPACK_PACKAGE_EXECUTABLES "kicadlibrarian")

SET(CPACK_INSTALL_PREFIX "/opt/kicadlibrarian")
SET(CPACK_SET_DESTDIR TRUE)

INCLUDE(CPack)
