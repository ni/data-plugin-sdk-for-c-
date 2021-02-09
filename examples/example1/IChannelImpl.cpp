// own header
#include "IChannelImpl.h"

// _variant_t & _bstr_t
#include <comutil.h>

// C++
#include <algorithm>

#ifdef min
# undef min
#endif


namespace {
  double dataR64__[] = {1.1, 1.2, 1.3, 1.4}; // example data
  short dataI16__[] = {7, 8, 9, 10}; // example data
  std::wstring dataStr__[] = { L"first", L"second", L"third", L"fourth" }; // example data
  double impR64__[] = {1.0, 0.1}; // example data
}

IChannelImpl::IChannelImpl(const tChannelType& type) : type_(type)
{
}

IChannelImpl::~IChannelImpl()
{
}

HRESULT IChannelImpl::GetLength(ULONG64& val) const
{
  // in this example all values have the same amount of values
  val = sizeof(dataR64__) / sizeof(double);
  return S_OK;
}

HRESULT IChannelImpl::GetDataType(VARTYPE& val) const
{
  switch (type_)
  {
  case eRawChn:
    val = VT_I2;
    break;
  case eStrChn:
    val = VT_BSTR;
    break;
  default:
    val = VT_R8;
    break;
  }
  return S_OK;
}

HRESULT IChannelImpl::IsImplicit(ULONG& val) const
{
  val = (eImplicitChn == type_ ? 1 : 0);
  return S_OK;
}

HRESULT IChannelImpl::IsRawLinear(ULONG& val, DOUBLE& offset, DOUBLE& factor) const 
{
  if (eRawChn == type_) {
    val = 1; 
    offset = 1; factor = 0.1; 
  }
  else {
    val = 0; 
  }
  return S_OK; 
}

HRESULT IChannelImpl::GetValues(ULONG64 numberToSkip, ULONG numberToTake, VARIANT& values) const
{
  ULONG64 numAvailable(0);
  if(eImplicitChn == type_) {
    numAvailable = 2;
  }
  else {
    GetLength(numAvailable);
  }

  if(numberToSkip > numAvailable) {
    return E_FAIL;
  }

  const ULONG numTakeable = static_cast<ULONG>(std::min(ULONG64(numberToTake), (numAvailable - numberToSkip)));
  if(0 == numTakeable) {
    // nothing to do so we return
    return S_OK;
  }

  if     (eRawChn == type_) {
    SAFEARRAY* psaStruct = SafeArrayCreateVector(VT_I2, 0, numTakeable);
    if (nullptr == psaStruct) {
      return E_OUTOFMEMORY;
    }
    for(LONG i = 0; i < LONG(numTakeable); ++i) {
      SafeArrayPutElement(psaStruct, &i, &dataI16__[numberToSkip + i]);
    }
    values.vt = VT_ARRAY | VT_I2;
    values.parray = psaStruct;  
  }
  else if(eImplicitChn == type_) {
    SAFEARRAY* psaStruct = SafeArrayCreateVector(VT_R8, 0, numTakeable);
    if (nullptr == psaStruct) {
      return E_OUTOFMEMORY;
    }
    for(LONG i = 0; i < LONG(numTakeable); ++i) {
      SafeArrayPutElement(psaStruct, &i, &impR64__[numberToSkip + i]);
    }
    values.vt = VT_ARRAY | VT_R8;
    values.parray = psaStruct;  
  }
  else if(eStrChn == type_) {
    SAFEARRAY* psaStruct = SafeArrayCreateVector(VT_BSTR, 0, numTakeable);
    if (nullptr == psaStruct) {
      return E_OUTOFMEMORY;
    }
    for (LONG i = 0; i < LONG(numTakeable); ++i) {
      _bstr_t strValue{ dataStr__[numberToSkip + i].c_str() };
      SafeArrayPutElement(psaStruct, &i, strValue.GetBSTR());
    }
    values.vt = VT_ARRAY | VT_BSTR;
    values.parray = psaStruct;
  }
  else {
    SAFEARRAY* psaStruct = SafeArrayCreateVector(VT_R8, 0, numTakeable);
    if (nullptr == psaStruct) {
      return E_OUTOFMEMORY;
    }
    for(LONG i = 0; i < LONG(numTakeable); ++i) {
      SafeArrayPutElement(psaStruct, &i, &dataR64__[numberToSkip + i]);
    }
    values.vt = VT_ARRAY | VT_R8;
    values.parray = psaStruct;  
  }

  return S_OK;
}

HRESULT IChannelImpl::GetName(BSTR& name) const
{
  switch(type_) {
  case eImplicitChn : name = _bstr_t(L"impl").Detach(); break;
  case eRawChn      : name = _bstr_t(L"raw").Detach(); break;
  case eStrChn      : name = _bstr_t(L"string").Detach(); break;
  default           : name = _bstr_t(L"channel").Detach(); break;
  }
  return S_OK;
}

HRESULT IChannelImpl::GetDescription( BSTR& description ) const
{
   description = _bstr_t(L"channel description").Detach();
   return S_OK;
}

HRESULT IChannelImpl::GetUnit( BSTR& unit ) const
{
   unit = _bstr_t(L"m/s").Detach();
   return S_OK;
}

HRESULT IChannelImpl::GetMinimum( DOUBLE& minimum ) const
{
   // minumum = ???
   // no minimum available. Return S_FALSE for success and no value for minimum
   return S_FALSE;
}

HRESULT IChannelImpl::GetMaximum( DOUBLE& maximum ) const
{
   // maximum = ???
   // no maximum available. Return S_FALSE for success and no value for maximum
   return S_FALSE;
}

HRESULT IChannelImpl::CountCustomProperties(ULONG& count) const
{
  count = 2;
  return S_OK;
}

HRESULT IChannelImpl::GetCustomProperty(ULONG index, BSTR& name, VARIANT& value) const
{
  switch(index) {
  case 0:
    name = _bstr_t(L"c0").Detach();
    value = _variant_t(double(1.0)).Detach();
    return S_OK;
    break;
  case 1:
    name = _bstr_t(L"c1").Detach();
    value = _variant_t(double(1.1)).Detach();
    return S_OK;
    break;
  default:
    return S_FALSE;
    break;
  }
}
