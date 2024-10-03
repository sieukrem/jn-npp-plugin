# jN Npp Plugin

`jN Npp Plugin` is a plugin for Notepad++, which allows you to extend Notepad++ by writing JavaScript code.

## Technology

`jN` uses the built-in javascript engine of Microsoft Windows. This powerful engine allows to access a lot of ActiveX based
services like Shell, WMI of operating system.

`jN` wraps the native Notepad++ API into ActiveX interfaces accessible via global objects `Editor` and `System` in your JavaScript code.

## How to Use - Getting Started

You will find the feature list and examples in [wiki](https://github.com/sieukrem/jn-npp-plugin/wiki).

## For Developers

### Folder Structure

- `common` - implementation of Notepad++ independent ActiveX elements (e.g. Dialog, Menu, WinApi, System, ...).
- `editor` - implementation of Notepad++ related ActiveX elements (e.g. DockableDialog, View, ViewLine).
- `npp` - copy-in files from original Notepad++ plugin template project.
- `deploy` - collection of JavaScript files, which were meant to show capabilities of `jN`, but contain also some useful functions like XML, Grep, Zen Coding, SmartHighlighter. 

### Building

Open `jN.sln` in Visual Studio and build solution.

> Rebuild entire solution every time you modified any of `*.idl` files!
