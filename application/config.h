#ifndef __CONFIG_H__
#define __CONFIG_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>


struct config
{
	int log_file_size;
	int log_file_max_size;
	int log_file_write_cur;
};

int init_config_file();
void open_config_file(int mode);
void close_config_file();
int write_config_file(struct config* conf);
int read_config_file();


#endif