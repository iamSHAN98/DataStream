/*
		MetaData holds structural information about the
		data to be stored, mainly type and dimension.
*/

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Keyword.h"
#include "ErrorHandler.h"

namespace DataStream{

	// Maintain order as in Keyword.h
	const hid_t ListType[] = {
		H5T_NATIVE_INT, H5T_NATIVE_UINT,
		H5T_NATIVE_LONG, H5T_NATIVE_ULONG,
		H5T_NATIVE_FLOAT, H5T_NATIVE_DOUBLE,
		H5T_NATIVE_LDOUBLE, H5T_NATIVE_HBOOL,
		H5T_NATIVE_HSIZE,
	};

	class MetaData{

		private :

			Type DType;
			std::vector<hsize_t> Dim;
			std::string Name;
			std::size_t Size;
			bool IsArray;

			// For compound data-types
			std::vector<MetaData> Members;
			std::vector<size_t> Offsets;

			// Create HDF5 data-types
			hid_t CreateString();
			hid_t CreateCompound();
			hid_t CreateArray(hid_t);

			friend class Kernel;

		public :

			MetaData() = default;

			MetaData(Type T, std::vector<hsize_t> D = {1}, std::string N = ""){
				DType = T, Dim = D, Name = N;
				Size = 0;
				IsArray = false;
			}

			~MetaData() = default;

			// Member initialization of compound data-types
			template <typename A, typename B> constexpr
			void AddMember(B A::*Member, const MetaData& Info){
				if(DType != DataStream::Compound)
					CompoundTypeError();

				if(!Size) Size = sizeof(A);
				Members.push_back(Info);

				/*
					Following definition is taken from Wikipedia
					(https://en.wikipedia.org/wiki/Offsetof)
				*/

				auto Offset = (size_t)&(((A*)0)->*Member);
				Offsets.push_back(Offset);
			}

			template <typename A, typename B> constexpr
			void AddMember(B A::*Member, std::string N, Type T, 
										 std::vector<hsize_t> D = {1}){
				MetaData Info(T, D, N);
				if((D.size() > 1) || (D[0] > 1))
					Info.IsArray = true;

				AddMember(Member, Info);
			}

			// Getter
			hid_t GetDataType();
			hid_t GetDataSpace();
			size_t GetDimension();
	};
	
}