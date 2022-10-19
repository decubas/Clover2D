#include "Clover.h"

#include <Tools/FileSystem.h>

#include <shobjidl.h>

	FileSystem::FileSystem()
	{
	}

	FileSystem::~FileSystem()
	{

	}

	void FileSystem::Initialize(const CPath& root)
	{
		root_ = root;		
		selected = root;

		directories_.push_back(root_);
		for (auto& dir : std::filesystem::recursive_directory_iterator(root_))
		{
			
			if (!dir.exists()) continue;

			if (dir.is_directory())
			{
				directories_.push_back(dir.path());
			}

			if (dir.is_regular_file())
			{
				if (!existExtension(dir.path().extension().generic_string()))
				{
					files_[dir.path().extension().generic_string()] = CVector<CPath>();
				}
				files_[dir.path().extension().generic_string()].push_back(dir.path());
			}
		}

	}

	CVector<CPath>& FileSystem::getFiles(const CString& extension)
	{
		return files_[extension];		
	}

	bool FileSystem::ShowDirectory(const CPath& dir)
	{

		if (!dir.has_extension() && ImGui::Button(dir.filename().generic_string().c_str()))
		{
			for (auto& rdir : std::filesystem::directory_iterator(dir))
			{
				ShowDirectory(rdir.path());
				selected = dir;
			}
		}

		if (dir.has_extension())
		{
			if (ImGui::Button(dir.filename().generic_string().c_str()))
			{
				std::string extension = dir.extension().generic_string();
			}
			return false;
		}
		ImGui::SameLine();



		return false;
	}


	void FileSystem::ShowContent(const CPath& dir)
	{
		for (auto& rdir : std::filesystem::directory_iterator(dir))
		{
			ShowDirectory(rdir.path());
		}
	}

	void FileSystem::ImGuiShow()
	{
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 75);
		ShowDirectory(root_);
		ImGui::NextColumn();
		ShowContent(selected);
	}


	

	bool FileSystem::OpenWindowsBrowser(CString& out)
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
			COINIT_DISABLE_OLE1DDE);
		IFileOpenDialog* pFileOpen;
		if (SUCCEEDED(hr))
		{

			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
		}
		hr = pFileOpen->Show(NULL);

		// Get the file name from the dialog box.
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				LPWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				// Display the file name to the user.
				if (SUCCEEDED(hr))
				{
					std::wstring p(pszFilePath);
					out = std::string(p.begin(), p.end());
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
				return true;
			}
		}

		return false;

	}
