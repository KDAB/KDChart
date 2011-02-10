# - Find KDChart
# This module finds if KDChart is installed.
#
# KDChart_FOUND		- Set to TRUE if KDChart was found.
# KDChart_LIBRARIES	- Path to KDChart libraries.
# KDChart_INCLUDE_DIR	- Path to the KDChart include directory.
#
# This file was generated automatically.
# Please edit generate-configure.sh rather than this file.

include(FindPackageHandleStandardArgs)

find_library(KDChart_LIBRARIES 
	NAMES KDChart kdchart 
	PATH_SUFFIXES bin)
find_path(KDChart_INCLUDE_DIR 
	NAMES KDChart KDChartGlobal)

mark_as_advanced(KDChart_LIBRARIES KDChart_INCLUDE_DIR)

find_package_handle_standard_args(KDChart DEFAULT_MSG KDChart_LIBRARIES KDChart_INCLUDE_DIR)
