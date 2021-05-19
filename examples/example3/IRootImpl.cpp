// own header
#include "IRootImpl.h"

// c++
#include <memory>

// _variant_t & _bstr_t
#include <comutil.h>

// local project
#include "IGroupImpl.h"

namespace ni {
  namespace dataplugin {
    namespace factory {

      // we have to implement the root factory which is used by ni/dataplugin/main.hpp
      ni::dataplugin::IRoot* __cdecl IRootBaseFactory()
      {
        return new (std::nothrow) IRootImpl();
      }

    }
  }
}

IRootImpl::IRootImpl()
{
}

IRootImpl::~IRootImpl()
{
  for each(auto child in children_) {
    child->Release();
  }
}

HRESULT IRootImpl::Initialize(const BSTR& filename)
{
  children_.push_back(new IGroupImpl());

  return S_OK;
}

HRESULT IRootImpl::GetNumberOfGroups(ULONG& count) const
{
  count = static_cast<ULONG>(children_.size());
  return S_OK;
}

HRESULT IRootImpl::GetGroup(ULONG index, ni::dataplugin::IGroup*& group)
{
  if(index >= children_.size()) {
    group = 0;
    return E_FAIL;
  }

  group = children_[index];
  group->AddRef();

  return S_OK;
}

HRESULT IRootImpl::GetName(BSTR& name) const
{
  name = _bstr_t(L"root").Detach();
  return S_OK;
}

HRESULT IRootImpl::GetDescription(BSTR& description) const 
{
   description = _bstr_t(L"root description").Detach();
   return S_OK;
}

HRESULT IRootImpl::GetAuthor(BSTR& author) const
{
   author = _bstr_t(L"National Instruments").Detach();
   return S_OK;
}

HRESULT IRootImpl::GetTitle( BSTR& title ) const
{
   // title = ???
   // no title available. Return S_FALSE for success and no value for title
   return S_FALSE;
}

HRESULT IRootImpl::GetDate( DATE& date ) const
{
   SYSTEMTIME st; GetLocalTime(&st);
   SystemTimeToVariantTime(&st, &date);

   return S_OK;
}

HRESULT IRootImpl::CountCustomProperties(ULONG& count) const
{
  count = 2;
  return S_OK;
}

HRESULT IRootImpl::GetCustomProperty( ULONG index, BSTR& name, VARIANT& value) const
{
  switch(index) {
  case 0:
    name = _bstr_t(L"r0").Detach();
    value = _variant_t(double(3.0)).Detach();
    return S_OK;
    break;
  case 1:
    name = _bstr_t(L"r1").Detach();
    value = _variant_t(double(3.1)).Detach();
    return S_OK;
    break;
  default:
    return S_FALSE;
    break;
  }
}

