# Using DataStream

The DataStream namespace contains a set of classes and functions to
handle I/O operations for each entry of a dataset. This involves
initializing the necessary HDF5 object instances and calling the
required I/O methods. Following is a brief summary of DataStream
identifiers, at least those required from an user's perspective. For
example use cases, refer to [write.cc](write.cc) and [read.cc](read.cc).

## Keywords

C enum (global) variables (see [include/Keyword.h](../include/Keyword.h)),
given as choices to DataStream classes and functions:

  | Keyword    | Purpose                       |
  | :--------: | :---------------------------: |
  | `Access`   | File access privileges        |
  | `Type`     | HDF5 data-types               |
  | `Compress` | HDF5 data compression filters |

## MetaData

Initialize a `MetaData` instance using the default constructor:

  ```cpp
  MetaData(Type DataType, std::vector<hsize_t> DataShape, std::string Name)
  ```
For user-defined data-types (i.e. `Type::Compound`), use the
`MetaData::AddMember` method to add member variables:

  ```cpp
  AddMember(&Class::Variable, std::string Name, Type DataType, std::vector<hsize_t> DataShape)
  ```
Except `Class::Variable`, the remaining arguments can be replaced
by an equivalent `MetaData` instance. The following methods are
called internally to initialize necessary HDF5 object instances. For
details, see [include/MetaData.h](../include/MetaData.h) and
[src/MetaData.cc](../src/MetaData.cc).

  | Method                   | Purpose                         |
  | :----------------------: | :-----------------------------: |
  | `MetaData::GetDataType`  | Returns HDF5 data-type (`H5T`)  |
  | `MetaData::GetDataSpace` | Returns HDF5 data-space (`H5S`) |

## File

`File` class contains I/O routines for HDF5 files to be invoked from
the user's end. File creation is done using the following constructor,
equivalent to calling the empty constructor and then the
`File::Initialize` method with same arguments:

  ```cpp
  File(std::string FileName, Access FileAccess)
  ```
The basic data handling operation involves storing / accessing a single
entry of a dataset and requires the following methods. For details, see
[include/File.h](../include/File.h) and [src/File.cc](../src/File.cc).

  | Method            | Purpose                                                                              |
  | :---------------: | :----------------------------------------------------------------------------------: |
  | `File::Add`       | Stores dataset path (string) and data- <br>container (pointer) for HDF5 file (`H5F`) |
  | `File::Configure` | Creates HDF5 data-set (`H5D`) instances<br>based on private `MetaData` instances     |
  | `File::Read`      | Loads data from dataset in file<br>to data-container in memory                       |
  | `File::Write`     | Flushes data from memory to file                                                     |

Depending upon `Access::Read` and `Access::Write` (or `Access::ReadWrite`)
there are additional arguments required for `File::Add`. `File::Configure`
internally (re-)creates HDF5 data-sets based on these arguments (when
writing) or from the information stored along with the data-sets.

  #### Read

  When reading from file, only the dataset path and a pointer for an
  appropriate data-container are needed to be passed to `File::Add`:

  ```cpp
  Add(std::string Path, void *Pointer)
  ```

  #### Write

  When writing to file, type, shape of the data (or a `MetaData`
  instance), and the number of entries (default is 1) must be given
  to `File::Add` and `File::Configure`:

  ```cpp
  Add(std::string Path, const void *Pointer, Type DataType, std::vector<hsize_t> DataShape)
  ```

## Kernel

`File::Add` initializes a set of `Kernel` instances, each associated
with a dataset, to handle the creation and initialization of HDF5
objects via the following methods. For details, see [include/Kernel.h](../include/Kernel.h)
and [src/Kernel.cc](../src/Kernel.cc).

  | Method                       | Purpose                                                                        |
  | :--------------------------: | :----------------------------------------------------------------------------: |
  | `Kernel::ConfigureType`      | Configures HDF5 data-type instance (`H5T`)                                     |
  | `Kernel::ConfigureDimension` | Creates HDF5 data-set (`H5D`) based on<br>given dimension (`Shape` x `NEntry`) |
  | `Kernel::ConfigureProperty`  | Handles dataset creation properties e.g.<br>chunking, setting compression etc. |
  | `Kernel::ConfigureAttribute` | Reads / writes default attributes : dataset shape<br>(`Shape`) and number of entries (`NEntry`) |

## Compression

The `Compressor` class adds HDF5 compression filters (`H5Z`) to dataset
creation property list (`H5P`) based on the user's choice of compression 
method and level of compression. For details, see [include/Compressor.h
](../include/Compressor.h)and [src/Compressor.cc](src/Compressor.cc).
The method `Kernel::SetCompression` initializes a `Compressor` instance
internally:

  ```cpp
  SetCompression(Compress Filter, uint Level)
  ```
The default choice is `Compress::GZip` at Level 5. Any other choice
for all the `Kernel` instances can be set by `File::SetCompression` with
same arguments as `Kernel::SetCompression` but must be done before calling
`File::Configure`.

## Attributes

AttributeHandler (see [include/AttributeHandler.h](../include/AttributeHandler.h))
consists of the following functions to work with HDF5 attribute (`H5A`)
instances:

  | Function         | Purpose                                                   |
  | :--------------: | :-------------------------------------------------------: |
  | `FindAttribute`  | Searches for an attribute by name at a given path in file |
  | `WriteAttribute` | Writes an attribute at the specified path from memory     |
  | `ReadAttribute`  | Reads an attribute by name from given path to memory      |

These are used internally by the following  `File` methods:

  ```cpp
  SetAttribute(std::string Path, std::string Name, const void *Pointer, MetaData)
  GetAttribute(std::string Path, std::string Name, void *Pointer)
  ```
Path may point to a data-set or a group (created as an intermediate link). All
I/O operations for attributes can only happen after calling `File::Configure`.
As mentioned already, each data-set created via `Kernel` instances has 2 default
attributes: `NEntry` (integer) and `Shape` (1D integer array).

## Errors

ErrorHandler consists of functions that use `stdexcept` (C++ STL), to
report short error messages, replacing the HDF5 error-stack (`H5E`).
However, these are for internal use only. For details, see
[include/ErrorHandler.h](../include/ErrorHandler.h).
