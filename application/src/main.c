#include<stdio.h>
#include"logctl.h"
#include"config.h"

void init();

int main(void)
{	
	
	struct config conf;
	
	init();
	
	//读取config文件信息
	read_config_file(&conf);
	
	//将klog信息写入log文件
	write_log_file(&conf);
	
	//保存配置信息
	write_config_file(&conf);
	
	printf("文件大小为:%d\n文件最大为:%d\n读写位置为:%d\n",
	conf.log_file_size,conf.log_file_max_size,conf.log_file_write_cur);
	
	return 0;
}


void init()
{
	init_config_file();
	init_log_file();
}