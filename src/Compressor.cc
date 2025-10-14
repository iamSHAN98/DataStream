#include <iostream>
#include "ErrorHandler.h"
#include "Compressor.h"

namespace DataStream{

	bool Compressor :: CheckAlgorithm(H5Z_filter_t Id){
		uint Filter;
		if(H5Zfilter_avail(Id)){
			H5Zget_filter_info(Id, &Filter);

			if(Filter & H5Z_FILTER_CONFIG_ENCODE_ENABLED)
					if(Filter & H5Z_FILTER_CONFIG_DECODE_ENABLED)
						return true;
		}
		return false;
	}

	void Compressor :: SetCompression(hid_t DProp, Compress Choice,
																		uint Strength){
		// Construct filter object
		try{
			if((Choice < 0) || (Choice >= CompressEnd))
				CompressionError(std::to_string(Choice));

			Obj = ListAlgo[Choice];
			if(!CheckAlgorithm(Obj.Filter)) CompressionError(Obj.Name);

			int n = (Strength - Obj.Min)/Obj.Step;					// Set compression level
			Level = Obj.Min + n*Obj.Step;										// Between Min and Max
			Level = (Level > Obj.Max) ? Obj.Max : Level;		// and multiple of Step 

			// Set filter
			switch(Obj.Filter){
				case H5Z_FILTER_DEFLATE :
					H5Pset_deflate(DProp, Level);
					break;

				case H5Z_FILTER_SZIP :
					H5Pset_szip(DProp, H5_SZIP_NN_OPTION_MASK, Level);
					break;

				default :
					H5Pset_filter(DProp, Obj.Filter, H5Z_FLAG_MANDATORY, 1, &Level);
					break;
			}
		}
		catch(std::domain_error Err){
			std::cout << Err.what() << "\nUsing default method : GZip\n";
			SetCompression(DProp, Compress::GZip, 5);
		}
	}

	void Compressor :: GetCompression(hid_t DProp, Compress& Choice,
																		uint& Strength){
		// Dummy variables
		char Name[50];
		uint Flag;
		size_t Size = 1;

		// Get filter
		Obj.Filter = H5Pget_filter(DProp, 0, &Flag, &Size, &Level, 
															 sizeof(Name), Name, NULL);

		// Re-construct filter choice
		for(int i = 0; i < CompressEnd; i++){
			if(Obj.Filter == ListAlgo[i].Filter){
				Obj = ListAlgo[i];
				Choice = (Compress)i;
				Strength = Level;
			}
		}
	}

}