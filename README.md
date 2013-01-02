# LibPE
LibPE is a simple parser for PE file format under [The BSD 2-Clause License](http://opensource.org/licenses/BSD-2-Clause).

## Design principles (May be not achieved)
### Easy-To-Use
* Get every part of PE file directly instead of RVA or FOA or even raw data structures.
* Using reference count to manage object's life circle.
* Support compilation of lib and dll.

### Compatible with PE32(x86) and PE32+(x64)
* No doubt, it's a must-have ability.
* Get file info through IPEFile instead of IPEFile32 and IPEFile64, simplify your code.

### Good performance
* Do not parse any content until you need it.
* Minimize the Disk IO.

### Cross platform
* Support windows & linux.

## Why / Why Not XXX?
* Why using basic types on windows?
	I presume most of the users are developing on Windows platform. So we should minimize the things you may learn.

* Why using COM-like interface?
	This is the problem I grapple with for most of the time. Althrough it makes LibPE harder to use, we still use it to avoid the use-after-free crashes and to solve the cross-dll problems.
	And we do not need QueryInterface due to the simple class hierarchy. And also we do not support COM in order to make LibPE easier to use.
	
* Why no boost?
	Boost is a very powerful tool. But you need checkout lots of code before you use LibPE. And of course, you should learn boost first before you see the code.
	
* Why so many template
	When you need a new type of PE parser, define your own PETrait and change the LIBPE_FORCE_TEMPLATE_REDUCTION_XXX macro. And wala! 

## Copyright
Copyright (c) 2012-2013, Jiang Jinzhou. See [LICENSE](https://github.com/r1f/libpe/blob/master/LICENSE) for details.