#pragma once

#include <ni/dataplugin/IChannel.h>

namespace ni {
  namespace dataplugin {

    // {EE8C8750-1D85-485c-9361-29BAE65B2707}
    /**
      \brief  GUID for group interface
    */
    static const GUID IID_IGroup =
    { 0xee8c8750, 0x1d85, 0x485c, { 0x93, 0x61, 0x29, 0xba, 0xe6, 0x5b, 0x27, 0x7 } };

    /**
      \brief  Interface for group access
    */
    class IGroup : public IProperties
    {
    public:
      /**
        \brief  Return the number of channels that will be imported.
        \param  count [Out]  Number of channels
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetNumberOfChannels(ULONG& count) const = 0;

      /**
        \brief  Get the interface of the related channel
        \param  index   zero based channel index
        \param  channel [Out] Channel object at the given position
        \return S_OK/S_FALSE for success and E_FAIL, ... for failure
      */
      virtual HRESULT GetChannel(ULONG index, IChannel*& channel) const = 0;
    };

  }
}
