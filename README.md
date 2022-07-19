# DataStream

DataStream is a C++ wrapper based on [HDF5 C API](http://portal.hdfgroup.org/display/HDF5/Core+Library)
for  storing data  in an  eventwise (e.g. Monte Carlo 
event  generation) or stepwise  (e.g. solving initial 
value  problems) manner.  In principle,  data for  an 
event  (step)  is  stacked  on top  of data  from the
previous event  (step). In case  of multi-dimensional 
data, such stacking  is always  done  along the first
axis. The  very purpose of DataStream  is to simplify
such  a data-storing  process in  HDF5 files within a
few lines of code and also generalize it for any type
of data with arbitrary dimensions.

## Installation

### Pre-requisites
  - `cmake` to build DataStream and external plugins,
  - `zlib` and `libaec` for pre-defined  compression
    methods GZip and SZip provided with HDF5

  One  may use  the package manager for installation.
  For Debian-based distros

  ```shell
  sudo apt install cmake
  sudo apt install zlib1g-dev libaec-dev
  ```

### HDF5 C

<details>
  <summary> Expand </summary>

  DataStream requires  only  C bindings  that  can be
  built from source

  - Create an  installation directory  and  define an
    environment  variable  using  its' absolute  path
    (for ease of configuration later)

    ```shell
    mkdir path/to/HDF5
    export HDF5_Path=path/to/HDF5
    ```

    *Append last line to* `.bashrc` (*or* `.profile`)

  - Extract the [source](https://github.com/HDFGroup/hdf5/tags) (pick any one)

    ```shell
    tar -xzf hdf5-hdf5-x_yy_z.tar.gz
    ```

  - Launch `configure` script specifying installation
    path  and configuration  options for  compression 
    libraries

    ```shell
    cd hdf5-hdf5-x_yy_z
    ./configure --prefix=$HDF5_Path --with-zlib --with-szlib
    make -jN install
    ```

    *Replace* `N` *with desired number of threads for
     parallel compilation*

  - Add HDF5 binaries and scripts to `PATH` and verify
    installation

    ```shell
    export PATH=$PATH:$HDF5_Path/bin
    h5cc --version
    ```

</details>

### External Plugins (Optional)

<details>
  <summary> Expand </summary>

  HDF5 (version >= `1.8.11`) enables [dynamic filter loading](https://docs.hdfgroup.org/hdf5/rfc/HDF5DynamicallyLoadedFilters.pdf) 
  e.g.  applying  non-native  compression  filters at 
  runtime. DataStream  comes configured with two such
  methods BZip2 and ZStd.

  - Install corresponding libraries
    
    ```shell
    sudo apt install libbz2-dev libzstd-dev
    ```

  - Install [BZip2 filter plugin](https://github.com/nexusformat/HDF5-External-Filter-Plugins.git)

    ```shell
    git clone https://github.com/nexusformat/HDF5-External-Filter-Plugins.git
    cd HDF5-External-Filter-Plugins
    cmake . -DCMAKE_INSTALL_PREFIX=$HDF5_Path -DENABLE_BZIP2_PLUGIN=True
    make install
    ```

    *Plugin gets installed at* `HDF5_Path/lib/plugins`

  - Install [ZStd filter plugin](https://github.com/aparamon/HDF5Plugin-Zstandard)
    
    ```shell
    git clone https://github.com/aparamon/HDF5Plugin-Zstandard.git
    cd HDF5Plugin-Zstandard
    cmake . -DPLUGIN_INSTALL_PATH=$HDF5_Path/lib/plugins
    make install
    ```

  - Define environment variable for plugin installation
   (append to `.bashrc`)

    ```shell
    export HDF5_PLUGIN_PATH=$HDF5_Path/lib/plugins
    ```

</details>

### DataStream

Building DataStream as a standalone library is pretty
straight-forward

```shell
git clone https://github.com/iamSHAN98/DataStream.git
cd DataStream
mkdir build && cd build
cmake ..
make -jN
```

Above defines the CMake variables `DataStream_INCLUDE`
(associated  header files) and  `DataStream` (shared
library object `libDataStream`) required for linking
code against DataStream as an external library (see
[example/CMakeLists.txt](example/CMakeLists.txt)). In that case, provide 
DataStream source path to project's CMakeLists.txt

  ```cmake
  # Project's CMakeLists.txt
  set(DSPATH path/to/source)
  add_subdirectory(${DSPATH} DataStream)
  ```

## [Using DataStream](example/README.md)

## [DataStream in Python](python/README.md)
