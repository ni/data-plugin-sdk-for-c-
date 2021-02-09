#pragma once

// base class
#include <ni/dataplugin/IRootExport.h>

// C++
#include <atomic>

class IRootExportImpl : public ni::dataplugin::IRootExport
{
private:
  // private destructor. Only destructed by Release
  virtual ~IRootExportImpl() {};

public:
  HRESULT Execute(
    const BSTR& paramString,
    const BSTR& initParamString,
    const ni::dataplugin::IRoot* root,
    BSTR& errorMsg) override;

public:
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override
  {
    *ppvObject = nullptr;

    if (ni::dataplugin::IID_IRootExport == riid) {
      *ppvObject = static_cast<ni::dataplugin::IRootExport*>(this);
      AddRef();
    }

    if (nullptr == *ppvObject) {
      return E_NOINTERFACE;
    }

    return S_OK;
  }

  ULONG STDMETHODCALLTYPE AddRef() override
  {
    return ++refCounter_;
  }

  ULONG STDMETHODCALLTYPE Release() override
  {
    const ULONG rv(--refCounter_);
    if (0 == rv) {
      delete this;
    }
    return rv;
  }

private:
  std::atomic<ULONG> refCounter_{1};
};
