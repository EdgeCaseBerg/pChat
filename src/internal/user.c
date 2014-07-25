#include "internal/user.h"

static int user_file_exists(){
	return file_exists(DATA_DIR USERS_INDEX);
}

int create_user(const char * username){
	if(user_file_exists() != 1) return 0;
	if(user_exists(username) == 1) return 1;

	FILE *fp = fopen(DATA_DIR USERS_INDEX, "a");
	if(!fp){
		fprintf(stderr, "%s\n", FAILED_FILE_OPEN DATA_DIR USERS_INDEX);
		return 0;
	}

	fprintf(fp, "%s\r\n", username);
	fflush(fp);
	fclose(fp);
	return 1;
}

int user_exists(const char * username){
	if(user_file_exists() != 1) return 0;

	FILE *fp = fopen(DATA_DIR USERS_INDEX, "r");
	if(!fp){
		fprintf(stderr, "%s\n", FAILED_FILE_OPEN DATA_DIR USERS_INDEX );
		return 0;	
	}

	char user[64];
	while(fscanf(fp, "%64s\r\n", user) == 1){
		if(strncmp(user, username,  63) == 0){
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}