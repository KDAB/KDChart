 Running the examples

The examples are only compiled automatically if you pass the -unittests option
to configure. This is because some examples require the "testtools" internal
library that is not installed when configured without -unittests.

If you run configure without -unittests, you can still compile the examples
by running "make" in the examples directory. For those examples that require
the testtools library to run, you can help the linker find it by adding
/path/to/kdchart_build_dir/lib to the library search path.

The library search path is called:
- Linux: LD_LIBRARY_PATH
- Windows: PATH
- Mac OS X: DYLD_LIBRARY_PATH
