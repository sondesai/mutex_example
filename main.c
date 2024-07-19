#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "log.h"

//Initialize mutexes
pthread_mutex_t mp;
pthread_mutex_t fdm;// = PTHREAD_MUTEX_INITIALIZER;

//Initialize thread arg
typedef struct list {
	int value;
	FILE *fd;
	struct list *next;
}L;

//Initialize thread functions
void inc(L *arg);
void dec(L *arg);

int main() {

	pthread_t thread1, thread2;
	int status1,status2;
	int ret;

	L node;
	node.value=1;
	node.next=NULL;

	ret = pthread_mutex_init(&mp,NULL);
	if (ret !=0) {
		log_err("Error initializing mutex mp. ret is %d:%d\n",ret,__LINE__);
		return 1;
	}
	ret = pthread_mutex_init(&fdm,NULL);
	if (ret !=0) {
		log_err("Error initializing mutex fdx. ret is %d:%d\n",ret,__LINE__);
		return 1;
	}
	pthread_create(&thread1, NULL, inc, &node);
	pthread_create(&thread2, NULL, dec, &node);

	pthread_join(thread1, &status1);
	pthread_join(thread2, &status2);


	/* Destroy a mutex to its default value */
	ret = pthread_mutex_destroy(&mp);
	if (ret !=0) {
		log_err("Error destroying mutex. ret is %d:%d\n",ret,__LINE__);
	}
	/* Destroy fdm mutex to its default value */
	ret = pthread_mutex_destroy(&fdm);
	if (ret !=0) {
		log_err("Error destroying mutex. ret is %d:%d\n",ret,__LINE__);
	}

	log_err("Mutex example done. List->values %d: and file test.txt is updated:%d\n",node.value,__LINE__);
	return 0;
}

void inc(L *arg)
{
	char str[]="thread1";
	if (pthread_mutex_lock(&mp)!=0) {
		log_err("Failed to get lock for mp in Inc():%d\n",__LINE__);
    	pthread_exit(0);
		return;
	}	
	if (pthread_mutex_lock(&fdm)!=0) {
		log_err("Failed to get lock for fdm in Inc():%d\n",__LINE__);
		if (pthread_mutex_unlock(&mp)!=0) {
			log_err("Failed to release lock for mp in Inc():%d\n",__LINE__);
		}	
   		pthread_exit(0);
		return;
	}	
 
	arg->fd = fopen("test.txt", "w+");
 
    if (arg->fd != NULL) {
		fwrite(str,sizeof(str),8,arg->fd);
		log_err("Inside fd inc Mutex lock.:%d\n",__LINE__);
    }
	arg->value=100;
	fclose(arg->fd);
	log_info("Inside Increment function after Mutex lock.:%d\n",__LINE__);
	if (pthread_mutex_unlock(&fdm)!=0) {
		log_err("Failed to release lock for fdm in Inc():%d\n",__LINE__);
	}	
	if (pthread_mutex_unlock(&mp)!=0) {
		log_err("Failed to release lock for mp in Inc():%d\n",__LINE__);
	}	
    pthread_exit(0);
}
void dec(L *arg)
{
	char str[]="thread2";
	if (pthread_mutex_lock(&fdm)!=0) {
		log_err("Failed to get lock for fdm in Dec():%d",__LINE__);
   		pthread_exit(0);
	}	
	if (pthread_mutex_lock(&mp)!=0) {
		log_err("Failed to get lock for mp in Dec():%d\n",__LINE__);
		if (pthread_mutex_unlock(&fdm)!=0) {
			log_err("Failed to release lock for mp in Dec():%d\n",__LINE__);
		}
    	pthread_exit(0);
	}
	arg->fd = fopen("test.txt", "w+");
 
    if (arg->fd != NULL) {
		fwrite(str,sizeof(str),8,arg->fd);
    }

	arg->value=200;
	fclose(arg->fd);
	log_info("Inside Decrement function after Mutex lock:%d\n",__LINE__);
	if (pthread_mutex_unlock(&mp)!=0) {
		log_err("Failed to release lock for fdm in Dec():%d\n",__LINE__);
	}	
	if (pthread_mutex_unlock(&fdm)!=0) {
		log_err("Failed to release lock for mp in Dec():%d\n",__LINE__);
	}
    pthread_exit(0);
}

