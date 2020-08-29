In the following example we assume you have the kdchart source available in
/path/to/kdchart-source.  replace '/path/to/kdchart-source' to fit your needs.

$ conan create -s build_type=Release -o kdchart:build_examples=True -o kdchart:build_tests=True /path/to/kdchart-source/conan kdab/stable

Configuration options:
 * build_static
   Builds static versions of the libraries. Default=False

 * build_tests
   Build the test harness. Default=False

 * build_examples
   Build the examples. Default=True

 * build_python_bindings
   Build/Generate python bindings (always false for Debug or static builds). Default=False

 * python_bindings_install_dir
   Alternative install path for python bindings. Default=auto detect
