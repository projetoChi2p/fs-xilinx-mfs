# Xilinx Memory Filesystem (xilmfs)

This source code is based on Xilinx Memory Filesystem version 2.3. Support for the Xilinx Memory Filesystem was discontinued by Xilinx.

The Xilinx Memory Filesystem (MFS) is an in-memory filesystem, or RAMDISK, similar to the RTEMS In-Memory FileSystem (IMFS), although it is not designed to be fully POSIX-compliant or feature-complete.

An important feature of MFS is that the memory containing the filesystem can be statically allocated, in contrast to other memory filesystems that use dynamic allocation.

The byte vector used by MFS is internally divided into blocks, each of which contains control flags and either directory/file metadata or raw file data content. The MFS blocks are managed, internally by MFS, as linked lists. Each file is a doubly linked list of raw file data content blocks. Each file entry in directory points to the first block of the file. All the directory/file metadata blocks are also kept as linked list. Finally, an additional linked list keeps an inventory of free blocks.

![Simplified MFS organization](xilmfs.svg)

A Xilinx utility named `mfsgen`, distributed with Vivado Design Suite, allows the creation of a filesystem image pre-populated with user-provided files. The sintax of `mfsgen` is similar to the `tar` utility. The filesystem image can then be converted to source code, for instance in the form of an array of bytes to be compiled and linked together with the final embedded application. The filesystem image can be used either as read-only, e.g. linked/stored in Flash, or as read-write, stored in RAM. For compliance with and image created by `mfsgen` utility, MFS must be compiled with very specific parameters of size of data blocks, filename length, number of entries per directory block, etc.

See also the [filesystem documentation](src/readme.txt) and [utilities documentation](src/utils/readme.txt).
