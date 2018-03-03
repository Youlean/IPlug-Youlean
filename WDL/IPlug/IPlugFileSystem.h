#pragma once

#include "IGraphics.h"

#include <string>
#include <vector>
#include <fstream>
#include <sys/stat.h>

#include "UTF_Helper.h"

using namespace std;

class IPlugFileSystem : public Windows_UTF_Converter
{
public:

#ifdef _WIN32
	void GetAllFilesInDirRelative(const string dir, vector<string> *files, bool look_in_subfolders = true, bool return_full_paths = false)
	{
		string currentDir = GetCurrentDir();

		string path = currentDir;
		path.append("\\");
		path.append(dir);

		GetAllFilesInDir(path, files, look_in_subfolders, return_full_paths);
	}

	void GetAllFilesInDir(const string dir, vector<string> *files, bool look_in_subfolders = true, bool return_full_paths = false)
	{
		GetAllFilesInDir(utf8_to_utf16(dir).c_str(), files, look_in_subfolders, return_full_paths);
	}

	void GetAllFilesInDir(const wstring dir, vector<string> *files, bool look_in_subfolders = true, bool return_full_paths = false)
	{
		wstring pattern(dir);
		pattern.append(L"\\*");

		WIN32_FIND_DATAW data;

		HANDLE hFind;
		if ((hFind = FindFirstFileW(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
		{
			do
			{
				wstring fileName = data.cFileName;

				if (fileName != L"." && fileName != L"..")
				{
					// If this is file path
					if (fileName.find('.') != wstring::npos)
					{
						files->push_back(utf16_to_utf8(dir + wstring(L"\\") + data.cFileName).c_str());
					}
					// If this is folder path
					else if (look_in_subfolders)
					{
						GetAllFilesInDir(dir + wstring(L"\\") + data.cFileName, files, look_in_subfolders, true);
					}
				}
			}
			while (FindNextFileW(hFind, &data) != 0);

			FindClose(hFind);
		}

		if (!return_full_paths)
		{
			string currentDir = utf16_to_utf8(dir + wstring(L"\\"));

			// Strip current directory path
			for (size_t i = 0; i < files->size(); i++)
			{
				(*files)[i] = (*files)[i].substr(currentDir.size(), files->size() - currentDir.size());
			}
		}
	}

	void GetAllFoldersInDirRelative(const string dir, vector<string> *files, bool look_in_subfolders = true, bool return_full_paths = false)
	{
		string currentDir = GetCurrentDir();

		string path = currentDir;
		path.append("\\");
		path.append(dir);

		GetAllFoldersInDir(path, files, look_in_subfolders, return_full_paths);
	}

	void GetAllFoldersInDir(const string dir, vector<string> *files, bool look_in_subfolders = true, bool return_full_paths = false)
	{
		GetAllFoldersInDir(utf8_to_utf16(dir).c_str(), files, look_in_subfolders, return_full_paths);
	}

	void GetAllFoldersInDir(const wstring dir, vector<string> *files, bool look_in_subfolders = true, bool return_full_paths = false)
	{
		wstring pattern(dir);
		pattern.append(L"\\*");

		WIN32_FIND_DATAW data;

		HANDLE hFind;
		if ((hFind = FindFirstFileW(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
		{
			do
			{
				wstring fileName = data.cFileName;

				if (fileName != L"." && fileName != L"..")
				{
					// If this is file path
					if (fileName.find('.') == wstring::npos)
					{
						files->push_back(utf16_to_utf8(dir + wstring(L"\\") + data.cFileName).c_str());
						
						if (look_in_subfolders)
						{
							GetAllFoldersInDir(dir + wstring(L"\\") + data.cFileName, files, look_in_subfolders, true);
						}
					}
				}
			} while (FindNextFileW(hFind, &data) != 0);

			FindClose(hFind);
		}

		if (!return_full_paths)
		{
			string currentDir = utf16_to_utf8(dir + wstring(L"\\"));

			// Strip current directory path
			for (size_t i = 0; i < files->size(); i++)
			{
				(*files)[i] = (*files)[i].substr(currentDir.size(), files->size() - currentDir.size());
			}
		}
	}

	bool FileCopyRelative(string src_path, string dst_path, bool overwrite_file = true)
	{
		string currentDir = GetCurrentDir();

		string path1 = currentDir;
		path1.append("\\");
		path1.append(src_path);

		string path2 = currentDir;
		path2.append("\\");
		path2.append(dst_path);

		return FileCopy(path1, path2, overwrite_file);
	}
	
	bool FileCopy(string src_path, string dst_path, bool overwrite_file = true, bool create_dirs = true)
	{
		if (create_dirs)
		{
			wstring destDir = utf8_to_utf16(dst_path);

			vector <wstring> folders;
			wstring::size_type pos;
			
			while (true)
			{
				pos = destDir.find_last_of(L"\\/");
				destDir = destDir.substr(0, pos);

				if (!DirExists(utf16_to_utf8(destDir)) && destDir.size() > 0)
					folders.push_back(destDir);
				else break;
			}

			for (int i = folders.size() - 1; i >= 0; i--)
			{
				CreateDirectoryW(folders[i].c_str(), NULL);
			}
			
			return CopyFileW(utf8_to_utf16(src_path).c_str(), utf8_to_utf16(dst_path).c_str(), !overwrite_file);
		}
		else
		{
			return CopyFileW(utf8_to_utf16(src_path).c_str(), utf8_to_utf16(dst_path).c_str(), !overwrite_file);
		}
	}

	bool DirCreateRelative(string dir_path)
	{
		string currentDir = GetCurrentDir();

		string path = currentDir;
		path.append("\\");
		path.append(dir_path);

		return DirCreate(path);
	}

	bool DirCreate(string dir_path)
	{
		return CreateDirectoryW(utf8_to_utf16(dir_path).c_str(), NULL);
	}

	bool FileDeleteRelative(string file_path)
	{
		string currentDir = GetCurrentDir();

		string path = currentDir;
		path.append("\\");
		path.append(file_path);

		return FileDelete(path);
	}

	bool FileDelete(string file_path)
	{
		return DeleteFileW(utf8_to_utf16(file_path).c_str());
	}

	bool DirDeleteRelative(string dir_path)
	{
		string currentDir = GetCurrentDir();

		string path = currentDir;
		path.append("\\");
		path.append(dir_path);

		return DirDelete(path);
	}

	bool DirDelete(string dir_path)
	{
		return RemoveDirectoryW(utf8_to_utf16(dir_path).c_str());
	}

	bool DirDeleteEverythingRelative(string dir_path)
	{
		string currentDir = GetCurrentDir();

		string path = currentDir;
		path.append("\\");
		path.append(dir_path);

		return DirDeleteEverything(dir_path);
	}

	bool DirDeleteEverything(string dir_path)
	{
		bool succeeded = true;

		vector <string> listOfFiles;
		GetAllFilesInDirRelative(dir_path, &listOfFiles, true, true);

		vector <string> listOfFolders;
		GetAllFoldersInDirRelative(dir_path, &listOfFolders, true, true);

		for (int i = 0; i < listOfFiles.size(); i++)
		{
			if (!FileDelete(listOfFiles[i])) succeeded = false;
		}

		for (int i = listOfFolders.size() - 1; i >= 0; i--)
		{
			if (!DirDelete(listOfFolders[i])) succeeded = false;
		}

		if (!DirDeleteRelative(dir_path)) succeeded = false;

		return succeeded;
	}

	string GetCurrentDir() 
	{
		WCHAR buffer[MAX_PATH];
		GetModuleFileNameW(NULL, buffer, MAX_PATH);
		string::size_type pos = wstring(buffer).find_last_of(L"\\/");
		return utf16_to_utf8(wstring(buffer).substr(0, pos));
	}

	bool DirExistRelative(string dir_path)
	{
		string currentDir = GetCurrentDir();

		string path = currentDir;
		path.append("\\");
		path.append(dir_path);

		return DirExists(path);
	}

	bool DirExists(string dir_path)
	{
		return DirExists(utf8_to_utf16(dir_path));
	}

	bool DirExists(wstring dir_path)
	{
		//return PathFileExistsW(dir_path.c_str());

		DWORD ftyp = GetFileAttributesW(dir_path.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;  //something is wrong with your path!

		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;   // this is a directory!

		return false;    // this is not a directory!
	}

	bool FileExists(string file_path)
	{
		return FileExists(utf8_to_utf16(file_path));
	}

	bool FileExists(wstring file_path)
	{
		//return PathFileExistsW(file_path.c_str());

		GetFileAttributesW(file_path.c_str());
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(file_path.c_str()) && GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			return false;
		}
		return true;
	}

	bool FileIsInUse(string file_path)
	{
		return FileIsInUse(utf8_to_utf16(file_path));

	}

	bool FileIsInUse(wstring file_path)
	{
		if (!FileExists(file_path)) return false;

		HANDLE hFile = CreateFileW(file_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return true;
		CloseHandle(hFile);
		return false;
	}

	bool FileRename(string src_path, string dst_path)
	{
		return FileRename(utf8_to_utf16(src_path), utf8_to_utf16(dst_path));
	}

	bool FileRename(wstring src_path, wstring dst_path)
	{
		 return MoveFileW(src_path.c_str(), dst_path.c_str());
	}

#else 

	bool FileCopy(const char* src_path, const char* dst_path)
	{
		utf8::ifstream src(src_path, ios::binary);
		utf8::ofstream dest(dst_path, ios::binary);
		dest << src.rdbuf();
		return src && dest;
	}

	bool FileExist(const char* path) 
	{
		utf8::ifstream file(path);
		return file.good();
	}

	bool FileDelete(const char* path)
	{
		int nError = 0;
#if defined(_WIN32)
		nError = _wremove(utf8_to_utf16(path).c_str());
#else 
		nError = remove(path);
#endif

		if (nError == 0) return true;

		return false;
	}

	bool DirExist(const char* path)
	{
#if defined(_WIN32)
		struct _stat st;
		_wstat(utf8_to_utf16(path).c_str(), &st);
		return st.st_mode & S_IFDIR;
#else 
		struct stat st;
		stat(path, &st);
		return st.st_mode & S_IFDIR;
#endif
	}

	bool DirCreate(const char* path)
	{
		int nError = 0;
#if defined(_WIN32)
		nError = _wmkdir(utf8_to_utf16(path).c_str());
#else 
		mode_t nMode = 0733; // UNIX style permissions
		nError = mkdir(path, nMode);
#endif

		if (nError == 0) return true;

		return false;
	}

	bool DirDelete(const char* path)
	{
		int nError = 0;
#if defined(_WIN32)
		nError = _wrmdir(utf8_to_utf16(path).c_str());
#else 
		nError = rmdir(path);
#endif

		if (nError == 0) return true;

		return false;
	}
#endif
private:

};