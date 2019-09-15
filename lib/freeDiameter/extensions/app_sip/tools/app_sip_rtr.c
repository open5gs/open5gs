/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Alexandre Westfahl <awestfahl@freediameter.net>						 *
*													 *
* Copyright (c) 2010, Alexandre Westfahl, Teraoka Laboratory (Keio University), and the WIDE Project. 	 *		
*													 *
* All rights reserved.											 *
* 													 *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:						 *
* 													 *
* * Redistributions of source code must retain the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer.										 *
*    													 *
* * Redistributions in binary form must reproduce the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer in the documentation and/or other						 *
*   materials provided with the distribution.								 *
* 													 *
* * Neither the name of the Teraoka Laboratory nor the 							 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of Teraoka Laboratory 						 *
*   													 *
* 													 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 	 *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 	 *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.								 *
*********************************************************************************************************/


// This file is separated from the source code because it is a separate command which will call registration termination function in Diameter-SIP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct rtrsipaor
{
	char username[200];
	char sip_aor1[200];
	char sip_aor2[200];
	char sip_aor3[200];
	char strreason[200];
	char desthost[200];
	int reason;
};

int main (int argc, char **argv)
{
	SOCKET sock;
    SOCKADDR_IN sin;
    struct rtrsipaor rtrsip;
    int numaor=0,i=0;
	int port=666;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_family = AF_INET;
    
    
    //We initialize the structure
    rtrsip.username[0]='\0';
    rtrsip.sip_aor1[0]='\0';
    rtrsip.sip_aor2[0]='\0';
    rtrsip.sip_aor3[0]='\0';
    rtrsip.strreason[0]='\0';
    rtrsip.desthost[0]='\0';
    rtrsip.reason=-1;
    
    
	//Start of arguments check
	if(argc<3)
	{
		fprintf(stderr,"Missing arguments! You must at least provide a username.\n");
		return 1;
	}
	
	
	for (i=1;i<argc;i++)
	{
		//We must check if it is a value or the name
		if(strncmp(argv[i],"-",1)==0)
		{
			if(strcmp(argv[i],"-u")==0)
			{
				//Username
				if(strlen(argv[i+1])<199)
				{
					strcpy(rtrsip.username,argv[i+1]);
					//We must not check the value
					i++;
				}
				else
				{
					fprintf(stderr,"Username is too long!\n");
				}
			}
			else if(strcmp(argv[i],"-a")==0)
			{
				i++;
				int j=i;
			
				for(j=i;j<argc;j++)
				{
					
					if(strncmp(argv[i],"-",1)!=0)
					{
						if(strlen(argv[i])>199)
						{
							fprintf(stderr,"SIP-AOR is too long!\n");
						}
						else if(strncmp(argv[i],"sip",3)!=0)
						{
							//Bad format of SIP-AOR
							fprintf(stderr,"A SIP-AOR must start by 'sip:' or 'sips:'. Aborting...\n");
							return 1;
						}
						else
						{
							if(numaor<3)
							{
								switch(numaor)
								{
									case 0:
										strcpy(rtrsip.sip_aor1,argv[i]);
									break;
									case 1:
										strcpy(rtrsip.sip_aor2,argv[i]);
									break;
									case 2:
										strcpy(rtrsip.sip_aor3,argv[i]);
									break;
								}
								numaor++;
							}
							else
							{
								fprintf(stderr,"You can not provide more than 3 SIP-AOR at the same time!\n");
								break;
							}
						}
						i=j+1;
					}
					else
					{
						//We have a new argument
						i--;
						break;
					}
				}
			}
			else if(strcmp(argv[i],"-r")==0)
			{
				
				if(strlen(argv[i+1])>199)
				{
					fprintf(stderr,"Deregistration reason is too long!\n");
				}
				else
				{
					strcpy(rtrsip.strreason,argv[i+1]);
				}
				i++;
			}
			else if(strcmp(argv[i],"-h")==0)
			{
				//Remote SIP Server
				if(strlen(argv[i+1])>199)
				{
					fprintf(stderr,"Host is too long!\n");
				}
				else if(strlen(argv[i+1])<5)
				{
					fprintf(stderr,"Host is too short!\n");
				}
				else
				{
					strcpy(rtrsip.desthost,argv[i+1]);
				}
				i++;
			}
			else if(strcmp(argv[i],"-pt")==0)
			{
				//Permanent Termination
				rtrsip.reason=0;
			}
			else if(strcmp(argv[i],"-nssa")==0)
			{
				//New SIP Server Assigned
				rtrsip.reason=1;
			}
			else if(strcmp(argv[i],"-ssc")==0)
			{
				//SIP Server Change
				rtrsip.reason=2;
			}
			else if(strcmp(argv[i],"-rss")==0)
			{
				//Remote SIP Server
				rtrsip.reason=3;
			}
			else if(strcmp(argv[i],"-p")==0)
			{
				
				if(sscanf(argv[i+1],"%d", &port)!=1)
				{
					fprintf(stderr,"Incorrect port number!\n");
					return 1;
				}
				
				
				i++;
			}
			else
			{
				fprintf(stderr,"Unknown argument: %s\n",argv[i]);
			}
		}
		else
		{
			fprintf(stderr,"Unknown argument: %s\n",argv[i]);
			i++;
		}
			
	}
	
	//If no SIP-AOR provided, we remove all
	if(numaor<1)
	{
		fprintf(stderr,"All SIP-AOR of %s will be deregistrated.\n",rtrsip.username);
	}
	
	//We want a username
	if(strlen(rtrsip.username)==0)
	{
		fprintf(stderr,"You must provide a username!\n");
		return 1;
	}
	
	if(rtrsip.desthost[0]=='\0')
	{
		fprintf(stderr,"You must provide the hostname of SIP-Server!\n");
		return 1;
	}
    
    //We set the port number
    sin.sin_port = htons(port);
   
    
    /*
    fprintf(stderr,"*%s*\n",rtrsip.username);
	fprintf(stderr,"*%s*\n",rtrsip.sip_aor1);
	fprintf(stderr,"*%s*\n",rtrsip.sip_aor2);
	fprintf(stderr,"*%s*\n",rtrsip.sip_aor3);
	fprintf(stderr,"*%d*\n",rtrsip.reason);
	fprintf(stderr,"*%s*\n",rtrsip.strreason);
	
	//return 0;
	*/
	
	
	//TODO: check args
	if(!connect(sock, (SOCKADDR*)&sin, sizeof(sin)))
    {
       fprintf(stderr,"Connexion succeed!\n");
        
 
        if(send(sock, &rtrsip, sizeof(struct rtrsipaor), 0))
			fprintf(stderr,"sent OK!\n");
		else
			fprintf(stderr,"not sent\n");
        
    }
    else
    {
        fprintf(stderr,"Unable to connect\n");
    }
 
    close(sock);

	return 0;
}






