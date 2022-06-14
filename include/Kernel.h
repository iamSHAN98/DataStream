/*
		Kernel methods provide means of reading and
		writing data to HDF5 datasets.

		Refer to 'Kernel.cc' in 'src' directory for
		definitions of rest of the class methods.

		Refer to examples given in 'example' directory 
		for possible use cases.
*/

#pragma once
#include "MetaData.h"
#include "Compressor.h"

namespace DataStream{

	class Kernel{

		// Inputs
		MetaData Info;
		std::string Path;
		bool ReadMode;

		// Compression
		Compress Filter;
		int Level;
		Compressor Obj;

		// HDF5 object identifiers
		hid_t Root, DSet;
		hid_t EventSpace, DSpace;								// Dataspace for single & all events
		hid_t DType;
		hid_t CProp, DProp;											// Link & dataset creation properties

		// Kernel specific attributes
		int N;																	// Total events
		std::vector<hsize_t> Index;							// Data index in dataset
		std::vector<hsize_t> DataDim;						// Dataset dimension

		// Constants
		static const int ChunkMin = 100;

		// Initialization steps
		void ConfigureType();
		void ConfigureDimension();
		void ConfigureProperty();
		void ConfigureAttribute();							// Default attributes : dimension, total events

		public :

			Kernel() = default;
			~Kernel() = default;

			// Write mode
			Kernel(hid_t Grp, const std::string& Name, MetaData In){
				Initialize(Grp, Name, In);
			}

			// Read mode			
			Kernel(hid_t Grp, const std::string& Name){
				Initialize(Grp, Name);
			}

			// Initializers
			void Initialize(hid_t, const std::string&, MetaData);
			void Initialize(hid_t, const std::string&);

			// Basic operations
			void Configure(int Length = 1);

			void Write(const void*);
			void Read(void*, int I = 0);
			void Close(){
				H5Tclose(DType);
				H5Sclose(EventSpace); H5Sclose(DSpace);
				H5Pclose(DProp);
				if(!ReadMode) H5Pclose(CProp);
				H5Dclose(DSet);
			}

			// Data compression
			void SetCompression(Compress Choice, int Strength){
				Filter = Choice, Level = Strength;
			}

			// Getter
			int GetEvent(){ return N; }
			std::vector<hsize_t> GetDimension(){ return Info.Dim; }
			std::string GetPath(){ return Path; }
	};

}