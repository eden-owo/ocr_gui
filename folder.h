#ifndef FOLDER_H
#define FOLDER_H

#pragma once

#include <string>
#include <vector>

std::wstring ReadCachedPath(const std::wstring& cacheFile);
void WriteCachedPath(const std::wstring& cacheFile, const std::wstring& path);
std::wstring SelectFolder(const std::wstring& title, const std::wstring& defaultPath = L"");
std::string  WStringToString(const std::wstring& wstr);
std::string  ReadFolder();
std::vector<std::string> ListFilesInFolder(const std::string& folderPath);

#endif // FOLDER_H
