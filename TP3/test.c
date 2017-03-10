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

typedef struct{
	pthread_mutex_t *mutex;
	Point *points;
	int index;
	int nbPoint;
}Arguments;
     

void *my_thread_process(void * arg){
	
	Arguments *args = (Arguments*) arg;
	int nbPoint = args->index + args->nbPoint;
	
	
	
	g_srandom();
		
	while(1){
		
		pthread_mutex_lock((args)->mutex);
		
		int i = 0;
		
		if(getppid() == 1)
			exit(1);
			
		for(i=args->index;i<nbPoint;i++){

			args->points[i].x+=g_random()%2?1:-1;
			args->points[i].y+=g_random()%2?1:-1;
		}
		
		pthread_mutex_unlock((args)->mutex);	
		
		g_msleep(20);
		
	}

	
	exit(0);
}
     
int main(){
	
	Arguments argument[N];
	Point points[N];
	pthread_mutex_t mutex;
    int i;
    int nbCoeur;
	FILE *file;
	pthread_t th[N];
 	void *ret;
 	
 	if (pthread_mutex_init(&mutex, NULL) != 0)
    {
		printf("Pb mutex\n");
		exit(1);
    }
    
    
    if(system("echo \"$(grep -c \"processor\" /proc/cpuinfo) / $(grep \"physical id\" /proc/cpuinfo |sort -u|wc -l)\"| bc > test.txt")==-1)
		perror("system"), exit(1);

	file = fopen("test.txt","r");
	fscanf(file,"%d",&nbCoeur);
	fclose(file);


	for(i=0; i<N; ++i) {
    		points[i].x=g_random()%(G_WIDTH-200)+100;
    		points[i].y=g_random()%(G_HEIGHT-200)+100;
    		points[i].color=g_random()%G_NB_COLORS;
    	}
     
    for(i=0; i<nbCoeur; ++i) {
		
		argument[i].index = i;
		argument[i].mutex = &mutex;
		argument[i].points = points;
		argument[i].nbPoint = N/nbCoeur;
		
		if(i > 0)
			argument[i].index = argument[i-1].index + N/nbCoeur;
		else
			argument[i].index = 0;
		
		if (pthread_create (&th[i], NULL, my_thread_process, &argument[i]) < 0) {
			fprintf (stderr, "pthread_create error : %d \n", i);
			exit (1);
   			}
		}
		
	g_init();
		
   	while(1) {
		
		pthread_mutex_lock((argument)->mutex);
		
   		g_clear();
		for(i=0; i<N; ++i) 
			g_draw(points[i].x, points[i].y, points[i].color);
		
		g_flush();
		
		pthread_mutex_unlock((argument)->mutex);
		
		g_msleep(20);

	}
	
	for(i=0; i<N; ++i) {
		if(pthread_join(th[i],&ret)!=0){
			fprintf(stderr,"pthread_join error : %d \n", i);
			exit(1);
		}
	}	
	
	 pthread_mutex_destroy(&mutex);
    	
	return 0;
}

