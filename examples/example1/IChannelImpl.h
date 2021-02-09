#pragma once

// base class
#include <ni/dataplugin/IChannel.h>

// c++
#include <string>
#include <vector>

typedef enum {
  eExplicitChn = 0,
  eImplicitChn = 1,
  eRawChn = 2
} tChannelType;

class IChannelImpl : public ni::dataplugin::IChannel
{
public:
  
  IChannelImpl(const tChannelType& type);
  virtual ~IChannelImpl();

  HRESULT GetName(BSTR& name) const override;
  HRESULT GetDescription(BSTR& description) const override;
  HRESULT GetUnit(BSTR& unit) const override;
  HRESULT GetMinimum(DOUBLE& minimum) const override;
  HRESULT GetMaximum(DOUBLE& maximum) const override;
  HRESULT CountCustomProperties(ULONG& count) const override;
  HRESULT GetCustomProperty(ULONG index, BSTR& name, VARIANT& value) const override;

  HRESULT GetLength(ULONG64& val) const override;
  HRESULT IsImplicit(ULONG& val) const override;
  HRESULT IsRawLinear(ULONG& val, DOUBLE& offset, DOUBLE& factor) const override;
  HRESULT GetDataType(VARTYPE& val) const override;
  HRESULT GetValues(ULONG64 numberToSkip, ULONG numberToTake, VARIANT& values) const override;

private:
  IChannelImpl& operator=(const IChannelImpl& rhs) = delete;
  IChannelImpl(const IChannelImpl& rhs) = delete;

public:

  HRESULT STDMETHODCALLTYPE QueryInterface (REFIID queriedIID, void** retval) override
  {
    *retval = 0;
    if(queriedIID == ni::dataplugin::IID_IChannel) { *retval = static_cast<IChannelImpl*>(this); }
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
  const tChannelType type_;
};