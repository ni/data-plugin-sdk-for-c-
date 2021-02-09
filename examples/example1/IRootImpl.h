#pragma once

// base class
#include <ni/dataplugin/IRoot.h>

// C++
#include <vector>

class IGroupImpl;

class IRootImpl : public ni::dataplugin::IRoot
{
public:
  IRootImpl(); 
private:
  // only deleted by Release
  virtual ~IRootImpl();

public:
  HRESULT Initialize(const BSTR& filename) override;
  HRESULT GetNumberOfGroups(ULONG& count) const override;
  HRESULT GetGroup(ULONG index, ni::dataplugin::IGroup*& group) override;

  HRESULT GetName(BSTR& name) const override;
  HRESULT GetDescription(BSTR& description) const override;
  HRESULT GetAuthor(BSTR& author) const override;
  HRESULT GetTitle(BSTR& title) const override;
  HRESULT GetDate(DATE& date) const override;
  HRESULT CountCustomProperties(ULONG& count) const override;
  HRESULT GetCustomProperty(ULONG index, BSTR& name, VARIANT& value) const override;

private:
  IRootImpl& operator=(const IRootImpl& rhs) = delete;
  IRootImpl(const IRootImpl& rhs) = delete;

public:

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID queriedIID, void** retval) override
  {
    *retval = 0;
    if(queriedIID == ni::dataplugin::IID_IRoot) { *retval = static_cast<IRootImpl*>(this); }
    else if(queriedIID == ni::dataplugin::IID_IProperties) { *retval = static_cast<IProperties*>(this); }
    else if(queriedIID == IID_IUnknown) { *retval = static_cast<IUnknown*>(this); }
    if(*retval == 0) { return E_NOINTERFACE; }
    AddRef();
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
  unsigned int refCounter_{ 1 };

private:

  std::vector<IGroupImpl*> children_;
};