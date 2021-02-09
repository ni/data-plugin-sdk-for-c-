#pragma once

#include <ni/dataplugin/IProperties.h>

namespace ni {
  namespace dataplugin {

    // {45E2EE9A-BBD8-4831-A97B-0B74E3F9E28F}
    /**
      \brief  GUID for channel interface
    */
    static const GUID IID_IChannel =
    { 0x45e2ee9a, 0xbbd8, 0x4831, { 0xa9, 0x7b, 0xb, 0x74, 0xe3, 0xf9, 0xe2, 0x8f } };

    /**
      \brief  Interface for channel access
    */
    class IChannel : public IProperties
    {
    public:
      /**
        \brief  Get the length of the channel
        \param  val [Out] The length of the channel
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetLength(ULONG64& val) const = 0;

      /**
        \brief  Get the data type of the related channel
        \param  val [Out] data type of the channel
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetDataType(VARTYPE& val) const = 0;

      /**
        \brief  Get the unit of this instance
        \param  unit  [Out]  unit of the object
        \return S_OK for success with value, S_FALSE for success with no value and E_FAIL, ... for failure
      */
      virtual HRESULT GetUnit(BSTR& unit) const = 0;

      /**
        \brief  Get minimum for the data of this instance
        \param  minimum  [Out]  minimum of the data
        \return S_OK for success with value, S_FALSE for success with no value and E_FAIL, ... for failure
        \remark If channel data type is VT_DATE, value of minimum must be represented as specified for DATE type
      */
      virtual HRESULT GetMinimum(DOUBLE& minimum) const = 0;

      /**
        \brief  Get maximum for the data of this instance
        \param  maximum  [Out]  maximum of the data
        \return S_OK for success with value, S_FALSE for success with no value and E_FAIL, ... for failure
        \remark If channel data type is VT_DATE, value of maximum must be represented as specified for DATE type
      */
      virtual HRESULT GetMaximum(DOUBLE& maximum) const = 0;

      /**
        \brief  Get the actual data
        \param  numberToSkip    number of values to skip at the beginning of the channel
        \param  numberToTake    number of values to be retrieved
        \param  values [Out]  destination to transport the requested data
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetValues(ULONG64 numberToSkip, ULONG numberToTake, VARIANT& values) const = 0;


      //////////////////////////////////////////////////////////////////////
      // optional part

      /**
        \brief  Determine if the given channel is implicit
        \param  val [Out]  1 if channel is implicit / 0 if it is no implicit
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT IsImplicit(ULONG& val) const { val = 0; return S_OK; };

      /**
        \brief  Determine if the given channel is raw linear scaled
        \remark All raw linear channels will end up as VT_R8 channels
        \param  val    [Out]  1 if channel is raw linear / 0 if it is no raw linear
        \param  offset [Out]  offset to be added to each channel value
        \param  factor [Out]  factor to be multiplied with each channel value
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT IsRawLinear(ULONG& val, DOUBLE& offset, DOUBLE& factor) const { val = 0; offset = 0.0; factor = 1.0; return S_OK; };

      /**
        \brief  Get the length of the channel
        \param val [Out] The length of the channel
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetFlagsLength(ULONG64& val) const { val = 0; return S_OK; }

      /**
        \brief  Get invalid flags for the given channel
        \param  numberToSkip    number of values to skip at the beginning of the channel
        \param  numberToTake    number of values to be retrieved
        \param  values [Out]   destination to transport the requested flags
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetFlags(ULONG64 numberToSkip, ULONG numberToTake, VARIANT& values) const { numberToTake; values; if (0 != numberToSkip) { return E_FAIL; }; return S_OK; }
    };

  }
}
