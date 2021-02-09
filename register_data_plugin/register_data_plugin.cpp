// windows
#include <wtypes.h>

// C++
#include <vector>
#include <iostream>
#include <array>
#include <cassert>
#include <filesystem>

namespace {

  std::wstring ReadRegistryEntry(const HKEY& hKey, const wchar_t* section, const wchar_t* entry)
  {
    assert(nullptr != section);
    assert(nullptr != entry);

    HKEY key;
    HRESULT rc = RegOpenKeyExW(hKey, section, 0, KEY_READ, &key);
    if (ERROR_SUCCESS != rc) {
      return std::wstring();
    }

    std::array<wchar_t, 2000> buffer;
    DWORD bufferSize = static_cast<DWORD>(buffer.size());
    rc = RegQueryValueExW(key, entry, nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer.data()), &bufferSize);
    buffer[buffer.size() - 1] = 0;

    RegCloseKey(key);

    if (ERROR_SUCCESS != rc) {
      return std::wstring();
    }

    return std::wstring(buffer.data());
  }

  DWORD CreateProcessAndWait(
    const wchar_t* commandLine,
    const wchar_t* workingDirectory = nullptr)
  {
    assert(nullptr != commandLine);

    PROCESS_INFORMATION pi;
    STARTUPINFOW si;
    DWORD exitCode = 0xFFFFFFFF;

    FillMemory(&si, sizeof(STARTUPINFO), 0);
    si.cb = sizeof(si);

    if (0 != CreateProcessW(nullptr, const_cast<wchar_t*>(commandLine), nullptr, nullptr, 0, NORMAL_PRIORITY_CLASS, nullptr, workingDirectory, &si, &pi)) {
      DWORD rc = WaitForSingleObject(pi.hProcess, INFINITE);
      GetExitCodeProcess(pi.hProcess, &exitCode);
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }
    return exitCode;
  }

  static std::wstring MakeFullPath(const wchar_t* path)
  {
    std::vector<wchar_t> buffer(1024);
    wchar_t* filePart = nullptr;
    size_t numReturned = GetFullPathNameW(path, static_cast<DWORD>(buffer.size()), buffer.data(), &filePart);
    if (0 == numReturned) {
      return std::wstring();
    }

    if (numReturned + 1 > buffer.size()) {
      buffer.resize(numReturned + 1);
      numReturned = GetFullPathNameW(path, static_cast<DWORD>(buffer.size()), buffer.data(), &filePart);
    }

    if ((0 == numReturned) || (numReturned + 1 > buffer.size())) {
      return std::wstring();
    }

    return std::wstring(&buffer[0]);
  }

}

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
  if (3 != argc) {
    std::wcout << L"USAGE: register_data_plugin.exe [-r|-u] FULL_URI_FILE_PATH\n"
      << L"  -r, --register   : register plugin\n"
      << L"  -u, --unregister : unregister plugin\n";
    return -1;
  }

  std::wstring usiRegOption;
  bool doUnregister{ false };
  if (0 == wcscmp(argv[1], L"-r") || 0 == wcscmp(argv[1], L"--register")) {
    usiRegOption = L"/o";
  }
  else if (0 == wcscmp(argv[1], L"-u") || 0 == wcscmp(argv[1], L"--unregister")) {
    usiRegOption = L"/u";
  }
  else {
    std::wcerr << L"ERROR: invalid option: " << argv[1] << L"\n";
    return -2;
  }

  const std::wstring uriFilePath(MakeFullPath(argv[2]));
  if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(uriFilePath.c_str())) {
    std::wcerr << L"ERROR: URI file does not exist! '" << uriFilePath.c_str() << L"'\n";
    return -3;
  }

  const std::wstring binDir(ReadRegistryEntry(HKEY_LOCAL_MACHINE, L"SOFTWARE\\National Instruments\\UsiCore", L"binDir"));
  if (binDir.empty()) {
    std::wcerr << L"ERROR: NI USI 19.0 or greater not installed\n";
    return -4;
  }

  const std::wstring usiRegPath(binDir + L"usiReg.exe");
  if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(usiRegPath.c_str())) {
    std::wcerr << L"ERROR: usiReg.exe does not exist in USI folder! '" << usiRegPath.c_str() << L"'\n";
    return -5;
  }

  // e.g. "C:\Program Files\National Instruments\Shared\USICore\Bin\usiReg.exe" /o "C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1\example1.uri"
  const std::wstring cmdLine(L"\"" + usiRegPath + L"\" " + usiRegOption + L" \"" + uriFilePath + L"\"");
  if (0 != CreateProcessAndWait(cmdLine.c_str())) {
    std::wcerr << L"ERROR: Registration failed! '" << uriFilePath.c_str() << L"'\n";
    return -6;
  }

  return 0;
}