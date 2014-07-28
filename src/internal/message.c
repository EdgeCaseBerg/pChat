#include "internal/message.h"

/* Creates and places the file name for two user's converations into toFill. toFill is 
 * assuming to be a buffer of BUFFER_LENGTH*3 + DATA_DIR size at least.
*/
static void getConversationFileName(const char * userA, const char * userB, char * toFill){
	int order = strncmp(userA,userB,BUFFER_LENGTH-1);
	if(order == 0) return;

	char buffer[(BUFFER_LENGTH*3) + strlen(DATA_DIR)]; //*3 for safety
	bzero(buffer, sizeof(buffer));
	if(order < 1){
		snprintf(buffer, sizeof(buffer), "%s%s-%s", DATA_DIR, userA, userB);
	}else{
		snprintf(buffer, sizeof(buffer), "%s%s-%s", DATA_DIR, userB, userA);
	}

	char timeBuffer[BUFFER_LENGTH];
	bzero(timeBuffer, sizeof(timeBuffer));

	/* pretty much exactly from strftime's man page: */
	time_t t;
	struct tm * tmp;
	t = time(NULL);
	tmp = localtime(&t);
	if(tmp == NULL){
		fprintf(stderr, "Failed to determine local time\n");
		return;
	}
	strftime(timeBuffer, sizeof(timeBuffer), "/%F", tmp);

	if(strlen(timeBuffer) + strlen(buffer) + 1 > sizeof(buffer)){
		fprintf(stderr, "Filename too large for conversation between %s and %s\n:%s/%s\n", userA, userB, buffer, timeBuffer);
		return;
	}

	char * fileName = strncat(buffer, timeBuffer, sizeof(buffer));
	strncpy(toFill, fileName, sizeof(buffer));
}

static FILE * getConversationFileInMode(const char * userA, const char * userB, const char * mode){
	int order = strncmp(userA,userB,BUFFER_LENGTH-1);
	if(order == 0) return NULL;

	char buffer[(BUFFER_LENGTH*3) + strlen(DATA_DIR)]; //*3 for safety
	bzero(buffer, sizeof(buffer));
	getConversationFileName(userA, userB, buffer);

	if(strlen(buffer) <= 0){
		fprintf(stderr, "%s\n", "Failed to compute filename");
		return NULL;
	}

	if(file_exists(buffer) == 0){
		fprintf(stderr, "%s %s\n", FAILED_FILE_EXISTS, buffer);
		return NULL;
	}

	return fopen(buffer, mode);
}


FILE * getConversationFileForReading(const char * userA, const char * userB){
	return getConversationFileInMode(userA, userB, "r");
}

FILE * getConversationFileForWriting(const char * userA, const char * userB){
	return getConversationFileInMode(userA, userB, "a");
}

/* Note createConversation does not use getConversationFileName since it must create directories along the way */
int createConversation(const char * userA, const char * userB){
	int order = strncmp(userA,userB,BUFFER_LENGTH-1);
	if(order == 0) return 0; //You can't talk to yourself.

	char buffer[(BUFFER_LENGTH*3) + strlen(DATA_DIR)]; //*3 for safety
	bzero(buffer, sizeof(buffer));
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
	bzero(timeBuffer, sizeof(timeBuffer));

	/* pretty much exactly from strftime's man page: */
	time_t t;
	struct tm * tmp;
	t = time(NULL);
	tmp = localtime(&t);
	if(tmp == NULL){
		fprintf(stderr, "Failed to determine local time\n");
		return 0;
	}
	strftime(timeBuffer, sizeof(timeBuffer), "/%F", tmp);

	if(strlen(timeBuffer) + strlen(buffer) + 1 > sizeof(buffer)){
		fprintf(stderr, "Filename too large for conversation between %s and %s\n:%s/%s\n", userA, userB, buffer, timeBuffer);
		return 0;
	}

	char * fileName = strncat(buffer, timeBuffer, sizeof(buffer));
	if(file_exists(fileName) == 1){
		//No sense in creating it again.
		return 1;
	}

	FILE *fp = fopen(fileName, "a");
	if(!fp){
		fprintf(stderr, "%s %s\n", FAILED_FILE_CREATION, fileName);
		return 0;
	}
	fflush(fp);
	fclose(fp);
	return 1;
} 

int updateConversation(const char * userA, const char * userB, const char * addendum){
	if( createConversation(userA, userB) != 1 ) return 0;
	FILE * fp = getConversationFileForWriting(userA, userB);

	if(!fp){
		fprintf(stderr, "%s updating between %s and %s\n", FAILED_CONVERSATION_RETRIEVAL, userA, userB );
		return 0;
	}

	fprintf(fp, "%s\n", addendum);
	fflush(fp);
	fclose(fp);
	return 1;
}

int checkConversationModifiedAfter(const char * userA, const char * userB, time_t lastCheckedTime){
	char buffer[(BUFFER_LENGTH*3) + strlen(DATA_DIR)];
	bzero(buffer, sizeof(buffer));
	getConversationFileName(userA,userB, buffer);
	if(file_exists(buffer) == 0) return 0;
	else return file_last_modified_after(buffer, lastCheckedTime);
}