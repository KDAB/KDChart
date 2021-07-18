#!/usr/bin/env python
from autogen.autogen import autogen

project = "KDChart"
version = "2.8.0"
subprojects = ["KDChart", "KDGantt"]
prefixed = True

autogen(project, version, subprojects, prefixed, policyVersion = 2)
