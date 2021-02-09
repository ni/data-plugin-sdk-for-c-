// own header
#include "IGroupImpl.h"

// _variant_t & _bstr_t
#include <comutil.h>

// local project
#include "IChannelImpl.h"


IGroupImpl::IGroupImpl()
{
  children_.push_back(new IChannelImpl(eImplicitChn));
  children_.push_back(new IChannelImpl(eExplicitChn));
  children_.push_back(new IChannelImpl(eRawChn));
  children_.push_back(new IChannelImpl(eStrChn));
}

IGroupImpl::~IGroupImpl()
{
  for each(auto child in children_) {
    child->Release();
  }
}

HRESULT IGroupImpl::GetNumberOfChannels(ULONG& count) const
{
  count = static_cast<ULONG>(children_.size());
  return S_OK;
}

HRESULT IGroupImpl::GetChannel(ULONG index, ni::dataplugin::IChannel*& channel) const
{
  if(index >= children_.size()) {
    channel = 0;
    return E_FAIL;
  }

  channel = children_[index];
  channel->AddRef();

  return S_OK;
}

HRESULT IGroupImpl::GetName(BSTR& name) const
{
  name = _bstr_t(L"group").Detach();
  return S_OK;
}

HRESULT IGroupImpl::GetDescription(BSTR& description) const
{
  description = _bstr_t(L"group description").Detach();
  return S_OK;
}

HRESULT IGroupImpl::CountCustomProperties(ULONG& count) const
{
  count = 2;
  return S_OK;
}

HRESULT IGroupImpl::GetCustomProperty( ULONG index, BSTR& name, VARIANT& value) const
{
  switch(index) {
  case 0:
    name = _bstr_t(L"g0").Detach();
    value = _variant_t(double(2.0)).Detach();
    return S_OK;
    break;
  case 1:
    name = _bstr_t(L"g1").Detach();
    value = _variant_t(double(2.1)).Detach();
    return S_OK;
    break;
  default:
    return S_FALSE;
    break;
  }
}
