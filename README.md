# LibPE
LibPE is a simple parser for PE file format.

## Design principles (May be not achieved)
### Easy-To-Use
* Get every part of PE file directly instead of RVA or FOA or even raw data structures.
* Compatible with CComPtr and CComQIPtr. (NOT COM, just CComPtr.)

### Compatible with PE32(x86) and PE32+(x64)
* No doubt, it's a must-have ability.
* Get file info through IPEFile instead of IPEFile32 and IPEFile64, simplify your code.

### Good performance
* Do not parse any content until you need it.
* Minimize the Disk IO.

### Cross platform
* Support windows & linux.

## Why / Why Not XXX?
* Why use basic types on windows: I presume most of the users are developing on Windows platform. So we should minimize the things you may learn.
* Why compatible with COM pointer: Using COM makes object's life circle very easy to manage and very easy to integrate with your project on Windows.
* Why no boost: Boost is a very powerful tool. But you need checkout lots of code before you use LibPE. And of course, you should learn boost first before you see the code.
* Why so many template: When you need a new type of PE parser, define your own PETrait and change the LIBPE_FORCE_TEMPLATE_REDUCTION_XXX macro. And wala! 