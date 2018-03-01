#include<stdio.h>
#include"logctl.h"
#include"config.h"

int main(void)
{	
	
	struct config conf;
	
	init_config_file();
	read_config_file(&conf);
	
	init_log_file();
	
	write_log_file(&conf);
	
	write_config_file(&conf);
	
	printf("%d,%d,%d\n",
	conf.log_file_size,conf.log_file_max_size,conf.log_file_write_cur);
	
	printf("%d\n",get_log_file_size());
	
	return 0;
}