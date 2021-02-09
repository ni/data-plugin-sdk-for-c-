#pragma once

#include <Unknwn.h>
#include <WTypes.h>

namespace ni {
  namespace dataplugin {

    // {8AC27C43-5132-4252-9EFF-92CA0DDBBC25}
    /**
      \brief  GUID for error info interface
    */
    static const GUID IID_IErrorInfo =
    { 0x8ac27c43, 0x5132, 0x4252, { 0x9e, 0xff, 0x92, 0xca, 0xd, 0xdb, 0xbc, 0x25 } };

    /**
      \brief  Interface for error info
    */
    class IErrorInfo : public IUnknown
    {
    public:

      /**
        \brief  Get a description for the last occurred error
        \param  errorText  [Out]  Description of the last error that occurred
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetLastErrorText(BSTR& errorText) const = 0;
    };

  }
}
