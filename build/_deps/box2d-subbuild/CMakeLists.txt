# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.22.1)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(box2d-populate NONE)



include(ExternalProject)
ExternalProject_Add(box2d-populate
                     "UPDATE_DISCONNECTED" "False" "URL" "https://github.com/erincatto/box2d/archive/refs/tags/v2.4.1.tar.gz"
                    SOURCE_DIR          "/home/dave/Documents/c++/Kookies/build/_deps/box2d-src"
                    BINARY_DIR          "/home/dave/Documents/c++/Kookies/build/_deps/box2d-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


