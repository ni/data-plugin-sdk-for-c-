#pragma once

namespace ni {
  namespace dataplugin {
    namespace factory {

      class IRoot;

      /**
        \brief  Create an instance of the given interface
      */
      ni::dataplugin::IRoot* __cdecl IRootBaseFactory();

    }
  }
}
