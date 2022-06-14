/*
		File-handler for storing and retrieving data
		event-by-event in HDF5 format.

		Refer to 'File.cc' in 'src' directory for the
		definitions of rest of the class methods.

		Refer to examples given in 'example' directory 
		for possible use cases.
*/

#pragma once
#include "Kernel.h"
#include "AttributeHandler.h"

namespace DataStream{

	class File{

		private :

			hid_t Vault;

			int Index;
			std::vector<Kernel> DataSets;
			std::vector<void*> Containers;

		public :

			File() = default;

			File(const std::string& Name, Access Flag){
				Open(Name, Flag);
			}

			~File() = default;
			
			// Initialize Kernel instances
			void Add(const std::string&, void *Addr, Type, 
							 std::vector<hsize_t> Dim = {1});
			void Add(const std::string&, void*, const MetaData&);
			void Add(const std::string&, void*);

			// Load datasets
			void Configure(int N = 1){
				while(Index < DataSets.size())
					DataSets[Index++].Configure(N);
			}

			// Data compression
			void SetCompression(Compress Filter, int Level){
				for(auto& Entry : DataSets)
					Entry.SetCompression(Filter, Level);
			}
			
			// Handling dataset attributes
			void SetAttribute(const std::string& Path, const std::string& Name, 
												const void*, Type, std::vector<hsize_t> Dim = {1});
			void SetAttribute(const std::string&, const std::string&, const void*, 
												MetaData);
			void GetAttribute(const std::string&, const std::string&, void*);

			// Basic file operations
			void Open(const std::string&, Access);

			void Write(){
				for(int i = 0; i < DataSets.size(); i++)
					DataSets[i].Write(Containers[i]);
			}

			void Read(int I = 0){
				for(int i = 0; i < DataSets.size(); i++)
					DataSets[i].Read(Containers[i], I);
			}

			void Close(){
				for(auto& Entry : DataSets)
					Entry.Close();
				H5Fclose(Vault);
			}

			bool CheckPath(const std::string& Path){
				return H5LTpath_valid(Vault, Path.c_str(), true);
			}

			bool CheckAttribute(const std::string& Path, const std::string& Name){
				return FindAttribute(Vault, Path, Name);
			}

			// Getter
			int GetEvent(const std::string&);
			std::vector<hsize_t> GetShape(const std::string&);
	};

}