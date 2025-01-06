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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "xilmfs.h"

#define UNUSED_ARGUMENT(x) (void)(x)

char *fs ; // = (char *)0xffe00000; /* base address of SRAM */

int main(int argc, char *argv[]) {


  UNUSED_ARGUMENT(argc);
  UNUSED_ARGUMENT(argv);

  int numbytes;
  char buffer[4];
  int fd;
  int device;
  int i;

  numbytes = 1000 *sizeof(struct mfs_file_block);

  fs = (char *)malloc(numbytes);

  mfs_init();

  device = mfs_init_fs(numbytes, fs, MFSINIT_NEW);
  mfs_create_dir(device, "dir1");
  mfs_change_dir(device, "dir1");
  mfs_create_dir(device, "dir1_1");
  mfs_change_dir(device, "dir1_1");
  mfs_create_dir(device, "dir1_1_1");
  fd = mfs_file_open(device, "file1", MFS_MODE_CREATE);
  mfs_file_write(device, fd, "abcd", 4);
  mfs_file_close(device, fd);
  mfs_change_dir(device, "..");
  fd = mfs_file_open(device, "file2", MFS_MODE_CREATE);
  mfs_file_write(device, fd, "efgh", 4);
  mfs_file_close(device, fd);
  fd = mfs_file_open(device, "file2",  MFS_MODE_READ);
  if (mfs_file_read(device, fd, buffer, 4) != 4) {
    printf("1");
  }
  else  {
    printf("0");
  }
  mfs_file_close(device, fd);
  fd = mfs_file_open(device, "file3",  MFS_MODE_CREATE);
  for(i = 0; i < 512; i++)
   mfs_file_write(device, fd, "a", 1);
  mfs_file_close(device, fd);
  fd = mfs_file_open(device, "file3", MFS_MODE_READ);
  i = mfs_file_lseek(device, fd, 0, SEEK_END);
  if (i != -1) {
    i = mfs_file_lseek(device, fd, 1, SEEK_CUR);
    if (i == -1) {
      i = mfs_file_lseek(device, fd, 0, SEEK_SET);
      if (i != -1) {
        i = mfs_file_lseek(device, fd, 1, SEEK_CUR);
        if (i != -1) {
          i = mfs_file_lseek(device, fd, 10, SEEK_CUR);
          if (i != -1) {
            i = mfs_file_lseek(device, fd, 20, SEEK_SET);
          }
        }
      }
    }
  }
  mfs_file_close(device, fd);
  fd = mfs_file_open(device, "file4", MFS_MODE_CREATE);
  for (i = 0; i < 513; i++)
    mfs_file_write(device, fd, "b", 1);
  mfs_file_close(device, fd);
  fd = mfs_file_open(device, "file4", MFS_MODE_READ);
  i = mfs_file_lseek(device, fd, 0, SEEK_END);
  if (i != -1) {
    i = mfs_file_lseek(device, fd, -1, SEEK_CUR);
    if (i != -1) {
      i = mfs_file_lseek(device, fd, 0, SEEK_SET);
      if (i != -1) {
        i = mfs_file_lseek(device, fd, 500, SEEK_CUR);
        if (i != -1) {
          i = mfs_file_lseek(device, fd, 12, SEEK_CUR);
          if (i != -1) {
            i = mfs_file_lseek(device, fd, 513, SEEK_SET);
          }
        }
      }
    }
  }
  mfs_file_close(device, fd);
  fd = mfs_file_open(device, "file5", MFS_MODE_CREATE);
  for (i=0; i < 1024; i++)
    mfs_file_write(device, fd,"c",1);
  mfs_file_close(device, fd);
  fd = mfs_file_open(device, "file6", MFS_MODE_CREATE);
  for (i =0; i < 1025; i++)
    mfs_file_write(device, fd, "d", 1);
  mfs_file_close(device, fd);
  fd = mfs_file_open(device, "file6", MFS_MODE_READ);
  i = mfs_file_lseek(device, fd, 0, SEEK_END);
  if (i != -1) {
    i = mfs_file_lseek(device, fd, -1, SEEK_CUR);
    if (i != -1) {
      i = mfs_file_lseek(device, fd, 0, SEEK_SET);
      if (i != -1) {
        i = mfs_file_lseek(device, fd, 700, SEEK_CUR);
        if (i != -1) {
          i = mfs_file_lseek(device, fd, -250, SEEK_CUR);
          if (i != -1) {
            i = mfs_file_lseek(device, fd, 1025, SEEK_SET);
          }
        }
      }
    }
  }
  fd = mfs_file_open(device, "file7", MFS_MODE_CREATE);
  for (i =0; i < 5000; i++)
    mfs_file_write(device, fd, "e", 1);
  mfs_file_close(device, fd);
  mfs_change_dir(device, "..");
  mfs_change_dir(device, "..");
  mfs_ls_r(device, -1);
{
  int fd;
  char c;
  long l;
  fd = mfs_file_open(device, "file1.tmp", MFS_MODE_CREATE);
  if  (fd == -1) {
    // printf("error opening the file %d\n", errno);
    exit(1);
  }
  for (c= 0; c < 127; c++) {
    mfs_file_write(device, fd, &c, 1);
  }
  mfs_file_close (device, fd);
  fd = mfs_file_open(device, "file1.tmp", MFS_MODE_READ);
  l = mfs_file_lseek (device, fd, 0, SEEK_SET);
  printf("l (0) %ld\n",  l);
  mfs_file_read(device, fd, &c, 1);
  printf("c (0) %d\n", c);
  l = mfs_file_lseek (device, fd, 10, SEEK_CUR);
  printf("l (11) %ld\n", l);
  mfs_file_read(device, fd, &c, 1);
  printf("c (11) %d\n", c);
  l = mfs_file_lseek (device, fd, 20, SEEK_CUR);
  printf("l (32) %ld\n", l);
  mfs_file_read(device, fd, &c, 1);
  printf("c (32) %d\n", c);
  mfs_file_read(device, fd, &c, 1);
  printf("c (32) %d\n", c);
  l = mfs_file_lseek (device, fd, 0, SEEK_END);
  printf("l (127) %ld\n", l);
  mfs_file_read(device, fd, &c, 1);
  printf("c (-1) %d\n", c);
}

 return 0;
}


