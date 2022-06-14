#pragma once
#include "hdf5_hl.h"

namespace DataStream{

	// File access privileges
	typedef enum { Read = 0, Write, ReadWrite } Access;

	// Datatypes
	typedef enum { 
		Compound = -2, String,												// To be constructed 
		Integer, UInteger, Long,											// C datatypes
		ULong, Float, Double,
		LDouble, Bool, HSize,													// Keep adding
		TypeEnd																				// Defines end of list
	} Type;													

	// In that order :D
	const hid_t ListType[] = {
		H5T_NATIVE_INT, H5T_NATIVE_UINT,
		H5T_NATIVE_LONG, H5T_NATIVE_ULONG,
		H5T_NATIVE_FLOAT, H5T_NATIVE_DOUBLE,
		H5T_NATIVE_LDOUBLE, H5T_NATIVE_HBOOL,
		H5T_NATIVE_HSIZE,
	};

	// Compression algorithms
	typedef enum {
		GZip = 0, SZip,
		BZip2, ZStd,
		CompressEnd																		// Define end of list
	} Compress;

}