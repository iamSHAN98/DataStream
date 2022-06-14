#pragma once
#include <string>
#include "hdf5.h"

#define H5Z_FILTER_BZIP2	307
#define H5Z_FILTER_ZSTD		32015

namespace DataStream{

	struct Algo{
		H5Z_filter_t Filter;
		std::string Name = "";
		uint Min, Max, Step = 1;
	};

	// Maintain order as in Keyword.h
	const Algo ListAlgo[] = {
		{ H5Z_FILTER_DEFLATE, "GZip",		1, 9 		 },
		{ H5Z_FILTER_SZIP, 		"SZip", 	2, 32, 2 },
		{ H5Z_FILTER_BZIP2, 	"BZip2", 	1, 9 		 },
		{ H5Z_FILTER_ZSTD, 		"ZStd", 	1, 22 	 },
	};

}