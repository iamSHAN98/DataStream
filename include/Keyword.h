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
		TypeEnd																				// End of list
	} Type;

	// Compression algorithms
	typedef enum {
		GZip = 0, SZip,
		BZip2, ZStd,
		CompressEnd																		// End of list
	} Compress;

}