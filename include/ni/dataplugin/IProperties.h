#pragma once

#include <Unknwn.h>
#include <WTypes.h>

namespace ni {
  namespace dataplugin {

    // {82722A7A-70C8-4eb5-BCB9-6EB694A62F5D}
    /**
      \brief  GUID for properties interface
    */
    static const GUID IID_IProperties =
    { 0x82722a7a, 0x70c8, 0x4eb5, { 0xbc, 0xb9, 0x6e, 0xb6, 0x94, 0xa6, 0x2f, 0x5d } };

    /**
      \brief  Interface for properties access
    */
    class IProperties : public IUnknown
    {
    public:

      /**
        \brief  Get the name of this instance
        \param  name  [Out]  name of the object
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetName(BSTR& name) const = 0;

      /**
        \brief  Get the description of this instance
        \param  description [Out]  name of the object
        \return S_OK for success with value, S_FALSE for success with no value and E_FAIL, ... for failure
      */
      virtual HRESULT GetDescription(BSTR& description) const = 0;

      /**
        \brief Count the number of custom properties of this instance
        \param count [Out]  Number of custom properties (instance attributes)
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT CountCustomProperties(ULONG& count) const = 0;

      /**
        \brief     Get a custom property
        \param     index Index of the custom property (0-based)
        \param     name  [Out] Name of the custom property
        \param     value [Out]   Container to return the value(s) of this property
        \return    return S_FALSE if no custom property is available at this index. S_OK if it exists and E_FAIL, ... for failure
        \remark VT_DATE has to be returned as local time
        \remark any invalid characters within the property name will be replaced by '_'
      */
      virtual HRESULT GetCustomProperty(ULONG index, BSTR& name, VARIANT& value) const = 0;
    };

  }
}
