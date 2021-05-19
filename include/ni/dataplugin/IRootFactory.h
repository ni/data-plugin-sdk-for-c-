#pragma once

namespace ni {
  namespace dataplugin {

    class IRoot;

    namespace factory {

      /**
        \brief  Create an instance of the given interface
      */
      ni::dataplugin::IRoot* __cdecl IRootBaseFactory();

    }
  }
}
