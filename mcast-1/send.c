/*
 * send.c
 * 
 * Copyright 2017 Valentin Dumont <vadumont2@b12-233>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <errno.h>
 
#include <netdb.h>      /* pour getaddrinfo*/
#include <string.h>     /* pour memset */
 
#include <arpa/inet.h>  /* pour inet_ntop */
 
#define DGRAM_MAX 1024  /* taille MAX en réception */
#define GROUP "230.0.0.0"
#define PORT 5000

void readingLine(char *final)
{
	char login[DGRAM_MAX];
	char msg[DGRAM_MAX];
	char *positionEntree = NULL;
	printf("\nVeuillez saisir ce que vous souhaitez envoyer sur le canal multicast :\n\n");
	if(fgets(msg,DGRAM_MAX,stdin) != NULL)
	{
		positionEntree = strchr(msg,'\n');
		if(positionEntree != NULL)
		{
			*positionEntree = '\0';
		}
		else {
		fprintf(stderr,"Erreur lors du fgets\n");
		exit(1);
		}	
	}	
		
	if((getlogin_r(login,15)) == -1)
		perror("getlogin_r"),exit(1);
		
	strcat(final,"[");
	strcat(final, login);
	strcat(final,"] : ");
	strcat(final,msg);
	
}


int main(int argc, char **argv) {
	
	struct sockaddr_in dst_addr, src_addr;	
	struct hostent * hostent;
	int s, ret;
	socklen_t len_src_addr;
	
	char final[DGRAM_MAX];
	
	if((inet_aton(GROUP,&dst_addr.sin_addr))==0){
		fprintf(stderr,"Erreur sur inet_aton\n");
		exit(1);
	}

	
	/* Création de la socket IPv4 */
	if((s=socket(AF_INET, SOCK_DGRAM, 0))==-1) {
		perror("socket"); exit(1);
	}
	
	
	/* Remplissage de l'adresse du destinataire */
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_port = htons(PORT);  
	memset(dst_addr.sin_zero, '\0', sizeof(dst_addr.sin_zero));
 
	while(1)
	{
		memset(final,0,sizeof(final));
		readingLine(final);
		
		
		/* Émission du datagramme */
		if(sendto(s, final, strlen(final)+1, 0, \
	   (struct sockaddr*) &dst_addr, sizeof(dst_addr))==-1) {
		perror("sendto"); exit(1);
		}
	}
	
	return 0;
}
