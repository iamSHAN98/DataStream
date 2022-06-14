#include "MetaData.h"

namespace DataStream{

	hid_t MetaData::CreateString(){
		return H5Tcreate(H5T_STRING, H5T_VARIABLE);
	}

	hid_t MetaData::CreateCompound(){
		hid_t T = H5Tcreate(H5T_COMPOUND, Size);
		for(int i = 0; i < Members.size(); i++)
			H5Tinsert(T, Members[i].Name.c_str(), Offsets[i], Members[i].GetDataType());
		return T;
	}

	hid_t MetaData :: CreateArray(hid_t Base){
		return H5Tarray_create(Base, Dim.size(), Dim.data());
	}

	hid_t MetaData::GetDataType(){
		hid_t T;

		if(DType < 0){
			switch(DType){
				case Type::Compound : T = CreateCompound();
															break;
				case Type::String 	: T = CreateString();
															break;
				default 						: DataTypeError();
			}
		}
		else if(DType >= TypeEnd)
			DataTypeError();
		else
			T = H5Tcopy(ListType[DType]);

		if(IsArray){
			auto Base = T;
			T = CreateArray(Base);
			H5Tclose(Base);
		}

		return T;
	}

	hid_t MetaData::GetDataSpace(){
		return H5Screate_simple(EventDim.size(), EventDim.data(), NULL);
	}

	size_t MetaData :: GetDimension(){ return EventDim.size(); }

}