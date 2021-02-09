#pragma once

#include <Unknwn.h>

namespace ni {
  namespace dataplugin {

    class IRoot;

    // {AA9EC417-CED4-47CB-80CB-8EC0BC52573E}
    /**
      \brief  GUID for root interface
    */
    static const GUID IID_IRootExport =
    { 0xaa9ec417, 0xced4, 0x47cb, { 0x80, 0xcb, 0x8e, 0xc0, 0xbc, 0x52, 0x57, 0x3e } };

    /**
      \brief  Interface for root access
    */
    class IRootExport : public IUnknown
    {
    public:
      /**
      \brief  Give the parameter strings that were send to the plugin in a sequence.
      \param  paramString  Parameter string send to the plugin
      \param  initParamString  Contains the initial parameter string that might contain alternative defaults
      \param  root  content to be exported
      \param  errorMsg  Message describing error
      \return S_OK for success and E_FAIL, ... for failure
      */
      virtual HRESULT Execute(const BSTR& paramString, const BSTR& initParamString, const IRoot* root, BSTR& errorMsg) = 0;
    };

    namespace factory {
      /**
        \brief  Create an instance of the given interface
      */
      ni::dataplugin::IRootExport* __cdecl IRootExportFactory();
    }
  }
}

