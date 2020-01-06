# - Find KDChart
# This module finds if KDChart is installed.
#
# KDChart_FOUND		- Set to TRUE if KDChart was found.
# KDChart_LIBRARIES	- Path to KDChart libraries.
# KDChart_INCLUDE_DIR	- Path to the KDChart include directory.
#
# Copyright (C) 2011-2020 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com

# Redistribution and use is allowed according to the terms of the BSD license

include(FindPackageHandleStandardArgs)

find_library(KDChart_LIBRARIES 
	NAMES KDChart kdchart 
	PATH_SUFFIXES bin)
find_path(KDChart_INCLUDE_DIR 
	NAMES KDChart KDChartGlobal)

mark_as_advanced(KDChart_LIBRARIES KDChart_INCLUDE_DIR)

find_package_handle_standard_args(KDChart DEFAULT_MSG KDChart_LIBRARIES KDChart_INCLUDE_DIR)
