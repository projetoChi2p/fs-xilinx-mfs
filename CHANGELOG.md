# Changelog

## Development Build: v2.3+
- Bugfix: in mfs_file_lseek() the return when (offset == 0) was removed so that other control operations can take place.
- Definition of macros in header file, like MFS_BLOCK_DATA_SIZE, MFS_MAX_FILENAME_LENGTH, etc was modified to allow parameterization during build.
- To simplify handling of multiple filesystems, global variables were encapsulated in an array of mfs_filesystem_struct "devices". The device number is returned upon sucessful mfs_init_fs() and must be passed on most filesystem operations.
