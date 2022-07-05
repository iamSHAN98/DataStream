#include <cstring>
#include "File.h"
using str = std::string;

namespace DataStream{

	void File :: Open(const str& Name, Access Flag){
		Index = 0;

		// Disable error stack
		H5Eset_auto(H5E_DEFAULT, NULL, NULL);

		switch(Flag){
			case Access::Write :
				Vault = H5Fcreate(Name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
				break;

			case Access::ReadWrite :
				Vault = H5Fopen(Name.c_str(), H5F_ACC_RDWR, H5P_DEFAULT); 
				break;

			default :
				Vault = H5Fopen(Name.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT); 
				break;
		}
	}

	void File :: Add(const str& Path, void *Addr, Type DType, 
									 std::vector<hsize_t> D){
		// Obtain string length
		auto Dim = D;
		if(DType == Type::String)
			Dim = {strlen(static_cast<const char*>(Addr))};

		Add(Path, Addr, MetaData(DType, Dim));
	}

	void File :: Add(const str& Path, void *Addr, const MetaData& Info){
		if(CheckPath(Path)) PathError(Path);
		DataSets.push_back(Kernel(Vault, Path, Info));
		Containers.push_back(Addr);
	}

	void File :: Add(const str& Path, void *Addr){
		if(!CheckPath(Path)) NoPathError(Path);
		DataSets.push_back(Kernel(Vault, Path));
		Containers.push_back(Addr);
	}

	void File :: SetAttribute(const str& Path, const str& Name, const void *Addr, 
														Type DType, std::vector<hsize_t> D){
		// Obtain string length
		auto Dim = D;
		if(DType == Type::String)
			Dim = {strlen(static_cast<const char*>(Addr))};

		SetAttribute(Path, Name, Addr, MetaData(DType, Dim));
	}

	void File :: SetAttribute(const str& Path, const str& Name, 
														const void *Addr, MetaData Info){
		if(!CheckPath(Path)) NoPathError(Path);
		WriteAttribute(Vault, Path, Name, Addr, Info);
	}

	void File :: GetAttribute(const str& Path, const str& Name, void *Addr){
		if(!CheckPath(Path)) NoPathError(Path);
		ReadAttribute(Vault, Path, Name, Addr);
	}

	int File :: GetEvent(const str& Path){
		int N = 0;
		GetAttribute(Path, "Event", &N);
		return N;
	}

	std::vector<hsize_t> File :: GetShape(const str& Path){
		hsize_t Dim[10] = {};
		GetAttribute(Path, "Shape", Dim);

		// Rank : index of first zero
		int i = 0;
		while(true)
			if(!Dim[i++]) 
				break;

		return std::vector<hsize_t>(Dim, Dim + i);
	}

}
