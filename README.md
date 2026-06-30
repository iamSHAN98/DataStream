# DataStream

DataStream is a C++ wrapper for [HDF5 C API](http://portal.hdfgroup.org/display/HDF5/Core+Library)
to store large datasets in HDF5 files by its entries. Possible use
cases may involve storing output of a Monte Carlo event generator
(per event), or storing solution at each step of an initial value
problem (per step). In principle, each entry of a dataset is stacked
on top of each other (e.g. along its first dimension) to represent
the entirety of it. DataStream can be used to simplify such a
data-storing process for HDF5 files within a few lines of code.

## Installation

### Pre-Requisites

  - `cmake` to build DataStream and external plugins.
  - `zlib` and `libaec` for compression methods GZip and SZip provided
    with HDF5.

  One may use the package manager for installing these. For Debian-based
  distros:

  ```shell
  sudo apt install cmake
  sudo apt install zlib1g-dev libaec-dev
  ```

### HDF5 C

The easiest way to install the HDF5 library and the required binaries,
is again using the package manager:

```shell
sudo apt install libhdf5-dev hdf5-tools
```
or, do it manually as outlined below.

#### Manual Installation (Long & Boring)

<details>
  <summary> Expand </summary>

  DataStream requires only the HDF5 C bindings which can be compiled
  from source:

  - Define an installation path and an environment variable leading to
    it for the ease of configuration as mentioned later:

    ```shell
    mkdir path/to/HDF5
    export HDF5Path=path/to/HDF5
    ```
    *Append the last line to the shell profile file (e.g.* `.bashrc`
    *in case of Bash)*.

  - Download and extract the HDF5 [source](https://github.com/HDFGroup/hdf5/tags)
    (pick any):

    ```shell
    tar -xzf hdf5-x_yy_z.tar.gz
    cd hdf5-x_yy_z
    ```

  - Execute the `configure` script specifying installation path and
    configuration options for compression libraries of choice:

    ```shell
    ./configure --prefix=$HDF5Path --with-zlib --with-szlib
    make -jN install
    ```
    *Replace* N *with the desired number of threads for parallel
    compilation*.

  - Add HDF5 binaries and scripts to `PATH` and verify whether the
    installation was successful or not:

    ```shell
    export PATH=$PATH:$HDF5Path/bin
    h5cc --version
    ```

</details>

#### External Plugins (Optional)

<details>
  <summary> Expand </summary>

  HDF5 (version >= 1.8.11) enables [dynamic filter loading
  ](https://support.hdfgroup.org/releases/hdf5/documentation/rfc/HDF5DynamicallyLoadedFilters.pdf)
  e.g. applying non-native compression filters at runtime.
  DataStream comes configured with two such methods, BZip2
  and ZStd.

  - Define the HDF5 installation path (`HDF5Path`) only if installed
    using the package manager. For Linux, it should be installed in
    `/usr`: headers in `/usr/include/hdf5` and libraries in
    `/usr/lib/$(uname -m)-linux-gnu/hdf5` (or something similar).

  - Install the required libraries:
    
    ```shell
    sudo apt install libbz2-dev libzstd-dev
    ```

  - Install the [BZip2 filter plugin](https://github.com/nexusformat/HDF5-External-Filter-Plugins.git):

    ```shell
    git clone https://github.com/nexusformat/HDF5-External-Filter-Plugins.git
    cd HDF5-External-Filter-Plugins
    cmake . -DCMAKE_INSTALL_PREFIX=$HDF5Path -DENABLE_BZIP2_PLUGIN=True
    make install
    ```
    *The plugin gets installed at* `HDF5Path/lib/plugins`.

  - Install the [ZStd filter plugin](https://github.com/aparamon/HDF5Plugin-Zstandard):
    
    ```shell
    git clone https://github.com/aparamon/HDF5Plugin-Zstandard.git
    cd HDF5Plugin-Zstandard
    cmake . -DPLUGIN_INSTALL_PATH=$HDF5Path/lib/plugins
    make install
    ```

  - Define an environment variable for the plugin installation path and
    append the same to the shell profile:

    ```shell
    export HDF5PluginPath=$HDF5Path/lib/plugins
    ```

</details>

### DataStream

At this point, building DataStream as a standalone library is pretty
straight-forward:

```shell
git clone https://github.com/iamSHAN98/DataStream.git
cd DataStream && mkdir build && cd build
cmake .. && make -jN
```
This defines the CMake variables, `DataStreamInclude` (header files) and
`DataStream` (dynamically linked library) that can be used to link code
against DataStream (see [example/CMakeLists.txt](example/CMakeLists.txt)).
In that case, provide DataStream source path to the project's CMakeLists.txt:

  ```cmake
  # Project's CMakeLists.txt
  set(DataStreamPath path/to/DataStream)
  add_subdirectory(${DataStreamPath} DataStream)
  ```

## Using DataStream

See [example/README.md](example/README.md).

## DataStream in Python

See [python/README.md](python/README.md).
