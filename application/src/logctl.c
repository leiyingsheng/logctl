#include "logctl.h"

#define	LOG_FILE_PATH		"bin/log.txt"
#define DELIMITER			"\r"
#define	SYSLOG_ACTION_READ_CLEAR 		4	 	//从日志读取并清除所有消息
#define	SYSLOG_ACTION_SIZE_BUFFER 		10 		//返回内核环缓冲区大小
#define SYSLOG_BUF_SIZE					128*1024
int count = 0;

int log_fd,test_fd;
char buffer[SYSLOG_BUF_SIZE];

/*
*检查log文件是否存在
*/

void init_log_file()
{
	
	int ret;
	
	ret = access(LOG_FILE_PATH,F_OK);
	
	if(ret == -1)
	{
		
		creat(LOG_FILE_PATH,666);
		
	}
	
}

/*
*打开log文件
*
*/

int open_log_file(int mode)
{
	
	log_fd = open(LOG_FILE_PATH,mode);
	//test_fd = open("/dev/test",O_RDWR);
	
}


/*
*关闭log文件
*
*/

int close_log_file()
{
	
	//free(buffer);
	//close(test_fd);
	close(log_fd);
	
}

/*
* 获取log.txt文件大小
*/

int get_log_file_size()
{
	struct stat stbuf;
	
	int ret;
	
	ret = stat(LOG_FILE_PATH,&stbuf);
	if(ret == -1)
	{
		
		perror("stat failure");
		
		return ret;
	}
	
	return stbuf.st_size;	
	
}


/*
*
*获取klog缓存区数据
*/

int get_klog_info()
{
	
	int n;
	
	int klog_buf_len;
	
	//获取klog缓存区大小
	klog_buf_len = klogctl(SYSLOG_ACTION_SIZE_BUFFER,0,0);	
	if(klog_buf_len<=0)
	{
		return 0;
	}
	
	
	/*buffer = (char* )malloc(klog_buf_len+1);
	if(!buffer)
	{
		perror("buffer error");
		free(buffer);
		return -1;	
	}*/

	//读取klog缓存区数据
	n = klogctl(SYSLOG_ACTION_READ_CLEAR,buffer,klog_buf_len);
	if(n<0)
	{
		perror("klogctl error");
		//free(buffer);
		return -1;
	}
	
	//printf("n=%d\n",n);
	
	return n;
}


/*
*对读取到的klog信息进行处理调用write_log_file()保存到文件
*
*/


int sava_klog_info(struct config* conf)
{
	
	int klog_buf_len,next_cur = 0;
	
	 //resid_size为log文件剩余大小，cp_len为每次保存的数据长度，i:开始位置
	int resid_size,cp_len,i=0; 	
	
	while(1)
	{
		
		memset(buffer,'\0',sizeof(buffer));
		
		//klog缓存区无数据时等待
		if((klog_buf_len = get_klog_info()) > 0)
		{
			
			//打开log文件，获取文件大小
			open_log_file(O_WRONLY);
			//conf->log_file_size = get_log_file_size();	
			
			//跳转到写位置，并获取文件剩余空间
			//lseek(log_fd,conf->log_file_write_cur,SEEK_SET);
			
			i = 0;
			
			for(;klog_buf_len > 0;)
			{
				
				//获取文件剩余空间
				resid_size = conf->log_file_max_size - conf->log_file_write_cur;
				
				//跳转到写位置
				lseek(log_fd,conf->log_file_write_cur,SEEK_SET);
				
				if(resid_size < klog_buf_len)//剩余空间是否不足
				{

					cp_len = resid_size;
					
					//i = cp_len;
					
					//下次读写光标设置为文件开头
					next_cur = 0;


					klog_buf_len = klog_buf_len - resid_size;

				}else
				{

					cp_len = klog_buf_len;

					next_cur = klog_buf_len + conf->log_file_write_cur;

					//i = 0;
					
					klog_buf_len = 0;

				}
				
				write_log_file(buffer+i,cp_len);
				
				conf->log_file_write_cur = next_cur;
				
				i = cp_len;
				
			}
			
			
			//将需要保存数据拷贝到dest
			
			//保存信息，并使用内核打印出来

			
			//free(dest);
			
			conf->log_file_size = get_log_file_size();
			close_log_file();
		
			//保存配置信息
			write_config_file(conf);
		
		}
		
		usleep(10000);
	}
	
	return 0;
}


/**
*
*将数据写入文件
*
*/
int write_log_file(char *buffer ,int len)
{
	
	char dest[len];
		
	//char* p;
	
	memset(dest,'\0',sizeof(dest));
	memcpy(dest,buffer,len);
	
	write(log_fd,dest,strlen(dest));
	
	return 0;
	
	
	/*for(p = strtok(dest, DELIMITER); p ;p = strtok(NULL, DELIMITER))
	{
		if(cur != 0)
		{
			write(log_fd,"\n",strlen("\n"));			
		}
		count++;
		
		sprintf(tag,"%d",count);
		write(log_fd,tag,strlen(tag));
		write(log_fd,p,strlen(p));
		//write(test_fd,p,strlen(p));
		cur = strlen(tag)+strlen(p)+strlen("\n");
		printf("cur=%d:%s%s\n",count,tag,p);
	}*/
}











