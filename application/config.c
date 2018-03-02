#include"config.h"

#define	CONFIG_FILE_PATH	"config.txt"

int config_fd;
struct config conf_init = {0,10485759,0};

/*
*
*初始化config文件
*
*/
int init_config_file()
{
	int ret;
	
	
	//检测是否存在，不存在则创建
	ret = access(CONFIG_FILE_PATH,F_OK);
	
	if(ret == -1)
	{	

		creat(CONFIG_FILE_PATH,666);
				
		write_config_file(&conf_init);
		
	}
	
	return 0;
}

/*
*
*打开配置文件
*
*/

void open_config_file(int mode)
{
	
	config_fd = open(CONFIG_FILE_PATH,mode);
	
}

/*
*
*关闭配置文件
*
*/
void close_config_file()
{
	close(config_fd);
	
}

/*
*
*写配置文件
*
*/
int write_config_file(struct config* conf)
{
	
	char str[33];
	memset(str,'\0',sizeof(str));	
	
	sprintf(str,"%d\n%d\n%d\n",
	conf->log_file_size,conf->log_file_max_size,conf->log_file_write_cur);
	
	//打开文件写入数据
	open_config_file(O_WRONLY);
	
	write(config_fd,str,strlen(str));	
	close_config_file();
	
	return 0;
}

/*
*
*读配置文件
*
*/
int read_config_file(struct config* conf)
{
	int n;
	
	char str[30];
	memset(str,'\0',sizeof(str));
	
	open_config_file(O_RDONLY);
	n = read(config_fd,str,sizeof(str));
	
	
	//读取config文件信息
	char* p;
	p = strtok(str,"\n");	
	conf->log_file_size = atoi(p);
	
	p = strtok(NULL,"\n");
	conf->log_file_max_size = atoi(p);
	
	p = strtok(NULL,"\n");
	conf->log_file_write_cur = atoi(p);
	
	close_config_file();
	return 0;
}









