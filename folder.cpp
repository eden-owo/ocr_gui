#include "Folder.h"
#include <shlobj.h>
#include <fstream>
#include <iostream>  // std::wcerr 必須要這個
#include <vector>

#ifdef _WIN32
#include <direct.h>   // _mkdir
#define mkdir(x, y) _mkdir(x)
#endif

std::wstring ReadCachedPath(const std::wstring& cacheFile) {
    std::wifstream infile(cacheFile);
    std::wstring path;
    if (infile.is_open()) {
        std::getline(infile, path);
    }
    return path;
}

void WriteCachedPath(const std::wstring& cacheFile, const std::wstring& path) {
    std::wofstream outfile(cacheFile);
    if (outfile.is_open()) {
        outfile << path;
    }
}

std::wstring SelectFolder(const std::wstring& title, const std::wstring& defaultPath) {
    BROWSEINFOW bi = { 0 };
    bi.lpszTitle = title.c_str();
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    if (!defaultPath.empty()) {
        static wchar_t buffer[MAX_PATH];
        wcsncpy_s(buffer, defaultPath.c_str(), MAX_PATH);
        bi.lpfn = [](HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) -> int {
            if (uMsg == BFFM_INITIALIZED) {
                SendMessageW(hwnd, BFFM_SETSELECTION, TRUE, lpData);
            }
            return 0;
        };
        bi.lParam = (LPARAM)buffer;
    }

    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    if (pidl != nullptr) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, path)) {
            CoTaskMemFree(pidl);
            return std::wstring(path);
        }
        CoTaskMemFree(pidl);
    }
    return L"";
}

std::string WStringToString(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
    // 移除最後的 null terminator（因為 std::string 已經自動管理）
    if (!str.empty() && str.back() == '\0') {
        str.pop_back();
    }
    return str;
}

std::string ReadFolder() {
    CoInitialize(nullptr);  // 初始化 COM（使用 SelectFolder）

    // 選擇影像資料夾
    std::wstring cacheFile = L"folder_cache.txt";
    std::wstring cachedPath = ReadCachedPath(cacheFile);
    std::wstring selectedFolder = SelectFolder(L"Please select folder.", cachedPath);
    WriteCachedPath(cachedPath, selectedFolder);

    if (selectedFolder.empty()) {
        std::wcerr << L"未選擇資料夾，結束程式。\n";
        CoUninitialize();
        return "";
    }
    WriteCachedPath(cacheFile, selectedFolder);
    // 清理 COM
    CoUninitialize();

    return WStringToString(selectedFolder);
}

std::vector<std::string> ListFilesInFolder(const std::string& folderPath) {
    std::vector<std::string> files;
    std::string searchPath = folderPath + "\\*";

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open folder: " << folderPath << std::endl;
        return files;
    }

    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            files.push_back(folderPath + "\\" + findData.cFileName);
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
    return files;
}
