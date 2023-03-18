#include "nesfile.h"

#include "nes/constants.h"

#include "tfm/tinyformat.h"

#include <array>
#include <fstream>

namespace {
constexpr size_t kHeaderSize = 16;
constexpr std::array<uint8_t, 4> kMagicNumber{'N', 'E', 'S', 0x1A};
} // namespace

namespace nes {

 std::unique_ptr<File> File::LoadFromFile(const std::string& filePath){
	std::ifstream input{filePath, std::ios::binary};

	if (!input.is_open()) {
		tfm::printf("ERROR: failed to open rom file: %s\n", filePath);
		return nullptr;
	}

	auto file = std::unique_ptr<File>(new File{});

	// Check file size
	auto fsize = input.tellg();
	input.seekg( 0, std::ios::end );
	fsize = input.tellg() - fsize;
	auto bufferSize = static_cast<size_t>(fsize);
	input.seekg( 0, std::ios::beg);

	if (bufferSize < kHeaderSize) {
		tfm::printf("ERROR: invalid rom file size: %d bytes\n", bufferSize);
		return nullptr;
	}

	file->buffer_.reserve(fsize);
	if (!input.read(reinterpret_cast<char*>(file->buffer_.data()), bufferSize)) {
		tfm::printf("ERROR: unable to read entire file (%d/%d read)\n", input.gcount(), fsize);
		return nullptr;
	}

	return file;
}

bool File::WriteToFile(const std::string& filePath){
	std::ofstream output{filePath, std::ios::binary};
	output.write(reinterpret_cast<char*>(buffer_.data()), buffer_.size());
	output.close();

	return true;
}

uint8_t File::GetChrBankCount() const {
	return chrRomSize_ / kChrBankSize;
}

std::span<uint8_t> File::GetChrBank(uint8_t index) {
	auto startAddress = chrRomStart_ + index * kChrBankSize;
	if (startAddress <= buffer_.size()) {
		return {buffer_.data() + startAddress, kChrBankSize};
	}

	tfm::printf("ERROR: chr bank index out of bounds (%d)\n", index);
	return {};
}

bool File::Init() {
	// Check magic number
	if (memcmp(buffer_.data(), kMagicNumber.data(), kMagicNumber.size()) != 0) {
		tfm::printf("ERROR: magic number not found\n");
		return false;
	}

	const auto prgRomSize = buffer_[4] * kPrgBankSize;
	const auto prgRomStart = kHeaderSize;
	chrRomSize_ = buffer_[5] * kChrBankSize;
	chrRomStart_ = prgRomStart + prgRomSize;

	return true;
}

} // namespace nes
