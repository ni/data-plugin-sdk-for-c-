#pragma once

// base class
#include <ni/dataplugin/IGroup.h>

// C++
#include <vector>

class IChannelImpl;

class IGroupImpl : public ni::dataplugin::IGroup
{
public:
  IGroupImpl(); 
  virtual ~IGroupImpl();

  HRESULT GetNumberOfChannels(ULONG& count) const override;
  HRESULT GetChannel(ULONG index, ni::dataplugin::IChannel*& channel) const override;

  HRESULT GetName(BSTR& name) const override;
  HRESULT GetDescription(BSTR& description) const override;
  HRESULT CountCustomProperties(ULONG& count) const override;
  HRESULT GetCustomProperty( ULONG index, BSTR& name, VARIANT& value) const override;

private:
  IGroupImpl& operator=(const IGroupImpl& rhs) = delete;
  IGroupImpl(const IGroupImpl& rhs) = delete;

public:

  HRESULT STDMETHODCALLTYPE QueryInterface (REFIID queriedIID, void** retval) override
  {
    *retval = 0;
    if(queriedIID == ni::dataplugin::IID_IGroup) { *retval = static_cast<IGroupImpl*>(this); }
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
    if(0 == rv) { 
      delete this; 
    }
    return rv;
  }
private:
  unsigned int refCounter_{ 1 };

private:

  std::vector<IChannelImpl*> children_;
};