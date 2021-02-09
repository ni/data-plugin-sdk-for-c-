# Implementation of a NI C++ Read File DataPlugin

## Description

This example shows you how to create a DataPlugin to load, to register, or to
search your own file formats in DIAdem, LabVIEW, or SystemLink or to index and
to browse them with the [NI DataFinder](https://www.ni.com/en-us/innovations/white-papers/12/what-is-the-ni-datafinder-.html).

The C++ DataPlugin implements the [TDM Data Model](http://www.ni.com/white-paper/11943/en/).
It is a *pull interface*. The NI kernel will request information in any sequence necessary.

The TDM interface is represented by the virtual classes stored in the following three headers:

```cpp
#include <ni/dataplugin/IRoot.h>
#include <ni/dataplugin/IGroup.h>
#include <ni/dataplugin/IChannel.h>
```

The DLL interface is created by the following code, which works as a factory for your implementation:

```cpp
#include <ni/dataplugin/main/Export.hpp>

namespace ni { namespace dataplugin { namespace factory {

  // we have to implement the root factory is used by ni/dataplugin/main.hpp
  ni::dataplugin::IRoot* __cdecl IRootBaseFactory()
  {
    return new (std::nothrow) ???();
  }

}}}
```

## Example Installer

After you have created a C++ DataPlugin DLL, you need to build the installer.

The C++ DataPlugin installer must contain at least the following three files:

- example1.dll (exports NI DataPlugin interface)
- example1.uri (registers your DataPlugin in the NI environment)
- [register_data_plugin.exe](../../register_data_plugin/README.md) (registers the URI file)

The URI file specifies that the `example1.dll` is installed in the folder:
`C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1`

It looks like this:
```xml
<?xml version="1.0" encoding="utf-8"?>
<usireginfo version="19.0.0">
  <storetype name="example1">
    <alias>example1</alias>
    <defaultfiltefilter extension="example1"/>
    <filefilters extension="example1">
         <description>example1 C++ DataPlugin(*.example1)</description>
    </filefilters>
    <description>example1 C++ DataPlugin</description>
    <easypluginparam><![CDATA[<dllpath>C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1\example1.dll</dllpath>]]></easypluginparam>

    <importsupported>1</importsupported>
    <exportsupported>0</exportsupported>
    <filepath>uspTdmMarshaller.dll</filepath>
    <model>TDM_1_0</model>
    <filtercasesensitive>NO</filtercasesensitive>
    <platform>x64</platform>
  </storetype>
</usireginfo>
```

#### Installation Tasks

- Install the three files mentioned above to this folder:
  `C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1`
- Register the DataPlugin by calling:
  `"C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1\register_data_plugin.exe" -r "C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1\example1.uri"`
- Check whether `register_data_plugin.exe` was returned with 0.
  Otherwise, the registration failed.


## Debugging the DataPlugin in Visual Studio with DIAdem

- Put a sample data file in a path without spaces
- Set the debug parameters:

  Parameter         | Value
  ----------------- | ------------------------------------------------------ 
  Command           | C:\Program Files\National Instruments\DIAdem 20{??}\DIAdem.exe
  Command Arguments | `/CDataFileLoad('"C:\temp\data.example1"','example1') /CDataDelAll(1) /CProgramExit()`
  Working Directory | `C:\Program Files\National Instruments\DIAdem 20{??}`

- Set a breakpoint for the method `Initialize`.

  ```cpp
  HRESULT IRootImpl::Initialize(const BSTR& filename)
  ```

- Debug your code

### DIAdem VBS Code To Call DataPlugin

```vbscript
Call DataFileLoad("C:\temp\data.example1","example1")
```
