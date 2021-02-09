// only include once in your project

#include <ni/dataplugin/IRootExport.h>
#include <ni/dataplugin/IRootFactory.h>
#include <ni/dataplugin/IRoot.h>

extern "C" {

  /**
   \brief  Entry point for DataPlugin
  */
  __declspec(dllexport) HRESULT __cdecl USP_TDM_MARSHALLER(REFIID queriedIID, void** retval)
  {
    try {
      if (ni::dataplugin::IID_IRoot == queriedIID) {
        *retval = ni::dataplugin::factory::IRootBaseFactory();
        if (0 == retval) {
          return E_FAIL;
        }
      }
      else if (ni::dataplugin::IID_IRootExport == queriedIID) {
        *retval = ni::dataplugin::factory::IRootExportFactory();
        if (0 == retval) {
          return E_FAIL;
        }
      }
      else {
        retval = 0;
        return E_FAIL;
      }
    }
    catch (...) {
      return E_FAIL;
    }
    return S_OK;
  }

} /* extern "C" */
