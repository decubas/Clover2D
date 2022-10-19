#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include <Clover.h>
#include <imgui.h>

	/////////////////////////////////////////////////// Only with C++ 17
	class FileSystem
	{
	public:

		FileSystem();
		~FileSystem();


		void Initialize(const CPath& root);

		static CString toString(CPath& path)
		{
			return path.string();
		}

		bool existExtension(const CString& extension) { return files_.find(extension) != files_.end(); }
		CVector<CPath>& getDirectories() { return directories_; }
		CVector<CPath>& getFiles(const CString& extension);

		static bool OpenWindowsBrowser(CString& out);

		void ImGuiShow();
	protected:
		bool ShowDirectory(const CPath& dir);
		void ShowContent(const CPath& dir);
		CPath selected = "";
		CVector<CPath> directories_;
		CMap<CString, CVector<CPath>> files_;
		CPath root_;
		CPath current_;
	};

#endif