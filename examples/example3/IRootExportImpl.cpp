// own header
#include "IRootExportImpl.h"

#include <ni/dataplugin/IRoot.h>
#include <ni/dataplugin/IGroup.h>
#include <ni/dataplugin/IChannel.h>
#include <ni/dataplugin/ParamString.h>

// C++
#include <memory>
#include <fstream>

// COM helper
#include <comutil.h>
#include <comdef.h>

namespace ni { namespace dataplugin { namespace factory {

  // we have to implement the root factory which is used by ni/dataplugin/main.hpp
  ni::dataplugin::IRootExport* __cdecl IRootExportFactory()
  {
    return new (std::nothrow) IRootExportImpl();
  }

}}}

/////////////////////////////////
// Implement EXPORT

namespace {
  void CK_(const HRESULT& hr, const char* description = "failed")
  {
    if (FAILED(hr)) {
      throw std::exception(description);
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

}

HRESULT IRootExportImpl::Execute(
  const BSTR& paramString,
  const BSTR& initParamString,
  const ni::dataplugin::IRoot* root,
  BSTR& errorMsg)
{
  try {

    ni::dataplugin::str::Param param(paramString);
    const std::wstring targetFileName = ni::dataplugin::str::Unescape(param.GetTagValue(L"filename"));

    std::wofstream file(targetFileName);

    ULONG groupCount(0);
    CK_(root->GetNumberOfGroups(groupCount), "unable to retrieve group count");
    for (ULONG groupIndex = 0; groupIndex < groupCount; ++groupIndex) {
      auto group = GetGroup__(root, groupIndex);
      
      _bstr_t groupName;
      CK_(group->GetName(groupName.GetBSTR()));
      file << (const wchar_t*)groupName << L"\n";

      ULONG channelCount(0);
      CK_(group->GetNumberOfChannels(channelCount), "unable to retrieve channel count");
      for (ULONG channelIndex = 0; channelIndex < channelCount; ++channelIndex) {
        auto channel = GetChannel__(group.get(), channelIndex);

        _bstr_t channelName;
        CK_(channel->GetName(channelName.GetBSTR()));
        file << L"  " << (const wchar_t*)channelName << L":";
        
        ULONG64 channelLength{ 0 };
        CK_(channel->GetLength(channelLength));
        file << L" length=" << channelLength;

        file << L"\n";
      }
    }

    return S_OK;
  }
  catch (const std::exception& ex) {
    _bstr_t msg(ex.what());
    errorMsg = msg.Detach();
    return E_FAIL;
  }
  catch (...) {
    // make sure no exception is thrown
    return E_FAIL;
  }
}
