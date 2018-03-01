#include "logctl.h"

#define	LOG_FILE_PATH		"log.txt"
#define FILE_SIZE_MAX		10485760
#define DELIMITER			"\n"

int log_fd;
char* buffer;

/*
*打开文件
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
	
}


/*
*关闭log文件
*
*/

int close_log_file()
{
	
	free(buffer);
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
	
	klog_buf_len = klogctl(10,0,0);	
	if(klog_buf_len<=0)
	{
		klog_buf_len=(1<<18);
	}
	
	
	buffer = (char* )malloc(klog_buf_len+1);
	if(!buffer)
	{
		perror("buffer error");
		free(buffer);
		return -1;	
	}

	
	n = klogctl(3,buffer,klog_buf_len);
	if(n<0)
	{
		perror("klogctl error");
		free(buffer);
		return -1;
	}
	
	return n;
}


/*
*log保存到log.txt
*/

int write_log_file(struct config * conf)
{
	//klog_filter_state = KLOG_START;
	
	char* p;
	char* dest,* src;
	int n,klog_buf_len,resid_size,cp_len,i=0;
	
	int flag;
	
	open_log_file(O_WRONLY);
	
	conf->log_file_size = get_log_file_size();
	
	klog_buf_len = get_klog_info();
	
	printf("klog_buf_len = %d\n",klog_buf_len);
	
	
	while(klog_buf_len > 0)
	{
		
		
		//cp_len = resid_size < klog_buf_len?resid_size:klog_buf_len;
		
		lseek(log_fd,conf->log_file_write_cur,SEEK_SET);
		
		resid_size = conf->log_file_max_size - conf->log_file_write_cur;
		
		if(resid_size < klog_buf_len)
		{
			
			cp_len = resid_size;
			
			conf->log_file_write_cur = 0;
			
			
			conf->log_file_size = conf->log_file_max_size;
			
			klog_buf_len = klog_buf_len - resid_size;
			
			
		}else
		{
			
			cp_len = klog_buf_len;
			
			conf->log_file_write_cur += klog_buf_len;
			
			if(conf->log_file_size != conf->log_file_max_size+1)
			{
				conf->log_file_size = conf->log_file_write_cur;
			}
			
			klog_buf_len = 0;
			
		}
		
		dest = (char*)malloc(cp_len);
		
		memset(dest,'\0',sizeof(dest));
		memcpy(dest,buffer+i,cp_len);
		
		for(p = strtok(dest, DELIMITER); p ;p = strtok(NULL, DELIMITER))
		{
			
			n = write(log_fd,p,strlen(p));
			write(log_fd,"\n",strlen("\n"));
			
		}
		
		free(dest);

		if(klog_buf_len > 0)
		{
			printf("%d\n",i);
			i=resid_size;
		}
		
	}
	
	close_log_file();
	
	return 0;
}
