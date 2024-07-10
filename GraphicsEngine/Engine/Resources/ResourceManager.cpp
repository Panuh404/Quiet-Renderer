#include "ResourceManager.h"
#include <filesystem>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourcePtr ResourceManager::CreateResourceFromFile(const wchar_t* file_path)
{
	std::wstring full_path = std::filesystem::absolute(file_path);

	auto it = m_Resources.find(full_path);
	if (it != m_Resources.end())
	{
		return it->second;
	}

	Resource* RawResource = this->CreateResourceFromFileConcrete(full_path.c_str());
	if (RawResource)
	{
		ResourcePtr res(RawResource);
		m_Resources[full_path] = res;
		return res;
	}

	return nullptr;
}
