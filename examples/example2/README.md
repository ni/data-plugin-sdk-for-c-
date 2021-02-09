# Implementing a Writing NI C++ DataPlugin

## Description

This example shows you how to create a DataPlugin to write your own file formats from DIAdem,
LabVIEW or SystemLink. The C++ DataPlugin implements the [TDM Data Model](http://www.ni.com/white-paper/11943/en/).
It is a *pull interface*. Request content to write in any sequence necessary.

The TDM interface is represented by the virtual classes stored in the following five headers:

```cpp
#include <ni/dataplugin/IRootExport.h>
#include <ni/dataplugin/IRoot.h>
#include <ni/dataplugin/IGroup.h>
#include <ni/dataplugin/IChannel.h>
#include <ni/dataplugin/ParamString.h>
```

Create the DLL interface with the following code, which works as a factory for your implementation:

```cpp
#include <ni/dataplugin/main/Export.hpp>

namespace ni { namespace dataplugin { namespace factory {

  // we have to implement the root factory which is used by ni/dataplugin/main.hpp
  ni::dataplugin::IRootExport* __cdecl IRootExportFactory()
  {
    return new (std::nothrow) ????;
  }

}}}
```

### Example Installer

After you have created a C++ DataPlugin DLL, you need to build the installer.

The C++ DataPlugin installer must contain at least the following three files:

- example2.dll (exports NI DataPlugin interface)
- example2.uri (registers your DataPlugin in the NI environment)
- [register_data_plugin.exe](../../register_data_plugin/README.md) (registers the URI file)

The URI file specifies that `example2.dll` is installed in the folder:
`C:\Program Files\National Instruments\Shared\USI\DataPlugins\example2`

It looks like this:
```xml
<?xml version="1.0" encoding="utf-8"?>
<usireginfo version="19.0.0">
  <storetype name="example2">
    <alias>example2</alias>
    <defaultfiltefilter extension="example2"/>
    <filefilters extension="example2">
         <description>example2 C++ DataPlugin (*.example2)</description>
    </filefilters>
    <description>example2 C++ DataPlugin</description>
    <easypluginparam><![CDATA[<dllpath>C:\Program Files\National Instruments\Shared\USI\DataPlugins\example2\example2.dll</dllpath>]]></easypluginparam>

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
  `C:\Program Files\National Instruments\Shared\USI\DataPlugins\example`
- Register the DataPlugin by calling:
  `"C:\Program Files\National Instruments\Shared\USI\DataPlugins\example2\register_data_plugin.exe" -r "C:\Program Files\National Instruments\Shared\USI\DataPlugins\example2\example2.uri"`
- Check whether `register_data_plugin.exe` was returned with 0. Otherwise, the registration failed.


## Debugging the DataPlugin in Visual Studio with DIAdem

- Put a sample data file in a path without spaces
- Set the debug parameters.

  Parameter         | Value
  ----------------- | ------------------------------------------------------ 
  Command           | C:\Program Files\National Instruments\DIAdem 20{??}\DIAdem.exe
  Command Arguments | /CDataFileSave('"C:\temp\data.example2"','example2_EXPORT') /CDataDelAll(1) /CProgramExit()
  Working Directory | C:\Program Files\National Instruments\DIAdem 20{??}

- Set a breakpoint for the method `Execute`.

  ```cpp
  HRESULT IRootExportImpl::Execute(const BSTR& paramString, const BSTR& initParamString, const ni::dataplugin::IRoot* root, BSTR& errorMsg)
  ```

- Debug your code

### DIAdem VBS Code That Calls the DataPlugin

```vbscript
Call DataFileSave("C:\temp\data.example2","example2")
```
