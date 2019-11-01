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


// This file is separated from the source code because it is a separate command which will call push profile function in Diameter-SIP
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

struct pprsipaor
{
	char username[200];
	char datatype1[255];
	char datatype2[255];
	char datatype3[255];
	int accounting;
};

int main (int argc, char **argv)
{
	SOCKET sock;
    SOCKADDR_IN sin;
    struct pprsipaor pprsip;
    int numdatatype=0,i=0;
    int port=667;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_family = AF_INET;
    
    
    //We initialize the structure
    pprsip.username[0]='\0';
    pprsip.datatype1[0]='\0';
    pprsip.datatype2[0]='\0';
    pprsip.datatype3[0]='\0';
    pprsip.accounting=0;
    
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
					strcpy(pprsip.username,argv[i+1]);
					//We must not check the value
					i++;
				}
				else
				{
					fprintf(stderr,"Username is too long!\n");
				}
			}
			else if(strcmp(argv[i],"-t")==0)
			{//User-data types
				i++;
				int j=i;
			
				for(j=i;j<argc;j++)
				{
					
					if(strncmp(argv[i],"-",1)!=0)
					{
						if(strlen(argv[i])>254)
						{
							fprintf(stderr,"User-data type is too long!\n");
						}
						else
						{
							if(numdatatype<3)
							{
								switch(numdatatype)
								{
									case 0:
										strcpy(pprsip.datatype1,argv[i]);
									break;
									case 1:
										strcpy(pprsip.datatype2,argv[i]);
									break;
									case 2:
										strcpy(pprsip.datatype3,argv[i]);
									break;
								}
								numdatatype++;
							}
							else
							{
								fprintf(stderr,"You can not provide more than 3 User-data type at the same time!\n");
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
			else if(strcmp(argv[i],"-a")==0)
			{
				pprsip.accounting=1;
				
				
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
			
		}
		else
		{
			fprintf(stderr,"Unknown argument: %s\n",argv[i]);
			i++;
		}
			
	}
	
	//We set the port number
	sin.sin_port = htons(port);
	
	
	//We want a username
	if(strlen(pprsip.username)==0)
	{
		fprintf(stderr,"You must provide a username!\n");
		return 1;
	}
	
    
    fprintf(stderr,"%d!\n",port);
   
    /*
    //DEBUG
    fprintf(stderr,"*%s*\n",pprsip.username);
	fprintf(stderr,"*%s*\n",pprsip.datatype1);
	fprintf(stderr,"*%s*\n",pprsip.datatype2);
	fprintf(stderr,"*%s*\n",pprsip.datatype3);
	fprintf(stderr,"*%d*\n",pprsip.accounting);
	
	return 0;
	*/
	
	
	//TODO: check args
	if(!connect(sock, (SOCKADDR*)&sin, sizeof(sin)))
    {
       fprintf(stderr,"Connexion succeed!\n");
        
 
        if(send(sock, &pprsip, sizeof(struct pprsipaor), 0))
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






