![Screenshot](https://github.com/Dudi119/AOS/blob/master/AOS_Title.png)

AOS is a hobby unikenerl written in modern C++.
## Main properties:
1. Based upon c++14 (no exceptions, rtti, atomics, threads and fences are supported yet).
2. No virtual memory concept, only physical memory is supported for now.
3. Limited to 32 bit.

In order to build the project, run the cmake script:
```
mkdir build && cd build
cmake ..
make
```

qemu can be used to run the image
```
qemu-system-i386 -kernel aos.bin
```
