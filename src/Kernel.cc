#include "Kernel.h"
#include "AttributeHandler.h"
using str = std::string;

namespace DataStream{

	void Kernel :: Initialize(hid_t Grp, const str& AbsPath, MetaData In){
		Root = Grp, Path = AbsPath, Info = In;
		Filter = Compress::GZip, Level = 5;				// Default compression parameters
		ReadMode = false;
	}

	void Kernel :: Initialize(hid_t Grp, const str& AbsPath){
		Root = Grp, Path = AbsPath;
		ReadMode = true;
	}

	void Kernel :: Configure(int Length){
		if(ReadMode){
			// Open dataset
			DSet = H5Dopen(Root, Path.c_str(), H5P_DEFAULT);
			if(DSet < 0) LinkAccessError(Path);

			// Configure HDF5 instances
			ConfigureAttribute();
			ConfigureType();
			ConfigureDimension();
			ConfigureProperty();
		}
		else{
			NEntry = Length;

			// Configure HDF5 instances
			ConfigureType();
			ConfigureDimension();
			ConfigureProperty();

			// Create dataset
			DSet = H5Dcreate(Root, Path.c_str(), DType, DSpace, CProp, DProp, H5P_DEFAULT);
    	if(DSet < 0) LinkCreationError(Path);

    	ConfigureAttribute();
		}
	}

	void Kernel :: Write(const void *Data){
		if(ReadMode) return;

		if(Index[0] >= DataDim[0]) return;
		
		// Select subset
		H5Sselect_hyperslab(DSpace, H5S_SELECT_SET, Index.data(), NULL, Info.Dim.data(), NULL);

		// Write data
		H5Dwrite(DSet, DType, ESpace, DSpace, H5P_DEFAULT, Data); 

		// Update index
		Index[0] += Info.Dim[0];
	}

	void Kernel :: Read(void *Data, int I){
		if(!ReadMode) return;

		// Set index
		Index[0] = I*Info.Dim[0];
		if(Index[0] >= DataDim[0]) return;
		
		// Select subset
		H5Sselect_hyperslab(DSpace, H5S_SELECT_SET, Index.data(), NULL, Info.Dim.data(), NULL);

		// Read data
		H5Dread(DSet, DType, ESpace, DSpace, H5P_DEFAULT, Data);
	}

	void Kernel :: ConfigureType(){
		if(ReadMode) DType = H5Dget_type(DSet);
		else DType = Info.GetDataType();
	}

	void Kernel :: ConfigureDimension(){
		if(ReadMode){
			DSpace = H5Dget_space(DSet);
    	DataDim.resize(H5Sget_simple_extent_ndims(DSpace));
    	H5Sget_simple_extent_dims(DSpace, DataDim.data(), NULL);

			Info.Dim = DataDim; 
			Info.Dim[0] = DataDim[0]/NEntry;
			ESpace = Info.GetDataSpace();
		}
		else{
			ESpace = Info.GetDataSpace();

			DataDim = Info.Dim; 
			DataDim[0] *= NEntry;
			
			auto MaxDim = Info.Dim; 
			MaxDim[0] = H5S_UNLIMITED;	
			
			DSpace = H5Screate_simple(DataDim.size(), DataDim.data(), MaxDim.data());
		}

		Index.resize(Info.Dim.size(), 0);	// Index at zero
	}
		
	void Kernel :: ConfigureProperty(){
		if(ReadMode){
			DProp = H5Dget_create_plist(DSet);
			Obj.GetCompression(DProp, Filter, Level);
		}
		else{
			DProp = H5Pcreate(H5P_DATASET_CREATE);
			
			// Create chunked data
			auto ChunkDim = Info.Dim; 
			ChunkDim[0] = (ChunkMin > NEntry) ? NEntry : ChunkMin;
    	H5Pset_chunk(DProp, ChunkDim.size(), ChunkDim.data());
    	
    	// Compress data
    	Obj.SetCompression(DProp, Filter, Level);

    	// Create intermediate links
    	CProp = H5Pcreate(H5P_LINK_CREATE);
			H5Pset_create_intermediate_group(CProp, true);
		}
	}

	void Kernel :: ConfigureAttribute(){
		if(ReadMode){
			ReadAttribute(DSet, ".", "NEntry", &NEntry);
			ReadAttribute(DSet, ".", "Shape", Info.Dim.data());
		}
		else{
			WriteAttribute(DSet, ".", "NEntry", &NEntry, MetaData(Type::Integer));
			WriteAttribute(DSet, ".", "Shape", Info.Dim.data(), MetaData(Type::HSize, Info.Dim));
		}
	}

}