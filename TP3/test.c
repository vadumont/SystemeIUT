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

void *my_thread_process(void * arg,sem_t *sem){
	
	g_srandom();
	
	if(getppid() == 1){
		exit(1);
	}
	
	if(sem_wait(sem) == -1) 
		perror("sem_wait"),exit(1);
	
    ((Point *)arg)->x+=g_random()%2?1:-1;
	((Point *)arg)->y+=g_random()%2?1:-1;
	
	if(sem_post(sem) == -1) 
		perror("sem_post"),exit(1);
		
	g_msleep(20);
	
	pthread_exit(NULL);
}
     
int main() {
	Point points[N];
    int i;
    sem_t *sem;
	pthread_t th[N];
 	void *ret;
 	
 	if(sem_unlink("/sem1") == -1) 
	if(errno != ENOENT) perror("sem_unlink"),exit(errno);

	
	if((sem = sem_open("/sem1",O_CREAT|O_EXCL, 0600,1)) == SEM_FAILED)
		perror("sem_open"),exit(1);
 	
	for(i=0; i<N; ++i) {
    		points[i].x=g_random()%(G_WIDTH-200)+100;
    		points[i].y=g_random()%(G_HEIGHT-200)+100;
    		points[i].color=g_random()%G_NB_COLORS;
    	}
     
    	g_init();
    	while(1) {
    		for(i=0; i<N; ++i) {
				
				//Passer 2 args ???????
				
    			if (pthread_create (&th[i], NULL, my_thread_process, &points[i],sem) < 0) {
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
			   		
   		if(sem_wait(sem) == -1) 
			perror("sem_wait"),exit(1);	
			
    	g_clear();
    	for(i=0; i<N; ++i) 
    		g_draw(points[i].x, points[i].y, points[i].color);
     
    	g_flush();
    		
    	if(sem_post(sem) == -1) 
			perror("sem_post"),exit(1);
			
    	}
    	
    	
    if(sem_unlink("/sem1") == -1) 
		perror("sem_unlink"),exit(1);
    	
	return 0;
}

