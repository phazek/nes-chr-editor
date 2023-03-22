#include <memory>
#include <span>
#include <vector>

namespace nes {

class File {
public:
	static std::unique_ptr<File> LoadFromFile(const std::string& filePath);
	bool WriteToFile(const std::string& filePath);

	uint8_t GetChrBankCount() const;
	[[nodiscard]] std::span<uint8_t> GetChrBank(uint8_t index);
private:
	std::vector<uint8_t> buffer_;
	size_t chrRomStart_ = 0;
	size_t chrRomSize_ = 0;


	File() = default;
	bool Init();
};

} // namespace nes
