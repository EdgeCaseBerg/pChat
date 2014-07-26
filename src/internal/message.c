#include "internal/message.h"

int createConversation(const char * userA, const char * userB){
	int order = strncmp(userA,userB,BUFFER_LENGTH-1);
	if(order == 0) return 0; //You can't talk to yourself.

	char buffer[(BUFFER_LENGTH*3) + strlen(DATA_DIR)]; //*3 for safety
	bzero(buffer);
	if(order < 1){
		snprintf(buffer, sizeof(buffer), "%s%s-%s", DATA_DIR, userA, userB);
	}else{
		snprintf(buffer, sizeof(buffer), "%s%s-%s", DATA_DIR, userB, userA);
	}

	int existsAlready = directory_exists(buffer);
	if(existsAlready == -1){
		fprintf(stderr, "%s%s\n", FAILED_DIR_EXISTS, buffer);
		return 0;
	}

	if(existsAlready != 1){	
		if( directory_create(buffer) != 0 ){
			fprintf(stderr, "%s %s\n", FAILED_DIR_CREATION,  buffer);
			return 0;
		}
	}

	/* Conversation Directory Exists, Order conversations by time */
	char timeBuffer[BUFFER_LENGTH];
	bzero(timeBuffer);

	/* pretty much exactly from strftime's man page: */
	time_t t;
	struct tm * tmp;
	t = time(NULL);
	tmp = localtime(&t);
	if(tmp == NULL){
		fprintf(stderr, "Failed to determine local time\n");
		return 0;
	}
	strftime(timeBuffer, sizeof(timeBuffer), "%F", tmp);

	if(strlen(timeBuffer) + strlen(buffer) + 1 > sizeof(buffer)){
		fprintf(stderr, "Filename too large for conversation between %s and %s\n:%s/%s\n", userA, userB, buffer, timeBuffer);
		return 0;
	}

	char * fileName = strncat(buffer, timeBuffer, sizeof(buffer));
	if(file_exists(fileName) == 1){
		//No sense in creating it again.
		return 1;
	}

	FILE *fp = fopen(fileName, "r");
	if(!fp){
		fprintf(stderr, "%s %s\n", FAILED_FILE_CREATION, fileName);
		return 0;
	}
	fclose(fp);
	return 1;
} 