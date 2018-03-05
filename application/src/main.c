#include<stdio.h>
#include"logctl.h"
#include"config.h"

void init();
void create_daemon();
int main(void)
{	
	
	create_daemon();
	
	struct config conf;
	
	init();
	
	//读取config文件信息
	read_config_file(&conf);
	
	//将klog信息写入log文件
	sava_klog_info(&conf);
	
	printf("文件大小为:%d\n文件最大为:%d\n读写位置为:%d\n",
	conf.log_file_size,conf.log_file_max_size,conf.log_file_write_cur);
	
	return 0;
}



void create_daemon()
{
	pid_t pid;
	pid = fork();
	
	if(-1 == pid)
	{
		exit(1);
	}else if(pid)
	{
		exit(0);
	}
	
	if(-1 == setsid())
	{
		exit(1);
	}
	
	pid = fork();
	
	if(-1 == pid)
	{
		exit(1);
	}else if(pid)
	{
		exit(0);
	}
	
	chdir("~");
	umask(0);
	
}

void init()
{
	init_config_file();
	init_log_file();
}