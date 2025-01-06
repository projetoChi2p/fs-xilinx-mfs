/******************************************************************************
*
* Copyright (C) 2002 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "xilmfs.h"

struct mfs_file_block efs[200];

#define UNUSED_ARGUMENT(x) (void)(x)

int main(int argc, char *argv[]) {
  char buf[512+1];
  char buf2[512+1];
  int fdr;
  int fdw;
  int tmp;
  int num_iter;
  int total_written;
  int total_read;
  int device;

  UNUSED_ARGUMENT(argc);
  UNUSED_ARGUMENT(argv);


  const int number_of_blocks = sizeof(efs)/sizeof(efs[0]);

  printf("Size of efs: %lu\n", (long unsigned)sizeof(efs));
  printf("Size of mfs_file_block: %lu\n", (long unsigned)sizeof(struct mfs_file_block));
  printf("number_of_blocks: %d\n", number_of_blocks);
  printf("Size of mfs_dir_ent_block: %lu\n", (long unsigned)sizeof(struct mfs_dir_ent_block));
  printf("Size of mfs_dir_block: %lu\n", (long unsigned)sizeof(struct mfs_dir_block));

  mfs_init();


  device = mfs_init_fs(35*sizeof(struct mfs_file_block), (char *)efs, MFSINIT_NEW);
  if ( device != 0 ) {
    printf("device %d [%d]\n", device, __LINE__);
  }

  fdr = mfs_file_open(device, ".", MFS_MODE_READ);
  // 0-th index
  if ( fdr != 0 ) {
    printf("fdr %d [%d]\n", fdr, __LINE__);
  }

  tmp = mfs_file_read(device, fdr, &(buf[0]), 512);
  // empty read must be zero
  if ( fdr != 0 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_file_close(device, fdr);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing empty filesystem: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_create_dir(device, "testdir1");
  // 1-st index
  if ( tmp != 1 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_create_dir(device, "testdir2");
  // 2-nd index
  if ( tmp != 2 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_create_dir(device, ".");
  // must fail because invalid or existing name
  if ( tmp != MFS_ERROR_FAILED ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing 2 directory entries for cwd /*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "testdir1");
  if (tmp != MFS_SUCCESS) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_get_current_dir_name(device, buf);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }
  printf("Current dir %s: [%d]\n", buf, __LINE__);


  tmp = mfs_create_dir(device, "testdir3");
  // 3-rd index
  if ( tmp != 3 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }


  tmp = mfs_create_dir(device, "testdir1");
  // 4-th index
  if ( tmp != 4 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing 2 subdirectory entries for cwd /testdir1/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "testdir3");
  if (tmp != MFS_SUCCESS) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_get_current_dir_name(device, buf);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }
  printf("Current dir %s: [%d]\n", buf, __LINE__);


  fdw = mfs_file_open(device, "testfile1", MFS_MODE_CREATE);
  // 0-th index
  if ( fdw != 0 ) {
    printf("fdw %d [%d]\n", fdw, __LINE__);
  }

  strcpy(buf,"this is a test string|");
  total_written = 0;
  for (num_iter = 0; num_iter < 100; num_iter++) {
    tmp = mfs_file_write(device, fdw, buf, strlen(buf));
    if (tmp != MFS_SUCCESS) {
      printf("iter %d tmp %d [%d]\n", num_iter, tmp, __LINE__);
    }
    total_written += strlen(buf);
  }

  fdr = mfs_file_open(device, "testfile1", MFS_MODE_READ);
  // 1-st index
  if ( fdr != 0 ) {
    printf("fdr %d [%d]\n", fdr, __LINE__);
  }

  num_iter = 0;
  total_read = 0;
  while( (tmp = mfs_file_read(device, fdr, buf2, 512)) > 0 ){
    buf2[tmp]='\0';
    strcpy(buf, buf2);
    printf("iter %d %d '%s' [%d]\n", num_iter++, tmp, buf, __LINE__);
    total_read += tmp;
  }
  if (total_written != total_read) {
    printf("written %d != read %d [%d]\n", total_written, total_read, __LINE__);
  }

  tmp = mfs_file_close(device, fdr);
  if (tmp != MFS_SUCCESS) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_file_close(device, fdw);
  if (tmp != MFS_SUCCESS) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_file_close(device, fdw); /* should return error */
  if (tmp != MFS_ERROR_FAILED) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing 1 file entry for cwd /testdir1/testdir3/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Cat existing file: [%d]\n", __LINE__);
  tmp = mfs_cat(device, "testfile1");
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  for (num_iter=0; num_iter < 10; num_iter++) {
    strcpy(buf,"testfileA");
    buf[8] = num_iter + 'A';
    tmp = mfs_create_dir(device, buf);
    if ( tmp != ( num_iter + 10 ) ) {
      printf("num_iter %d tmp %d [%d]\n", num_iter, tmp, __LINE__);
    }
  }

  printf("Listing 1 file and multiple directory entries for cwd /testdir1/testdir3/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "testfileX");
  if ( tmp != MFS_ERROR_FAILED ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "..");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_get_current_dir_name(device, buf);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }
  printf("Current dir %s: [%d]\n", buf, __LINE__);


  tmp = mfs_change_dir(device, ".");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_get_current_dir_name(device, buf);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }
  printf("Current dir %s: [%d]\n", buf, __LINE__);


  printf("Listing 2 subdirectory entries for cwd /*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_delete_dir(device, "testfileX");
  if ( tmp != MFS_ERROR_FAILED ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_delete_dir(device, ".");
  if ( tmp != MFS_ERROR_FAILED ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_delete_file(device, "testfile1");
  if ( tmp != MFS_ERROR_FAILED ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Cat non-existing file: [%d]\n", __LINE__);
  tmp = mfs_cat(device, "testfile1"); /* should return error */
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_ERROR_FAILED ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing 2 subdirectory entries for cwd /testdir1/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }


  printf("Capturing data from stdin. Please type text. In POSIX, press CTRL-D for EOF. [%d]\n", __LINE__);
  fflush(stdout);
  tmp = mfs_copy_stdin_to_file(device, "stdinfile");
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing 1 file and 2 subdirectory entries for cwd /testdir1/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Cat: [%d]\n", __LINE__);
  tmp = mfs_cat(device, "stdinfile");
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_rename_file(device, "stdinfile", "tmp1");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_file_copy(device, "tmp1", "tmp2");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing 2 files and 2 subdirectory entries for cwd /testdir1/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Cat: [%d]\n", __LINE__);
  tmp = mfs_cat(device, "tmp2");
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_get_current_dir_name(device, buf);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }
  printf("Current dir %s: [%d]\n", buf, __LINE__);

  tmp = mfs_create_dir(device, "/testpath1");
  // 22-th entry
  if ( tmp != 22 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing 2 files and 2 subdirectory entries for cwd /testdir1/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_create_dir(device, "/testpath1/testpath2");
  // 23-th index
  if ( tmp != 23 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "/testpath1/testpath2");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_get_current_dir_name(device, buf);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }
  printf("Current dir %s: [%d]\n", buf, __LINE__);

  printf("Listing empty /testpath1/testpath2/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "/testpath1");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_get_current_dir_name(device, buf);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }
  printf("Current dir %s: [%d]\n", buf, __LINE__);

  printf("Listing 1 directory entry for /testpath1/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }


  tmp = mfs_change_dir(device, "/testpath2");
  if ( tmp != MFS_ERROR_FAILED ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "testpath2");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_create_dir(device, "/a/b/c");
  // must fail parent does no exist
  if ( tmp != MFS_ERROR_FAILED ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "/testpath2/");
  if ( tmp != MFS_ERROR_FAILED ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "/testpath1/");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing /testpath1/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "/testpath1/testpath2/");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing /testpath1/testpath2/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_create_dir(device, "testpath3/");
  // 24-th index
  if ( tmp != 24 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing /testpath1/testpath2/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }


  tmp = mfs_delete_dir(device, "/testpath1/testpath2/testpath3");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_change_dir(device, "/testpath1");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_delete_dir(device, "/testpath1/testpath2/");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing /testpath1/*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }


  tmp = mfs_change_dir(device, "/");
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_get_current_dir_name(device, buf);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }
  printf("Current dir %s: [%d]\n", buf, __LINE__);

  printf("Listing /*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  fdw = mfs_file_open(device, "testappend", MFS_MODE_CREATE);
  // 0-th index
  if ( fdw != 0 ) {
    printf("fdw %d [%d]\n", fdw, __LINE__);
  }

  strcpy(buf2,"TESTING APPEND MODE...\n");
  tmp = mfs_file_write(device, fdw, buf2, strlen(buf2));
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_file_close(device, fdw);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Cat: [%d]\n", __LINE__);
  tmp = mfs_cat(device, "testappend");
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  fdw = mfs_file_open(device, "testappend", MFS_MODE_WRITE);
  // 0-th index
  if ( fdw != 0 ) {
    printf("fdw %d [%d]\n", fdw, __LINE__);
  }

  tmp = mfs_file_lseek(device, fdw, 0, MFS_SEEK_END);
  // File contains 23 octets
  if ( tmp != 23 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  strcpy(buf2, "testing append mode2\n");
  // append at the end
  tmp = mfs_file_write(device, fdw, buf2, strlen(buf2));
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_file_close(device, fdw);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing /*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Cat: [%d]\n", __LINE__);
  tmp = mfs_cat(device, "testappend");
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }


  fdw = mfs_file_open(device, "testappend", MFS_MODE_WRITE);
  // 0-th index
  if ( fdw != 0 ) {
    printf("fdw %d [%d]\n", fdw, __LINE__);
  }

  tmp = mfs_file_lseek(device, fdw, 10, MFS_SEEK_SET);
  if ( tmp != 10 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  strcpy(buf2, "testing append mode3\n");
  // partial overwrite
  tmp = mfs_file_write(device, fdw, buf2, strlen(buf2));
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_file_lseek(device, fdw,-10, MFS_SEEK_END);
  // File contains 65 octets
  if ( tmp != 55 ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Listing /*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_file_write(device, fdw, buf2, strlen(buf2));
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  tmp = mfs_file_close(device, fdw);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  printf("Cat: [%d]\n", __LINE__);
  tmp = mfs_cat(device, "testappend");
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }


  tmp = mfs_get_current_dir_name(device, buf);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }
  printf("Current dir %s: [%d]\n", buf, __LINE__);

  printf("Listing /*: [%d]\n", __LINE__);
  tmp = mfs_ls(device);
  printf("--- end [%d]\n", __LINE__);
  if ( tmp != MFS_SUCCESS ) {
    printf("tmp %d [%d]\n", tmp, __LINE__);
  }

  return 0;
}



/* testing testing */
