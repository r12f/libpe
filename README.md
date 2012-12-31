# LibPE
LibPE is a simple parser for PE file format.

## Design principles (May be not achieved)
### Easy-To-Use
* Get every part of PE file directly instead of RVA or FOA or even raw data structures.
* Compatible with CComPtr and CComQIPtr. (NOT COM, just CComPtr. Easy to manage object's life circle and integrate it with your project on Windows.)

### Compatible with PE32(x86) and PE32+(x64)
* No doubt, it's a must-have ability.
* Get file info through IPEFile instead of IPEFile32 and IPEFile64, simplify your code.

### Good performance
* Minimize the Disk IO.

### Cross platform
* Support windows & linux.

### Why Not XXX?
* Why no boost: Boost is a very powerful tool. But you need checkout lots of code before you use LibPE. And of course, you should learn boost first before you see the code.
