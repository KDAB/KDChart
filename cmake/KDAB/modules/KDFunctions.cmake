#
# SPDX-FileCopyrightText: 2021-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: BSD-3-Clause
#
if(POLICY CMP0057)
    cmake_policy(SET CMP0057 NEW) # if() supports IN_LIST
endif()

get_property(_supported_languages GLOBAL PROPERTY ENABLED_LANGUAGES)
if("C" IN_LIST _supported_languages)
    include(CheckCCompilerFlag)
    set(KD_C_PROJECT True SCOPE GLOBAL)
endif()
if("CXX" IN_LIST _supported_languages)
    include(CheckCXXCompilerFlag)
    set(KD_CXX_PROJECT True SCOPE GLOBAL)
endif()

# If the condition is true, add the specified value to the arguments at the parent scope
function(kd_append_if condition value)
    if(${condition})
        foreach(variable ${ARGN})
            set(${variable} "${${variable}} ${value}" PARENT_SCOPE)
        endforeach()
    endif()
endfunction()

# Add C and C++ compiler command line option
macro(kd_add_flag_if_supported flag name)
    if(KD_C_PROJECT)
        check_c_compiler_flag("-Werror ${flag}" "C_SUPPORTS_${name}")
        kd_append_if("C_SUPPORTS_${name}" "${flag}" CMAKE_C_FLAGS)
    endif()
    if(KD_CXX_PROJECT)
        check_cxx_compiler_flag("-Werror ${flag}" "CXX_SUPPORTS_${name}")
        kd_append_if("CXX_SUPPORTS_${name}" "${flag}" CMAKE_CXX_FLAGS)
    endif()
endmacro()
