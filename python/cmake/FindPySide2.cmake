#  SHIBOKEN_INCLUDE_DIR        - Directories to include to use SHIBOKEN
#  SHIBOKEN_LIBRARY            - Files to link against to use SHIBOKEN
#  SHIBOKEN_BINARY             - Executable name
#  SHIBOKEN_BUILD_TYPE         - Tells if Shiboken was compiled in Release or Debug mode.

#  PYSIDE_BASEDIR              - Top of the PySide2 installation
#  PYSIDE_INCLUDE_DIR          - Directories to include to use PySide2
#  PYSIDE_LIBRARY              - Files to link against to use PySide2
#  PYSIDE_TYPESYSTEMS          - Type system files that should be used by other bindings extending PySide2
#  PYSIDE_VERSION              - PySide2 Version

#  PYTHON_SITE_PACKAGES        - Path where the Python modules are installed

set(PySide2_FOUND FALSE)
set(Shiboken2_FOUND FALSE)
set(PySide2Python3_FOUND FALSE)
set(PYSIDE2_CUSTOM_PREFIX "/usr/" CACHE PATH "Extra path to look for PySide components.")

execute_process(
  COMMAND ${Python3_EXECUTABLE} -c "if True:
      from distutils.sysconfig import get_python_lib
      print(get_python_lib())
      "
  OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Python site-packages:       ${PYTHON_SITE_PACKAGES}")

# extract PySide2 version
execute_process(
  COMMAND ${Python3_EXECUTABLE} -c "if True:
    import os, sys
    try:
        import PySide2
        print(PySide2.__version__)
    except Exception as error:
        print(error, file=sys.stderr)
        exit()
        "
  OUTPUT_VARIABLE PYSIDE2_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${Python3_EXECUTABLE} -c "if True:
    import os, sys
    try:
        import PySide2.QtCore as QtCore
        print(os.path.dirname(QtCore.__file__))
    except Exception as error:
        print(error, file=sys.stderr)
        exit()
        "
  OUTPUT_VARIABLE PYSIDE2_BASEDIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(PYSIDE2_BASEDIR)
  set(PYSIDE_BASEDIR ${PYSIDE2_BASEDIR} CACHE PATH "Top level install of PySide2" FORCE)
  execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "if True:
      import os
      import PySide2.QtCore as QtCore
      print(os.path.basename(QtCore.__file__).split('.', 1)[1])
      "
    OUTPUT_VARIABLE PYSIDE2_SUFFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "if True:
      import os
      import PySide2.QtCore as QtCore
      print(\";\".join(map(str, QtCore.__version_info__)))
      "
    OUTPUT_VARIABLE PYSIDE2_SO_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  list(GET PYSIDE2_SO_VERSION 0 PYSIDE2_SO_MACRO_VERSION)
  list(GET PYSIDE2_SO_VERSION 1 PYSIDE2_SO_MICRO_VERSION)
  list(GET PYSIDE2_SO_VERSION 2 PYSIDE2_SO_MINOR_VERSION)
  string(REPLACE ";" "." PYSIDE2_SO_VERSION "${PYSIDE2_SO_VERSION}")

  if(NOT APPLE)
    set(PYSIDE2_SUFFIX "${PYSIDE2_SUFFIX}.${PYSIDE2_SO_MACRO_VERSION}.${PYSIDE2_SO_MICRO_VERSION}")
  else()
    string(REPLACE ".so" "" PYSIDE2_SUFFIX ${PYSIDE2_SUFFIX})
    set(PYSIDE2_SUFFIX "${PYSIDE2_SUFFIX}.${PYSIDE2_SO_MACRO_VERSION}.${PYSIDE2_SO_MICRO_VERSION}.dylib")
  endif()

  set(PySide2_FOUND TRUE)
  message(STATUS "PySide2 base dir:       ${PYSIDE2_BASEDIR}")
  message(STATUS "PySide2 version:        ${PYSIDE2_SO_VERSION}")
  message(STATUS "PySide2 suffix:         ${PYSIDE2_SUFFIX}")

else()
  message(STATUS "PySide not found. Consider running pip3 install --index-url=https://download.qt.io/snapshots/ci/pyside/5.12/latest/ pyside2 --trusted-host download.qt.io")
endif()

#SHIBOKEN
#===============================================================================
execute_process(
  COMMAND ${Python3_EXECUTABLE} -c "if True:
    import os
    try:
        import shiboken2_generator
        print(shiboken2_generator.__path__[0])
    except:
        exit()
        "
  OUTPUT_VARIABLE SHIBOKEN_GENERATOR_BASEDIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${Python3_EXECUTABLE} -c "if True:
    import os
    try:
        import shiboken2
        print(shiboken2.__path__[0])
    except:
        exit()
        "
  OUTPUT_VARIABLE SHIBOKEN_BASEDIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "ShibokenGenerator base dir:       ${SHIBOKEN_GENERATOR_BASEDIR}")
message(STATUS "Shiboken base dir:                ${SHIBOKEN_BASEDIR}")

if(SHIBOKEN_BASEDIR)
  find_path(SHIBOKEN_INCLUDE_DIR
    shiboken.h
    PATHS ${SHIBOKEN_GENERATOR_BASEDIR}/include ${PYSIDE2_CUSTOM_PREFIX}/include/shiboken2
    NO_DEFAULT_PATH
  )
  if(MSVC)
    set(SHIBOKEN_LIBRARY_BASENAME "shiboken2.abi3.lib")
  elseif(CYGWIN)
    set(SHIBOKEN_LIBRARY_BASENAME "")
  elseif(WIN32)
    set(SHIBOKEN_LIBRARY_BASENAME "libshiboken2.${PYSIDE2_SUFFIX}")
  else()
    set(SHIBOKEN_LIBRARY_BASENAME "libshiboken2.${PYSIDE2_SUFFIX}")
  endif()

  if(NOT SHIBOKEN_INCLUDE_DIR)
    message(STATUS "shiboken_generator not found. Consider running pip3 install --index-url=https://download.qt.io/snapshots/ci/pyside/${Qt5Core_VERSION}/latest/ shiboken2-generator --trusted-host download.qt.io")
    return()
  endif()

  message("Look for ${SHIBOKEN_LIBRARY_BASENAME} at ${SHIBOKEN_BASEDIR}")
  find_file(SHIBOKEN_LIBRARY
    ${SHIBOKEN_LIBRARY_BASENAME}
    PATHS ${SHIBOKEN_BASEDIR} ${PYSIDE2_CUSTOM_PREFIX}/lib
    NO_DEFAULT_PATH
  )

  find_program(SHIBOKEN_BINARY
    shiboken2
    PATHS ${SHIBOKEN_GENERATOR_BASEDIR} ${PYSIDE2_CUSTOM_PREFIX}/bin
    NO_DEFAULT_PATH
  )

  set(SHIBOKEN_GENERATOR_VERSION "<none>")
  if(SHIBOKEN_BINARY)
    execute_process(
      COMMAND ${SHIBOKEN_BINARY} --version
      OUTPUT_VARIABLE SHIBOKEN_GENERATOR_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" SHIBOKEN_GENERATOR_VERSION "${SHIBOKEN_GENERATOR_VERSION}")
  endif()

  message(STATUS "Shiboken include dir:       ${SHIBOKEN_INCLUDE_DIR}")
  message(STATUS "Shiboken library:           ${SHIBOKEN_LIBRARY}")
  message(STATUS "Shiboken binary:            ${SHIBOKEN_BINARY}  Vr:${SHIBOKEN_GENERATOR_VERSION}")

  if(SHIBOKEN_INCLUDE_DIR AND SHIBOKEN_LIBRARY AND SHIBOKEN_BINARY)
    set(Shiboken2_FOUND TRUE)
  endif()

else()
  message(STATUS "shiboken_generator not found. Consider running pip3 install --index-url=https://download.qt.io/snapshots/ci/pyside/${Qt5Core_VERSION}/latest/ shiboken2-generator --trusted-host download.qt.io")
  set(Shiboken2_FOUND FALSE)
endif()

if(MSVC)
  # On Windows we must link to python3.dll that is a small library that links against python3x.dll
  # that allow us to choose any python3x.dll at runtime
  execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "if True:
      for lib in '${Python3_LIBRARIES}'.split(';'):
          if '/' in lib:
              prefix, py = lib.rsplit('/', 1)
              if py.startswith('python3'):
                  print(prefix + '/python3.lib')
                  break
        "
    OUTPUT_VARIABLE PYTHON_LIMITED_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
else()
  # On Linux and MacOs our modules should not link with any python library
  # that must be handled by the main process
  set(PYTHON_LIMITED_LIBRARIES "")
endif()

if(PySide2_FOUND)
  #PySide
  #===============================================================================
  find_path(PYSIDE_INCLUDE_DIR
    pyside.h
    PATHS ${PYSIDE2_BASEDIR}/include ${PYSIDE2_CUSTOM_PREFIX}/include/PySide2
    NO_DEFAULT_PATH
  )

  # Platform specific library names
  if(MSVC)
    set(PYSIDE_LIBRARY_BASENAME "pyside2.abi3.lib")
  elseif(CYGWIN)
    set(PYSIDE_LIBRARY_BASENAME "")
  elseif(WIN32)
    set(PYSIDE_LIBRARY_BASENAME "libpyside2.${PYSIDE2_SUFFIX}")
  else()
    set(PYSIDE_LIBRARY_BASENAME "libpyside2.${PYSIDE2_SUFFIX}")
  endif()

  find_file(PYSIDE_LIBRARY
    ${PYSIDE_LIBRARY_BASENAME}
    PATHS ${PYSIDE2_BASEDIR} ${PYSIDE2_CUSTOM_PREFIX}/lib
    NO_DEFAULT_PATH
  )

  find_path(PYSIDE_TYPESYSTEMS
    typesystem_core.xml
    PATHS ${PYSIDE2_BASEDIR}/typesystems ${PYSIDE2_CUSTOM_PREFIX}/share/PySide2/typesystems
    NO_DEFAULT_PATH
  )

  message(STATUS "PySide include dir:         ${PYSIDE_INCLUDE_DIR}")
  message(STATUS "PySide library:             ${PYSIDE_LIBRARY}")
  message(STATUS "PySide typesystems:         ${PYSIDE_TYPESYSTEMS}")

  if(PYSIDE_INCLUDE_DIR AND PYSIDE_LIBRARY AND PYSIDE_TYPESYSTEMS)
    set(PySide2Python3_FOUND TRUE)
  endif()

  # Create PySide2 target
  add_library(PySide2::libpyside SHARED IMPORTED GLOBAL)
  if(MSVC)
    set_property(TARGET PySide2::libpyside PROPERTY IMPORTED_IMPLIB Python3::Python)
  endif()
  set_property(TARGET PySide2::libpyside PROPERTY IMPORTED_LOCATION Python3::Python)
  set_property(TARGET PySide2::libpyside APPEND PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES
    ${PYSIDE_INCLUDE_DIR}
    ${PYSIDE_INCLUDE_DIR}/QtCore/
    ${PYSIDE_INCLUDE_DIR}/QtGui/
    ${PYSIDE_INCLUDE_DIR}/QtWidgets/
    ${Python3_INCLUDE_DIRS}
  )
endif()

if(Shiboken2_FOUND)
  # Create shiboke2 target
  add_library(Shiboken2::libshiboken SHARED IMPORTED GLOBAL)
  if(MSVC)
    set_property(TARGET Shiboken2::libshiboken PROPERTY IMPORTED_IMPLIB ${SHIBOKEN_LIBRARY})
  endif()
  set_property(TARGET Shiboken2::libshiboken PROPERTY IMPORTED_LOCATION ${SHIBOKEN_LIBRARY})
  set_property(TARGET Shiboken2::libshiboken APPEND PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${SHIBOKEN_INCLUDE_DIR} ${Python3_INCLUDE_DIRS}
  )
  set_property(TARGET Shiboken2::libshiboken APPEND PROPERTY
    INTERFACE_LINK_LIBRARIES ${PYTHON_LIMITED_LIBRARIES}
  )
endif()

if(PySide2_FOUND AND Shiboken2_FOUND)
  set(PySide2_FOUND TRUE)
else()
  set(PySide2_FOUND FALSE)
endif()

mark_as_advanced(
  SHIBOKEN_INCLUDE_DIR
  SHIBOKEN_LIBRARY
  SHIBOKEN_BINARY
  SHIBOKEN_BINARY
  PYSIDE_BASEDIR
  PYSIDE_INCLUDE_DIR
  PYSIDE_LIBRARY
  PYSIDE_TYPESYSTEMS
  PYTHON_SITE_PACKAGES
)

find_package_handle_standard_args(PySide2
  REQUIRED_VARS
  SHIBOKEN_INCLUDE_DIR
  SHIBOKEN_LIBRARY
  SHIBOKEN_BINARY
  SHIBOKEN_BINARY
  PYSIDE_BASEDIR
  PYSIDE_INCLUDE_DIR
  PYSIDE_LIBRARY
  PYSIDE_TYPESYSTEMS
  PYTHON_SITE_PACKAGES
  PYSIDE2_VERSION
)
