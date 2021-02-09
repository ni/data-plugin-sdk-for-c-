# NI DataPlugin SDK for C++

## Description

Create a DataPlugin to load, register, or search your own file formats in
DIAdem, LabVIEW, or SystemLink or to index and to browse them with
[NI DataFinder](https://www.ni.com/en-us/innovations/white-papers/12/what-is-the-ni-datafinder-.html).
You can also create a DataPlugin to write to a file format. You can create
DataPlugins in the following languages:

- [VBScript](https://zone.ni.com/reference/en-XX/help/370858P-01/TOC896.htm)
- [Python](https://github.com/ni/vscode-ni-python-dataplugins)
- C++

**Before you implement a DataPlugin in C++, consider using VBScript or Python instead.
Creating a DataPlugin to read files with VBScript or Python is much easier because
NI provides appropriate helper functions.** 

Use C++ if you need to create a writing DataPlugin or if you need to include an
extensive library to handle the data format. If you need to create a DataPlugin
to read **and** write files, consider implementing the reading part with VBScript
or Python. For C++ DataPlugins, you must also create an installer.

VBScript DataPlugins, on the other hand, are very easy to create. DIAdem VBScript
provides many helper interfaces to easily import the most commonly used file
structures, like:

- CSV
- XLSX
- Binary files

DIAdem also provides wizards to help you create a DataPlugin.

At the bottom of the [DataPlugins Programmer's Reference Guide](https://www.ni.com/en-us/support/documentation/supplemental/04/dataplugins-programmer-s-reference-guide.html)
you will find a help file with very good hints on how to use VBScript DataPlugins.
Refer to the [DIAdem help](https://zone.ni.com/reference/en-XX/help/370858P-01/TOC896.htm)
for [DataPlugin examples](https://zone.ni.com/reference/en-XX/help/370858P-01/TOC898.htm).
You can also find this help in DIAdem.

If you decide to write a C++ DataPlugin, note that there are **no NI-provided helper functions**
for opening binary or text files. You implement using C++ methods and libraries which you then
include in the DataPlugin installer.

A C++ DataPlugin

- Provides an interface C++ structure containing only pure virtual functions provided by a DLL
- Supports NI products with version 2020 or higher
- Supports only 64-bit applications

The stable DLL interface of the C++ version (including memory management) is based on
Microsoft COM VARIANT elements:
  - [_variant_t](https://docs.microsoft.com/en-us/cpp/cpp/variant-t-class)
  - [_bstr_t](https://docs.microsoft.com/en-us/cpp/cpp/bstr-t-class)



## Examples

### C++ DataPlugin to Read a File

[example1](examples/example1/README.md) is an example of a DataPlugin that loads files.

#### Using the DataPlugin in DIAdem

> The following code loads an existing file into the DIAdem Data Portal:

```vbsscript
Call DataFileLoad("C:\git\dataplugin_cpp_sdk\examples\example1\data\data.example1", "example1")
```

### C++ DataPlugin to Write a File

[example2](examples/example2/README.md) is an example of a DataPlugin that exports files (DataFileSave).

#### Using the DataPlugin in DIAdem

> The following code saves the content of the DIAdem Data Portal to a file:

```vbsscript
Call DataFileSave("C:\temp\data.example2", "example2_EXPORT")
```


## Installer

After you have created a C++ DataPlugin DLL, you need to build the installer.

The installer must contain at least the following three files:

- {plugin}.dll
- {plugin}.uri
- register_data_plugin.exe

### Collect Files to Create an Installer

Run *CMake install* to collect files for the `example1` installer:

- example1.dll
- example1.uri
- [register_data_plugin.exe](register_data_plugin/README.md)

To register a DataPlugin in the NI environment, you need a URI file that describes
your DataPlugin DLL. It looks like the following and specifies that the `example1.dll`
is installed in this folder (**Make sure the path is correct for your installer**):
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

- Install the three files above into this folder:
  `C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1`
- Register the DataPlugin by calling:
  `"C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1\register_data_plugin.exe" -r "C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1\example1.uri"`
- Check whether `register_data_plugin.exe` was returned with 0.
  Otherwise, the registration failed.

#### Uninstallation Tasks

- Unregister DataPlugin by calling:
  `"C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1\register_data_plugin.exe" -u "C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1\example1.uri"`
- Remove the folder:
  `C:\Program Files\National Instruments\Shared\USI\DataPlugins\example1`

## Simple Read Test

This [folder](tests/simple_read_test/README.md) contains a tool that performs
simple read tests while you are developing.
