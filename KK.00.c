#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>

struct header {
	unsigned int signalType : 4;
	unsigned int signalSize : 12;
};

struct connectionDownlinkDataDetails {
	unsigned int connectionId : 32; 
	unsigned int mcs : 7;
	unsigned int tbs : 5;
	unsigned int dataSize : 32;
};

struct connectionRequest {
	struct header header; // header headerCR // struct struct header
	unsigned int connectionId : 32;
	unsigned int configPreset : 4;
	unsigned int signalStrength : 10;
};

struct downlinkData {
	struct header header;
};

struct signalPtrs {
	struct connectionRequest * ptr1;
	struct downlinkData * ptr4;
};

void *read_input(struct signalPtrs * ptr)
{
	FILE* inputFile = fopen("input.in", "r");
	struct header inputHeader;
	unsigned int tmp1, tmp2;
	int result = fscanf(inputFile, "%d %d", &tmp1, &tmp2);
	inputHeader.signalType=tmp1; inputHeader.signalSize=tmp2;
	if(result!=2){
		return NULL;
	}
	ptr = malloc(inputHeader.signalSize);
	if(ptr == NULL){
		return NULL;
	}
	if(inputHeader.signalType==1){
		unsigned int tmp3;
		int result = fscanf(inputFile, "%d %d %d", &tmp1, &tmp2, &tmp3);
		if (result!=3){
			return NULL;
		}
		struct connectionRequest *cr = malloc(inputHeader.signalSize);
		cr->header=inputHeader;
		cr->connectionId=tmp1;
		cr->configPreset=tmp2;
		cr->signalStrength=tmp3;
		ptr->ptr1 = cr;
		ptr->ptr4 = NULL;
	}else if(inputHeader.signalType==4){
		unsigned int noOfConnections;
		unsigned int tmp3, tmp4;
		int result = fscanf(inputFile, "%d", &noOfConnections);
		if (result!=1 || noOfConnections >15){
			return NULL;
		}
		struct downlinkData *dd = malloc(inputHeader.signalSize);
		dd->header=inputHeader;
		dd->noOfConnections=noOfConnections;
		struct connectionDownlinkDataDetails connectionDownlinkDataDetails[noOfConnections];
		for(int i = 0; i<noOfConnections; i++){
			result = fscanf(inputFile, "%d %d %d %d", &tmp1, &tmp2, &tmp3, &tmp4);
			dd->connectionDownlinkDataDetails[i].connectionId=tmp1;
			dd->connectionDownlinkDataDetails[i].mcs=tmp2;
			dd->connectionDownlinkDataDetails[i].tbs=tmp3;
			dd->connectionDownlinkDataDetails[i].dataSize=tmp4;
			if (result!=4){
				return NULL;
			}
		}
		ptr->ptr4 = dd;
		ptr->ptr1 = NULL;
	}

	fclose(inputFile);
    return NULL;
}

void *write_output(struct signalPtrs * ptr) 
{
	FILE* outputFile = fopen("output.out", "w");
	
	if(ptr->ptr1!=NULL){
		struct header h= ptr->ptr1->header;
		
		fprintf(outputFile, "{");
		fprintf(outputFile, "\n \"ConnectionRequest\": {");
		fprintf(outputFile, "\n  \"header\": {");
		fprintf(outputFile, "\n   \"signalType\": %d", 8);
		fprintf(outputFile, ",\n   \"signalSize\": %d", 88);
		fprintf(outputFile, "\n  },");
		fprintf(outputFile, "\n  \"connectionId\": %d", 14);
		fprintf(outputFile, ",\n  \"configPreset\": %d", 15);
		fprintf(outputFile, ",\n  \"signalStrength\": %d", 16);
		fprintf(outputFile, "\n }");
		fprintf(outputFile, "\n}\n\n");
		
	}else if(ptr->ptr4!=NULL){    
			
		fprintf(outputFile, "{");
		fprintf(outputFile, "\n \"DownlinkData\": {");
		fprintf(outputFile, "\n  \"header\": {"); 
		fprintf(outputFile, "\n   \"signalType\": %d", 8);
		fprintf(outputFile, ",\n   \"signalSize\": %d", 88);
		fprintf(outputFile, "\n  },");
		
		fprintf(outputFile, "\n \"noOfConnection\": %d", 14);
		fprintf(outputFile, ",\n \"connectionDownLinkDataDetails\": [");
		
		int noc=2;
		for(int i=0; i<noc; i++){
			fprintf(outputFile, "\n  {"); 
			fprintf(outputFile, "\n   \"connectionId\": %d", 14);
			fprintf(outputFile, ",\n   \"mcs\": %d", 15);
			fprintf(outputFile, ",\n   \"tbs\": %d", 16);
			fprintf(outputFile, ",\n   \"dataSize\": %d", 16);
			fprintf(outputFile, "\n  },");
		}
		fprintf(outputFile, "\n ]");
		fprintf(outputFile, "\n}\n");
		
	}
		
		
		
	fclose(outputFile);
    return NULL;
}

int main(){
	struct signalPtrs * ptr = malloc(sizeof(struct signalPtrs));
	
	
	pthread_t inputThread; //zadeklarowanie wątku inputThread
	pthread_create(&inputThread, NULL, read_input, ptr); //Wykonanie wątku (wywołanie wątku inputThread jako funkcja wykonaj1)
	
	pthread_join(inputThread, NULL); //Zakończenie wątku (czekanie, aż wątek zakończy działanie, aby uniknąć błędów pamięci)
	
	pthread_t outputThread;
	pthread_create(&outputThread, NULL, write_output, ptr);
	pthread_join(outputThread, NULL);
	
	
	
	return 0;

}
