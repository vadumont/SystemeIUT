#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h>  
  
#include "graphic.h"
   
#define N 200
     
typedef struct {
  	int x,y,color;
} Point;
     
     
void fils(sem_t *sem,Point *points,int i){
	
	g_srandom();
	
	while(1){
		
		if(getppid() == 1){
			exit(1);
		}
		
		if(sem_wait(sem) == -1) 
			perror("sem_wait"),exit(1);
		

   		points[i].x+=g_random()%2?1:-1;
   		points[i].y+=g_random()%2?1:-1;
   		
   		if(sem_post(sem) == -1) 
			perror("sem_post"),exit(1);
		
		
		g_msleep(20);
	}
	exit(0);
	
}
     

int main() {
	
  	Point *points;
   	int i;
    sem_t *sem;
    int shmfd;
    
   	
    if(sem_unlink("/sem1") == -1) 
		if(errno != ENOENT) perror("sem_unlink"),exit(errno);
   	
   	if(shm_unlink("/shmem") == -1)
		if(errno != ENOENT) perror("shm_unlink"),exit(errno);

	
	if((sem = sem_open("/sem1",O_CREAT|O_EXCL, 0600,1)) == SEM_FAILED)
		perror("sem_open"),exit(1);
		
   	
   	if((shmfd=shm_open("/shmem",O_RDWR|O_CREAT|O_EXCL, 0600)) == -1) {
		perror("shm_open"),exit(1);
	}
	
	
	if(ftruncate(shmfd,N * sizeof(Point)))
		perror("fseek"), exit(1);
	
	if ((points=(Point *)mmap(0,N * sizeof(Point),
        PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0)) == MAP_FAILED)
        perror("mmap"), exit(1);

    g_init();
    

    
    for(i=0; i<N; ++i) {
   		points[i].x=g_random()%(G_WIDTH-200)+100;
   		points[i].y=g_random()%(G_HEIGHT-200)+100;
   		points[i].color=g_random()%G_NB_COLORS;
   	}
   	
   	
   	for(i=0; i<N-1;i++) {
		
		switch(fork()){
			
			case -1 : 
				perror("fork");
				return -1;
				
			case 0 : 
				
				fils(sem,points,i);
				break;
		}	
		
	}
   	
   	while(1) {
   		
   		if(sem_wait(sem) == -1) 
			perror("sem_wait"),exit(1);
		
     
   		g_clear();
   		for(i=0; i<N; ++i) 
   			g_draw(points[i].x, points[i].y, points[i].color);
		
		g_flush();
			
				
		if(sem_post(sem) == -1) 
			perror("sem_post"),exit(1);
		
		
		g_msleep(20);	
	}
	
	wait(NULL);
	
	if(munmap(points,N * sizeof(Point)))
		perror("unmap"),exit(1);
	
	
	if(shm_unlink("/shmem") == -1) 
		perror("unlink"),exit(1);
	
	
	if(sem_unlink("/sem1") == -1) 
		perror("sem_unlink"),exit(1);
	
     
    return 0;
}

