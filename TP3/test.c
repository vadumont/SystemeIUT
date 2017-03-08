#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h>  
#include "pthread.h" 
#include "graphic.h"
   
#define N 200
   

typedef struct {
	int x,y,color;
} Point;

struct Arguments{
	sem_t sem;
	Point points[N];
};
     

void *my_thread_process(void * arg){
	
	g_srandom();
	
	if(getppid() == 1){
		exit(1);
	}
	
	if(sem_wait(arg.sem) == -1) 
		perror("sem_wait"),exit(1);
	
    ((Point *)arg.points)->x+=g_random()%2?1:-1;
	((Point *)arg.points)->y+=g_random()%2?1:-1;
	
	if(sem_post(arg->sem) == -1) 
		perror("sem_post"),exit(1);
		
	g_msleep(20);
	
	pthread_exit(NULL);
}
     
int main(){
	
	struct Arguments argument;
	
    int i;

	pthread_t th[N];
 	void *ret;
 	
 	if(sem_unlink("/sem1") == -1) 
	if(errno != ENOENT) perror("sem_unlink"),exit(errno);

	
	if((argument->sem = sem_open("/sem1",O_CREAT|O_EXCL, 0600,1)) == SEM_FAILED)
		perror("sem_open"),exit(1);
 	
	for(i=0; i<N; ++i) {
    		argument.points[i].x=g_random()%(G_WIDTH-200)+100;
    		argument.points[i].y=g_random()%(G_HEIGHT-200)+100;
    		argument.points[i].color=g_random()%G_NB_COLORS;
    	}
     
    	g_init();
    	while(1) {
    		for(i=0; i<N; ++i) {
				
				//Passer 2 args ???????
				
    			if (pthread_create (&th[i], NULL, my_thread_process, argument) < 0) {
      				fprintf (stderr, "pthread_create error : %d \n", i);
      				exit (1);
   			}
		}
		for(i=0; i<N; ++i) {
			if(pthread_join(th[i],&ret)!=0){
				fprintf(stderr,"pthread_join error : %d \n", i);
				exit(1);
			}
		}	
			   		
   		if(sem_wait(argument.sem) == -1) 
			perror("sem_wait"),exit(1);	
			
    	g_clear();
    	for(i=0; i<N; ++i) 
    		g_draw(argument.points[i].x, argument.points[i].y, argument.points[i].color);
     
    	g_flush();
    		
    	if(sem_post(argument.sem) == -1) 
			perror("sem_post"),exit(1);
			
    	}
    	
    	
    if(sem_unlink("/sem1") == -1) 
		perror("sem_unlink"),exit(1);
    	
	return 0;
}

