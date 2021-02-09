# Register DataPlugin Utility

To register a DataPlugin in the NI infrastructure, a URI file describing your
plugin must be registered with NI usireg.exe.

This folder contains all files to generate `register_data_plugin.exe` used
in an installation utility.

The following sequence is performed in the utility:

- Determine NI USI bin folder by reading 
  `HKEY_LOCAL_MACHINE\SOFTWARE\National Instruments\UsiCore\binDir`
  from the registry.
- Call `usireg.exe` with option `/o`(overwrite) or `/u`(unregister) using 
  the specified URI file
- Return 0 if everything was successful

> You need to integrate `register_data_plugin.exe` in your installer to register
> and unregister a DataPlugin. It simplifies installation and debugging.

## Register Plugin

`"C:\PluginInstallPath\register_data_plugin.exe" -r "C:\PluginInstallPath\example1.uri"`

## Unregister Plugin

`"C:\PluginInstallPath\register_data_plugin.exe" -u "C:\PluginInstallPath\example1.uri"`
