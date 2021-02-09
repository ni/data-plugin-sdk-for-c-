// C++
#include <iostream>
#include <memory>
#include <algorithm>

// windows
#include <wtypes.h>
#include <comutil.h>
#include <comdef.h>

// plugin sdk
#include <ni/dataplugin/IRoot.h>

#ifdef min
# undef min
#endif

namespace {

  class AutoLoadLibrary
  {
  public:
    AutoLoadLibrary(const wchar_t* dllPath) 
      : handle_(::LoadLibraryExW(dllPath, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH))
    {
    }

    ~AutoLoadLibrary()
    {
      if (nullptr != handle_) {
        ::FreeLibrary(handle_);
      }
    }

    const HINSTANCE& Handle() const
    {
      return handle_;
    }

    template <class T> bool GetProcAddress(const char* functionName, T& functionPointer)
    {
      functionPointer = nullptr;

      if (!handle_) {
        return false;
      }

      functionPointer = reinterpret_cast<T>(::GetProcAddress(handle_, functionName));
      if (!functionPointer) {
        return false;
      }

      return true;
    }

    AutoLoadLibrary& operator=(const AutoLoadLibrary& rhs) = delete;
    AutoLoadLibrary(const AutoLoadLibrary& rhs) = delete;

  private:

    HINSTANCE handle_;
  };

  template<class T> T Replace(
    const T& strIn,
    const T& searchString,
    const T& replaceString)
  {
    const T::size_type searchStringSize(searchString.size());
    if (0 == searchStringSize) {
      return strIn;
    }

    T str(strIn);
    const T::size_type replaceStringSize(replaceString.size());

    T::size_type n = 0;
    while (T::npos != (n = str.find(searchString, n))) {
      str.replace(n, searchStringSize, replaceString);
      n += replaceStringSize;
    }
    
    return str;
  }

  void CK_(const HRESULT& hr, const char* description = "failed")
  {
    if (FAILED(hr)) {
      throw std::runtime_error(description);
    }
  }

  auto GetGroup__(const ni::dataplugin::IRoot* root, ULONG index)
  {
    ni::dataplugin::IGroup* ptr = nullptr;
    CK_(const_cast<ni::dataplugin::IRoot*>(root)->GetGroup(index, ptr), "retrieving group failed");

    auto del = [](ni::dataplugin::IGroup* p) { if (nullptr != p) { p->Release(); }};
    std::unique_ptr<ni::dataplugin::IGroup, decltype(del)> rv(ptr, del);
    return rv;
  }

  auto GetChannel__(const ni::dataplugin::IGroup* group, ULONG index)
  {
    ni::dataplugin::IChannel* ptr = nullptr;
    CK_(group->GetChannel(index, ptr), "retrieve channel failed");

    auto del = [](ni::dataplugin::IChannel* p) { if (nullptr != p) { p->Release(); }};
    std::unique_ptr<ni::dataplugin::IChannel, decltype(del)> rv(ptr, del);
    return rv;
  }

  void AccessCustomProperties__(const ni::dataplugin::IProperties* properties)
  {
    ULONG propCount{ 0 };
    CK_(properties->CountCustomProperties(propCount));
    for (ULONG propIndex = 0; propIndex < propCount; ++propIndex) {
      _bstr_t name;
      _variant_t value;
      CK_(properties->GetCustomProperty(propIndex, name.GetBSTR(), value.GetVARIANT()));
    }
  }

  int read_tree(int argc, wchar_t *argv[])
  {
    if (4 != argc) {
      std::wcout << L"USAGE: simple_read_test.exe --read_tree DLLPATH EXAMPLEFILE\n";
      return -201;
    }

    const std::wstring pluginDllPath(argv[2]);
    const std::wstring exampleFilePath(argv[3]);
    if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(pluginDllPath.c_str())) {
      std::wcerr << L"ERROR: dll does not exist. '" << pluginDllPath.c_str() << L"'\n";
      return -202;
    }

    if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(exampleFilePath.c_str())) {
      std::wcerr << L"ERROR: file does not exist. '" << exampleFilePath.c_str() << L"'\n";
      return -203;
    }

    AutoLoadLibrary pluginHandle(pluginDllPath.c_str());
    if (nullptr == pluginHandle.Handle()) {
      std::wcerr << L"ERROR: Unable to load library into process.\n";
      return -204;
    }

    using tUSP_TDM_MARSHALLER = HRESULT(__cdecl*)(REFIID queriedIID, void** retval);
    tUSP_TDM_MARSHALLER USP_TDM_MARSHALLER;
    if (!pluginHandle.GetProcAddress("USP_TDM_MARSHALLER", USP_TDM_MARSHALLER)) {
      std::wcerr << L"ERROR: Entry point 'USP_TDM_MARSHALLER' could not be found.\n";
      return -205;
    }

    void* dummy = nullptr;
    USP_TDM_MARSHALLER(ni::dataplugin::IID_IRoot, &dummy);
    if (nullptr == dummy) {
      std::wcerr << L"ERROR: Plugin dll did not return read file interface\n";
      return -206;
    }

    auto refRelease = [&](ni::dataplugin::IRoot* ptr) {if (ptr) ptr->Release(); };
    std::unique_ptr<ni::dataplugin::IRoot, decltype(refRelease)> root(static_cast<ni::dataplugin::IRoot*>(dummy), refRelease);

    std::wstring paramString(
      LR"(<filename readonly="yes">)" +
      Replace<std::wstring>(exampleFilePath, L"&", L"&amp;") +
      LR"(</filename>)");

    CK_(root->SetParameter(_bstr_t(paramString.c_str()).GetBSTR()));
    CK_(root->Initialize(_bstr_t(exampleFilePath.c_str()).GetBSTR()));

    _bstr_t rootName;
    CK_(root->GetName(rootName.GetBSTR()));

    _bstr_t rootDescription;
    CK_(root->GetDescription(rootDescription.GetBSTR()));

    _bstr_t rootTitle;
    CK_(root->GetTitle(rootTitle.GetBSTR()));

    _bstr_t rootAuthor;
    CK_(root->GetAuthor(rootAuthor.GetBSTR()));

    DATE rootDate{ 0.0 };
    CK_(root->GetDate(rootDate));

    AccessCustomProperties__(root.get());

    ULONG groupCount(0);
    CK_(root->GetNumberOfGroups(groupCount), "unable to retrieve group count");
    for (ULONG groupIndex = 0; groupIndex < groupCount; ++groupIndex) {
      auto group = GetGroup__(root.get(), groupIndex);

      _bstr_t groupName;
      CK_(group->GetName(groupName.GetBSTR()));
      std::wcout << (const wchar_t*)groupName << L"\n";

      _bstr_t groupDescription;
      CK_(group->GetDescription(groupDescription.GetBSTR()));

      AccessCustomProperties__(group.get());

      ULONG channelCount(0);
      CK_(group->GetNumberOfChannels(channelCount), "unable to retrieve channel count");
      for (ULONG channelIndex = 0; channelIndex < channelCount; ++channelIndex) {
        auto channel = GetChannel__(group.get(), channelIndex);

        _bstr_t channelName;
        CK_(channel->GetName(channelName.GetBSTR()));
        std::wcout << L"  " << (const wchar_t*)channelName << L":";

        VARTYPE channelDataType{ VT_EMPTY };
        CK_(channel->GetDataType(channelDataType));

        _bstr_t channelUnit;
        CK_(channel->GetUnit(channelUnit.GetBSTR()));

        _bstr_t channelDescription;
        CK_(channel->GetDescription(channelDescription.GetBSTR()));

        ULONG64 channelLength{ 0 };
        CK_(channel->GetLength(channelLength));
        std::wcout << L" length=" << channelLength;
        std::wcout << L"\n";

        ULONG isImplicit{ 0 };
        CK_(channel->IsImplicit(isImplicit));

        ULONG channelIsRawLiner{ 0 };
        DOUBLE channelOffset{ 0.0 };
        DOUBLE channelFactor{ 0.0 };
        CK_(channel->IsRawLinear(channelIsRawLiner, channelOffset, channelFactor));

        DOUBLE channelMinimum{ 0.0 };
        CK_(channel->GetMinimum(channelMinimum));

        DOUBLE channelMaximum{ 0.0 };
        CK_(channel->GetMaximum(channelMaximum));

        AccessCustomProperties__(channel.get());

        // accessing at 0 must always work
        _variant_t firstChunk;
        CK_(channel->GetValues(0ULL, 3ULL, firstChunk.GetVARIANT()));

        if (channelLength > 0 && 0 == isImplicit) {
          // last values
          const ULONG lastChunkSize {static_cast<ULONG>(std::min(channelLength, 3ULL))};
          const ULONG64 lastChunkStartIndex{ channelLength - lastChunkSize };
          _variant_t lastChunk;
          CK_(channel->GetValues(lastChunkStartIndex, lastChunkSize, lastChunk.GetVARIANT()));
          
          // try to get more than available
          _variant_t lastChunkMoreThanAvailable;
          CK_(channel->GetValues(lastChunkStartIndex, lastChunkSize + 3, lastChunkMoreThanAvailable.GetVARIANT()));

          // accessing after last returns empty variant
          _variant_t shouldBeEmpty;
          CK_(channel->GetValues(channelLength, 5UL, shouldBeEmpty.GetVARIANT()));

          _variant_t shouldFail;
          if (!FAILED(channel->GetValues(channelLength + 1, 5UL, shouldBeEmpty.GetVARIANT()))) {
            throw std::runtime_error("accessing at invalid index should fail");
          }
        }
      }
    }


    return 0;
  }
}

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
  if (argc < 2) {
    std::wcout << L"USAGE: simple_read_test.exe --TEST OPTIONS\n"
      << L"  --read_tree DLLPATH EXAMPLEFILE\n";
    return -1;
  }

  try {

    if (0 == wcscmp(L"--read_tree", argv[1])) {
      return read_tree(argc, argv);
    }
    else {
      std::wcerr << "ERROR: unknown test: '" << argv[1] << "'\n";
      return -100;
    }
  }
  catch (const std::exception& ex) {
    std::cerr << "ERROR: std::exception: " << ex.what() << "\n";
    return -101;
  }
  catch (...) {
    std::cerr << "ERROR: Unknown exception showed up\n";
    return -102;
  }
  return 0;
}