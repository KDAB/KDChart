#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2010-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

from conans import ConanFile, CMake, tools


class KdchartConan(ConanFile):
    name = "KDChart"
    version = "3.0.1"
    license = ("https://raw.githubusercontent.com/KDAB/KDChart/master/LICENSES/MIT.txt")
    author = "Klaralvdalens Datakonsult AB (KDAB) info@kdab.com"
    url = "https://github.com/KDAB/KDChart.git"
    description = "KD Chart is a tool for creating business and scientific charts."
    generators = "cmake"
    options = dict({
        "build_static": [True, False],
        "build_examples": [True, False],
        "build_tests": [True, False],
        "build_python_bindings": [True, False],
        "python_bindings_install_prefix": "ANY"
    })
    default_options = dict({
        "build_static": False,
        "build_examples": True,
        "build_tests": False,
        "build_python_bindings": False,
        "python_bindings_install_prefix": "ANY"
    })
    settings = "build_type"

    def requirements(self):
        # Kdab uses a custom qt build
        # To build qt with extra modules enabled use qt package from:
        #   https://github.com/bincrafters/conan-qt
        # Build the command:
        #   conan create -ks -o qt:qtsvg=True -o qt:qtdeclarative=True -o qt:qtremoteobjects=True -o qt:qtscxml=True . 5.13.2@kdab/stable
        self.requires("qt/5.13.2@kdab/stable")

    def source(self):
        git = tools.Git(folder="")
        git.clone(self.url)
        # We want cmake support, so use master for now
        # git.checkout("kdchart-%s-release"%self.version)

    def configure(self):
        # Use kdab flags to match qt package hash
        # ~$ conan create -ks -o qt:qttools=True -o qt:qtsvg=True -o qt:qtdeclarative=True -o qt:qtremoteobjects=True -o qt:qtscxml=True . 5.13.2@kdab/stable
        self.options["qt"].qtsvg = True
        self.options["qt"].qtdeclarative = True
        self.options["qt"].qtremoteobjects = True
        self.options["qt"].qtscxml = True
        self.options["qt"].qttools = True

    def build(self):
        self.cmake = CMake(self)
        self.cmake.definitions["KDChart_STATIC"] = self.options.build_static
        self.cmake.definitions["KDChart_EXAMPLES"] = self.options.build_examples
        self.cmake.definitions["KDChart_TESTS"] = self.options.build_tests
        self.cmake.definitions["KDChart_PYTHON_BINDINGS"] = self.options.build_python_bindings
        if self.options.python_bindings_install_dir:
            self.cmake.definitions["KDChart_PYTHON_BINDINGS_INSTALL_PREFIX"] = self.options.python_bindings_install_dir
        self.cmake.configure()
        self.cmake.build()

    def package(self):
        self.cmake.install()

    def package_info(self):
        self.env_info.CMAKE_PREFIX_PATH.append(self.package_folder)
