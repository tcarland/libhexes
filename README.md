libhexes
=========

 Copyright (c) 2011 Timothy Charlton Arland <tcarland@gmail.com>
  
## LICENSE
  
  libhexes is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as 
  published by the Free Software Foundation, either version 3 of 
  the License, or (at your option) any later version.
 
  libhexes is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public 
  License along with libhexes.  
  If not, see <http://www.gnu.org/licenses/>.


## Overview

***libhexes*** is a C++ library that provides an object-oriented 
approach to developing with the ncurses C library. 

The initial library development was concerned primarily with wrapping 
ncurses and specifically the panel API within convenient objects for 
simplified use in C++ applications. A few widgets have been created so 
far. Focus has been on the framework stability and ease for application 
interaction and specialization to simplify working with terminal I/O. 

Really this was a brief foray into writing such an API and was not 
really developed further, despite my love of cool CLI's. It serves 
as more of an example as a result.

There would still need to be work done in underlying terminal 
I/O features and widgets development for items like dropdown menus, 
radio boxes, etc.

Hexes or *libhexes* requires both libncurses and the libpanel curses 
extension. This should be inclusive with any modern packages of ncurses. 


## Documentation

The library API is available in html format generated via 'doxygen'. 
Simply run 'make documentation' to generate the html docs.


## Build

***Unix/Linux***:  Libhexes uses a custom build environment called 
[tcamake](https://github.com/tcarland/tcamake) to control dependencies.
Running 'make' should suffice on supported platforms.

The ncurses headers are needed for building if not already installed with the 
ncurses library. 

 * Ubuntu:      *apt-get install libncurses-dev*
 * RHEL/Centos: *yum install ncurses-devel*


