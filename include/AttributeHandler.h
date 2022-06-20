/*
		Methods for searching, creating and reading
		attributes from given  path, implemented in
		setter, getter methods of Kernel and File.
*/

#pragma once
#include "MetaData.h"
#include "ErrorHandler.h"

namespace DataStream{

	static bool FindAttribute(hid_t Loc, std::string Path, std::string Name){
		return (H5Aexists_by_name(Loc, Path.c_str(), Name.c_str(), H5P_DEFAULT) > 0);
	}

	static void WriteAttribute(hid_t Loc, std::string Path, std::string Name,
														 const void *Addr, MetaData Info){
		if(FindAttribute(Loc, Path, Name)) AttributeError(Name, Path);

		hid_t AType = Info.GetDataType();
		hsize_t ADim = Info.GetDimension();
		hid_t ASpace = H5Screate_simple(1, &ADim, NULL);
		hid_t Attr = H5Acreate_by_name(Loc, Path.c_str(), Name.c_str(), 
								 AType, ASpace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		
		H5Awrite(Attr, AType, Addr);

		H5Tclose(AType);
		H5Sclose(ASpace);
		H5Aclose(Attr);
	}

	static void ReadAttribute(hid_t Loc, std::string Path, std::string Name,
														void *Addr){
		if(!FindAttribute(Loc, Path, Name)) NoAttributeError(Name, Path);

		hid_t Attr = H5Aopen_by_name(Loc, Path.c_str(), Name.c_str(), 
																 H5P_DEFAULT, H5P_DEFAULT); 
		hid_t AType = H5Aget_type(Attr);
		
		H5Aread(Attr, AType, Addr);

		H5Tclose(AType);
		H5Aclose(Attr);
	}

}