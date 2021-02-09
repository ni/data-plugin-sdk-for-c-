# Simple Read Test

This folder contains a tool that performs simple read tests while you are developing.

> The test application does not use the NI infrastructure, but calls the C++ DataPlugin API directly.

## Available Tests

### --read_tree

#### Usage

**simple_read_test.exe --read_tree CPP_PLUGIN_DLL_PATH FILE_TO_READ`**

#### Description

Passes a file to your "Read File C++ DataPlugin" and recursively executes the following steps:

- Access root with all properties
- Access groups with all properties
- Access channels with all properties
- Access first channel values
- Access last channel values
- Access more values than available
- Return 0 if no error occurred

#### Example

`simple_read_test.exe --read_tree "C:\PluginInstallPath\example1.dll" "C:\PluginInstallPath\data\data.example1"`

