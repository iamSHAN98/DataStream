/*
		Kernel methods provide means of reading and
		writing data to HDF5 datasets.
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
		uint Level;
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

			// Write mode
			Kernel(hid_t Grp, const std::string& Name, MetaData In){
				Initialize(Grp, Name, In);
			}

			// Read mode			
			Kernel(hid_t Grp, const std::string& Name){
				Initialize(Grp, Name);
			}

			~Kernel() = default;

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
			void SetCompression(Compress Choice, uint Strength){
				if(ReadMode) return;
				Filter = Choice, Level = Strength;
			}

			// Getter
			int GetEvent(){ return N; }
			std::vector<hsize_t> GetShape(){ return Info.Dim; }
			std::string GetPath(){ return Path; }
	};

}