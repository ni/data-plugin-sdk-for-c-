# Implementation of a NI C++ Read & Write File DataPlugin

## Description

This example is a merge of example1 and example2. Which means it can be used to read and write 
with a single dll and two entries in the URI file.

The TDM interface is represented by the virtual classes stored in the following headers:

```cpp
#include <ni/dataplugin/IRoot.h>
#include <ni/dataplugin/IGroup.h>
#include <ni/dataplugin/IChannel.h>

#include "IRootExportImpl.h"
```

The DLL interface is created by the following code, which works as a factory for your implementation:

```cpp
#include <ni/dataplugin/main/ImportExport.hpp>

namespace ni { namespace dataplugin { namespace factory {

  // we have to implement the root factory is used by ni/dataplugin/main.hpp
  ni::dataplugin::IRoot* __cdecl IRootBaseFactory()
  {
    return new (std::nothrow) ???();
  }

  // we have to implement the root factory which is used by ni/dataplugin/main.hpp
  ni::dataplugin::IRootExport* __cdecl IRootExportFactory()
  {
    return new (std::nothrow) ????;
  }

}}}
```

## Example Installer

After you have created a C++ DataPlugin DLL, you need to build the installer.

The C++ DataPlugin installer must contain at least the following three files:

- example3.dll (exports NI DataPlugin interface)
- example3.uri (registers your DataPlugin in the NI environment)
- [register_data_plugin.exe](../../register_data_plugin/README.md) (registers the URI file)

The URI file specifies that the `example3.dll` is installed in the folder:
`C:\Program Files\National Instruments\Shared\USI\DataPlugins\example3`

It looks like this:
```xml
<?xml version="1.0" encoding="utf-8"?>
<usireginfo version="19.0.0">

  <storetype name="example3">
    <alias>example3</alias>
    <defaultfiltefilter extension="example3"/>
    <filefilters extension="example3">
         <description>example3 C++ DataPlugin(*.example3)</description>
    </filefilters>
    <description>example3 C++ DataPlugin</description>
    <easypluginparam><![CDATA[<dllpath>C:\Program Files\National Instruments\Shared\USI\DataPlugins\example3\example3.dll</dllpath>]]></easypluginparam>

    <importsupported>1</importsupported>
    <exportsupported>0</exportsupported>
    <filepath>uspTdmMarshaller.dll</filepath>
    <model>TDM_1_0</model>
    <filtercasesensitive>NO</filtercasesensitive>
    <platform>x64</platform>
  </storetype>

  <storetype name="example3_EXPORT">
    <alias>example3_EXPORT</alias>
    <defaultfiltefilter extension="example3"/>
    <filefilters extension="example3">
      <description>example3 C++ DataPlugin(*.example3)</description>
    </filefilters>
    <description>example3 C++ DataPlugin</description>
    <easypluginparam><![CDATA[<exportplugin>true</exportplugin><dllpath>C:\Program Files\National Instruments\Shared\USI\DataPlugins\example3\example3.dll</dllpath>]]></easypluginparam>

    <importsupported>0</importsupported>
    <exportsupported>1</exportsupported>
    <filepath>uspTdmMarshaller.dll</filepath>
    <model>TDM_1_0</model>
    <filtercasesensitive>NO</filtercasesensitive>
    <platform>x64</platform>
  </storetype>

</usireginfo>
```

#### Installation Tasks

- Install the three files mentioned above to this folder:
  `C:\Program Files\National Instruments\Shared\USI\DataPlugins\example3`
- Register the DataPlugin by calling:
  `"C:\Program Files\National Instruments\Shared\USI\DataPlugins\example3\register_data_plugin.exe" -r "C:\Program Files\National Instruments\Shared\USI\DataPlugins\example3\example3.uri"`
- Check whether `register_data_plugin.exe` was returned with 0.
  Otherwise, the registration failed.


## Debugging the DataPlugin in Visual Studio with DIAdem

### Debug Read File

- Put a sample data file in a path without spaces
- Set the debug parameters:

  Parameter         | Value
  ----------------- | ------------------------------------------------------ 
  Command           | C:\Program Files\National Instruments\DIAdem 20{??}\DIAdem.exe
  Command Arguments | `/CDataFileLoad('"C:\temp\data.example3"','example3') /CDataDelAll(1) /CProgramExit()`
  Working Directory | `C:\Program Files\National Instruments\DIAdem 20{??}`

- Set a breakpoint for the method `Initialize`.

  ```cpp
  HRESULT IRootImpl::Initialize(const BSTR& filename)
  ```

- Debug your code

### Debug Write File

- Set the debug parameters.

  Parameter         | Value
  ----------------- | ------------------------------------------------------ 
  Command           | C:\Program Files\National Instruments\DIAdem 20{??}\DIAdem.exe
  Command Arguments | /CDataFileSave('"C:\temp\data.example3"','example3_EXPORT') /CDataDelAll(1) /CProgramExit()
  Working Directory | C:\Program Files\National Instruments\DIAdem 20{??}

- Set a breakpoint for the method `Execute`.

  ```cpp
  HRESULT IRootExportImpl::Execute(const BSTR& paramString, const BSTR& initParamString, const ni::dataplugin::IRoot* root, BSTR& errorMsg)
  ```


### DIAdem VBS Code To Call DataPlugin

```vbscript
Call DataFileLoad("C:\temp\data.example3","example3")
Call DataFileSave("C:\temp\data_out.example3","example3_EXPORT")
```
