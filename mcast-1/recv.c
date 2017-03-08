/*
 * recv.c
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
#include <netdb.h>      /* pour gethostbyname*/
#include <string.h>     /* pour memset */
#include <ctype.h>     /* pour toupper */
 
#define DGRAM_MAX 1024  /* taille MAX en réception */
#define GROUP "230.0.0.0"
#define PORT 5000

int main(int argc, char **argv)
{
	struct sockaddr_in addr, src_addr;
	int sd, ret;
	socklen_t len_src_addr;
	struct ip_mreqn mreqn;
	
	char request[DGRAM_MAX];
	
 
	if(inet_aton(GROUP, &mreqn.imr_multiaddr)==0) {
		fprintf(stderr,"Pb Adr multicast : %s !\n", GROUP);
	exit(1);
	}
 
	mreqn.imr_address.s_addr = htonl(INADDR_ANY);
	mreqn.imr_ifindex=0; /* n'importe quelle interface */
 
 
	/* Création de la socket IPv4 */
	if((sd=socket(AF_INET, SOCK_DGRAM, 0))==-1) {
		perror("socket"); exit(1);
	}
 
 
 	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
		(void *) &mreqn, sizeof(struct ip_mreq)) < 0)
		perror("setsockopt – IP_ADD_MEMBERSHIP");
		
	/* Attachement de la socket */
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl (INADDR_ANY);
	addr.sin_port = htons(PORT);
	memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));
 
	if(bind(sd, (struct sockaddr *)&addr, sizeof addr)) {
		perror("bind"); exit(1);
	}		
	
	while(1) {
 
		/* Attente et lecture d'une requête */
		len_src_addr=sizeof src_addr;
		if((ret=recvfrom(sd, request, DGRAM_MAX-1, 0,
			(struct sockaddr*) &src_addr, &len_src_addr))==-1) {
		perror("recvfrom"); exit(1);
		}
	
		puts(request);
	
		
	}
	
	if(setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		&mreqn, 	sizeof(mreqn)) == -1)
		perror("setsockopt: IP_DROP_MEMBERSHIP");
		
	return 0;
}
