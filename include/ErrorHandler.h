/*
		Set of error messages for handling HDF5 objects
		e.g. path (links), attributes, datasets etc.
*/

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
		throw std::runtime_error(Path + " creation failed");
	}

	static void LinkAccessError(std::string Path){
		throw std::runtime_error(Path + " access failed");
	}

	static std::string CompressionError(std::string Name){
		throw std::domain_error("Compression method (name/id) " + Name + " is not available");
	}

}
