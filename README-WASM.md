# KDChart with WebAssembly

## Demo

A demo is available at <https://demos.kdab.com/wasm/kdchart>.

## Build tips for KDChart

- Visit <https://doc.qt.io/qt-5/wasm.html> if you haven't yet

- Open a terminal suitable for WASM development (with the correct Qt and toolchain in PATH, etc)

- KDChart can be built with:

```bash
    cmake \
      -DCMAKE_TOOLCHAIN_FILE=/usr/local/emsdk-1.39.8/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
      -DCMAKE_FIND_ROOT_PATH=~/Qt/5.15.1/wasm_32/ -DCMAKE_BUILD_TYPE=Release`
    (Adapt the paths to your own situation)
```

## Builds tips for your own app ==

- Link to KDChart (libkdchart.a, or similar)
