#include "nesfile.h"

namespace nes {

static std::unique_ptr<File> NesFile::LoadFromFile(const std::string& filePath){
	return nullptr;
}

bool NesFile::WriteToFile(const std::string& path){
	return false;
}


} // namespace nes
