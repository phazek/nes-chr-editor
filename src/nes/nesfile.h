#include <memory>

namespace nes {

class File {
public:
	static std::unique_ptr<File> LoadFromFile(const std::string& filePath);
	bool WriteToFile(const std::string& path);
private:
	File() = default;
};

} // namespace nes
