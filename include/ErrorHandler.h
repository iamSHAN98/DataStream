#pragma once
#include <string>
#include <stdexcept>

namespace DataStream{

	static void CompoundTypeError(){
		throw std::logic_error("Can't insert member to non-compound data-type");
	}

	static void DataTypeError(){
		throw std::domain_error("Can't recognize given data-type");
	}

	static void PathError(std::string Path){
		throw std::runtime_error(Path + " already exists");
	}

	static void NoPathError(std::string Path){
		throw std::runtime_error("Can't locate " + Path);
	}

	static void AttributeError(std::string Name, std::string Path){
		throw std::runtime_error("Attribute " + Name + " already exists at " + Path);
	}

	static void NoAttributeError(std::string Name, std::string Path){
		throw std::runtime_error("Can't locate attribute " + Name + " at " + Path);
	}

	static void LinkCreationError(std::string Path){
		throw std::runtime_error("Final/intermediate link(s) in " + Path +
														 " may correspond to existing dataset(s)");
	}

	static void DataSetError(std::string Path){
		throw std::runtime_error(Path + " happens to be a group");
	}

	static std::string AlgorithmError(std::string Name){
		throw "Compression algorithm " + Name + " is not registered";
	}

	static std::string CompressionError(std::string Name){
		throw std::runtime_error("Compression algorithm " + Name + " can't be used");
	}

}
