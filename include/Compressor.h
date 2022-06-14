#pragma once
#include "Keyword.h"
#include "Algo.h"
#include "ErrorHandler.h"

namespace DataStream {

	class Compressor{

		Algo Obj;
		uint Level;

		public :

			Compressor() = default;

			Compressor(Compress Choice, int Strength){
				Initialize(Choice, Strength);
			}

			~Compressor() = default;

			void Initialize(Compress Choice, int Strength){
				try{																					  // Set algorithm
					if((Choice < 0) || (Choice >= Compress::CompressEnd))
						AlgorithmError(Obj.Name);
					Obj = ListAlgo[Choice];
				}
				catch(std::string Msg){
					Obj = ListAlgo[0];
					std::cout << Msg << "\nUsing default method : " << Obj.Name << "\n"; 
				}

				int n = (Strength - Obj.Min)/Obj.Step;					// Set compression level
				Level = Obj.Min + n*Obj.Step;										// Between Min and Max
				Level = (Level > Obj.Max) ? Obj.Max : Level;		// and multiple of Step 
			}

			void CompressData(hid_t DProp){
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

				if(!ChekAlgorithm(Obj.Filter)) CompressionError(Obj.Name);
			}

			bool ChekAlgorithm(H5Z_filter_t Id){
				uint Filter;
				if(H5Zfilter_avail(Id)){
					H5Zget_filter_info(Id, &Filter);

					if(Filter & H5Z_FILTER_CONFIG_ENCODE_ENABLED)
	 					if(Filter & H5Z_FILTER_CONFIG_DECODE_ENABLED)
	 						return true;
				}
				return false;
			}

	};

}