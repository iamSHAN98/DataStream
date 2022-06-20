# Using DataStream

DataStream is a namespace region organizing a set of
modules  and functions to handle  event-by-event I/O
operations in HDF5 files : automating initialization
of HDF5 object instances and calling I/O methods.

*Following is a brief introduction to DataStream 
identifiers, at least those required from an user's
perspective* (*refer to* [write.cc](write.cc) *and*
[read.cc](read.cc)).

## [Keywords](../include/Keyword.h)

<details>
  <summary> Expand </summary>

  `C enum`  (global) variables  as choices  given to 
  modules

  - `Access` : file access privileges

  - `Type` : HDF5 data-types

  - `Compress` : data compression filters

</details>

## [MetaData](../include/MetaData.h)

<details>
  <summary> Expand </summary>

  Initialize  `MetaData` instance using default (and 
  only) constructor

  ```cpp
  MetaData(Type DataType, vector<hsize_t> DataShape, string Name)
  ```

  For user-defined data-types (`Type::Compound`), use 
  `AddMember` method to add member variables

  ```cpp
  AddMember(&Class::Variable, string Name, Type DataType, vector<hsize_t> DataShape)
  ```

  `Class`, `Variable`  are template arguments. Rest
  of the arguments can be replaced by an equivalent
  `MetaData` instance. The methods

  - `GetDataType` creates HDF5 data-type (`H5T`)

  - `GetDataSpace`  creates  HDF5 data-space (`H5S`)

  However, these methods are for internal use only.

</details>

## [File](../include/File.h)

<details>
  <summary> Expand </summary>

  `File` is the module for invoking HDF5 I/O routines
  from the user's end. Initialization  is done using
  the constructor

  ```cpp
  File(string FileName, Access FileAccess)
  ```

  This is equivalent to calling the empty  instructor
  followed by `Initialize` method with same arguments. 
  Basic data handling requires calling three methods

  1. `Add` : initialize `File` instance with dataset
     path and data-container (as pointer)

  2. `Configure` : create HDF5 data-type, data-space
     and  data-set  (`H5D`) instances  in order  to 
     access dataset at given path in file

  3. `Read` / `Write`

  Depending upon `Access::Read` and  `Access::Write`
  there  are additional arguments  to above methods.
  `Access::ReadWrite`  grants read and write access
  to different datasets within same file.

### Write

- Type and shape of data (or `MetaData` instance)
  must be provided through `Add`

  ```cpp
  Add(string Path, const void *Pointer, Type DataType, vector<hsize_t> DataShape)
  ```

- Number of events (or steps, default is 1) through
  `Configure`

### Read

No additional input required

```cpp
Add(string Path, void *Pointer)
```

`Configure` internally re-constructs DataStream
identifiers from information imprinted along the
data-set being read.

</details>

## [Kernel](../include/Kernel.h)

<details>
  <summary> Expand </summary>

  `File::Add` internally creates a `Kernel` instance
  that  explicitly works  with HDF5 module to access
  data-sets. Hence, `File`  is just an interface for
  a bunch of `Kernel` instances which in turn is the
  same for data-sets being read and written to.

</details>

## Compression

<details>
  <summary> Expand </summary>

  [Compressor](../include/Compressor.h)  adds compression filter (`H5Z`)
  to data-set creation property list  (`H5P`) based
  on  compression  method and  level provided. Each
  `Kernel`  instance has `Compressor` instance as a
  private  member. Interface  for initializing  all
  at once is the `File` method

  ```cpp
  SetCompression(Compress Filter, uint Level)
  ```

  It must be done before `File::Configure` otherwise
  default options are chosen :  `Compress::GZip` and
  `Level = 5`.

</details>

## Attributes

<details>
  <summary> Expand </summary>

  [AttributeHandler](../include/AttributeHandler.h) consists of base functions
  for work with attributes 

  - `FindAttribute` : looks for an attribute by name
    at given path

  - `WriteAttribute` : writes an attribute at given
    path with data-type, data-shape, data-container
    specified for its value

  - `ReadAttribute` : reads an attribute by name from
    given path with data-container specified to store
    its value

  These are used internally by the following  `File`
  methods

  ```cpp
  SetAttribute(string Path, string Name, const void *Pointer, MetaData)
  GetAttribute(string Path, string Name, void *Pointer)
  ```

  Path may point to a data-set or a group (created
  as a link during data-set creation) but it must
  exist beforehand for DataStream to work on. Hence,
  these can only be called after  `File::Configure`.
  Each data-set created with `Kernel` has 2 default
  attributes : `Event` and `Shape`.

</details>

## Errors

<details>
  <summary> Expand </summary>

  [ErrorHandler](../include/ErrorHandler.h) consists  of functions, using
  `stdexcept`  (C++  STL), to provide  short  error 
  messages  in  place of  HDF5 error-stack (`H5E`). 
  However, these are for internal use only.

</details>