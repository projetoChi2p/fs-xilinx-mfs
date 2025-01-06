# Xilinx Memory Filesystem (xilmfs)

This source code is based on Xilinx Memory Filesystem version 2.3. The support for Xilinx Memory Filesystem was discontinued by Xilinx.

Xilinx Memory Filesystem (MFS) is an in-memory filesystem, or RAMDISK, similar to RTEMS In-Memory FileSystem (IMFS), although not POSIX full-featured or compliant.

A important feature of MFS is that the memory containing the file system can be statically allocated, in contrast to other memory filesystems which use dynamic allocation.

The byte vector used by MFS is internally divided in blocks, which always contain control flags and either directory and file metadata or either file raw data content.

See also the [filesystem documentation](src/readme.txt) and [utilities documentation](src/utils/readme.txt).
