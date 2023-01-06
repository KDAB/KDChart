#!/usr/bin/env python

# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2021-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

# Run this on Linux.

import os
import stat

# This script, when run in KD Chart's source and build directory containing the results of a full
# build, writes a list of files to be excluded from packaging to CPackIgnores.txt.
# autogen reads that list and passes it to CPack.

# You should re-generate CPackIgnores.txt before every release and also check that no new binary
# slips through the cracks using
# find . -type f -executable

sourceDirectory = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))


def ignoredFiles():
    ret = []

    def findExecutables(top):
        for f in os.listdir(top):
            pathname = os.path.join(top, f)
            if stat.S_ISDIR(os.stat(pathname).st_mode):
                findExecutables(pathname)
            elif os.access(pathname, os.X_OK):
                # The file is executable for us
                ret.append(pathname + '$')
                # for OS X
                ret.append(pathname + '.app/')

    # The only executables in those paths are binaries and we're making a SOURCE package.
    for path in ['examples', 'tests', 'qtests']:
        findExecutables(os.path.join(sourceDirectory, path))

    return sorted(ret)


f = open('CPackIgnores.txt', 'w')
for ign in ignoredFiles():
    # write paths relative to the source dir, one per line
    f.write(ign[len(sourceDirectory):] + '\n')
