"""
    get_data : Returns dataset as numpy array. Flattens it in
               case of scalar data
"""
def get_data(file, path) :
    dset = file[path]
    dim = dset.attrs["Shape"]
    nev = dset.attrs["NEntry"][0]

    arr = dset[:]

    if dim[0] > 1 :
        return arr.reshape(nev, *dim)

    return arr

"""
    get_entry : Returns a single entry : 'index' corresponds
                to entry no 'index + 1'
"""
def get_entry(file, path, index = 0) :
    dset = file[path]
    dim_x = int(dset.attrs["Shape"][0])

    start, end = index*dim_x, (index + 1)*dim_x
    arr = dset[start:end]

    if len(dset) == 1 :
        if arr.ndim == 1 :        
            return arr.flatten()[0]     # Scalar data
        
        return arr[0]                   # Vector data

    return arr                          # Matrix data

"""
    get_nentry : Returns total no of entrys, stored as an
                attribute of corresponding dataset
"""
def get_nentry(file, path) :
    return file[path].attrs['NEntry'][0]

"""
    get_shape : Returns shape of data for a single entry,
                stored as an attribute of corresponding
                dataset.
"""
def get_shape(file, path) :
    return file[path].attrs["Shape"]

"""
    get_dimension : Returns dimension of data
"""
def get_dimension(file, path) :
    return file[path].attrs["Shape"].ndim

"""
    get_attribute : Returns specified attribute of given
                    dataset
"""
def get_attribute(file, path, attr) :
    obj = file[path].attrs[attr]
    return obj[0] if len(obj) == 1 else obj
