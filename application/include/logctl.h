#ifndef __LOGCTL__
#define __LOGCTL__


#include<sys/klog.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include"config.h"

void init_log_file();
int open_log_file(int mode);
int close_log_file();
int get_klog_info();
int get_log_file_size();
int sava_klog_info(struct config * conf);
int write_log_file(char *buffer ,int len);

#endif
