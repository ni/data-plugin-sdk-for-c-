#pragma once

#include <ni/dataplugin/IGroup.h>

namespace ni {
  namespace dataplugin {

    // {9914456A-AE4E-483e-8DF5-6911E0A7480E}
    /**
      \brief  GUID for root interface
    */
    static const GUID IID_IRoot =
    { 0x9914456a, 0xae4e, 0x483e, { 0x8d, 0xf5, 0x69, 0x11, 0xe0, 0xa7, 0x48, 0xe } };

    /**
      \brief  Interface for root access
    */
    class IRoot : public IProperties
    {
    public:
      /**
      \brief  Initialize the root Element
      \param  filename   File to be opened for read
      \return - S_OK/S_FALSE for success
              - E_ACCESSDENIED if file cannot be accessed
              - E_INVALIDARG if this plugin cannot load the file
              - CO_E_SERVER_INIT_TIMEOUT if a timeout happened while opening the file
              - E_OUTOFMEMORY if there is not enough memory to load the file
              - E_FAIL for unspecified error
      */
      virtual HRESULT Initialize(const BSTR& filename) = 0;

      /**
      \brief  Return the number of groups that will be imported.
      \param  count [Out]  Number of groups
      \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetNumberOfGroups(ULONG& count) const = 0;

      /**
      \brief  Get the interface of the related channel
      \param  index   zero based group index
      \param  group [Out]  group object at the given position
      \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetGroup(ULONG index, IGroup*& group) = 0;

      /**
        \brief  Get the title of this instance
        \param  title  [Out]  title of the object
        \return S_OK for success with value, S_FALSE for success with no value and E_FAIL, ... for failure
      */
      virtual HRESULT GetTitle(BSTR& title) const = 0;

      /**
        \brief  Get the author of this instance
        \param  author  [Out]  author of the object
        \return S_OK for success with value, S_FALSE for success with no value and E_FAIL, ... for failure
      */
      virtual HRESULT GetAuthor(BSTR& author) const = 0;

      /**
        \brief  Get the date of this instance
        \param  date  [Out]  date of the object
        \return S_OK for success with value, S_FALSE for success with no value and E_FAIL, ... for failure
      */
      virtual HRESULT GetDate(DATE& date) const = 0;


      //////////////////////////////////////////////////////////////////////
      // optional part

      /**
      \brief  Give the parameter strings that were send to the plugin in a sequence.
      \param  paramString   Parameter string send to the plugin
      \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT SetParameter(const BSTR& /*paramString*/) { return S_OK; };

      //////////////////////////////////////////////////////////////////////////
      //

      /**
      \brief Will be called before initialize if opened by Indexer
      \param reserved   Reserved for future use.
      \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT OpenedByIndexer(const ULONG& /*reserved*/) { return S_OK; };
    };

  }
}