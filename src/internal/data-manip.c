#include "internal/data-manip.h"

/* Code to handle low level data manipulation of files and file contents*/

int directory_exists(const char * directoryToCheck){
	DIR * dir = NULL;
	int success;

	dir = opendir(directoryToCheck);
	if ( dir ) {
		closedir(dir);
		success = 1;
	} else if ( ENOENT == errno ) success = 0; /* The directory doesn't exist */
	else success = -1;/* Something went wrong with opening it */

	return success;
}


int directory_create(const char * directoryToCheck){
	return mkdir(directoryToCheck, DATA_DIR_PERM);
}

int file_exists(const char * filename){
	/* Security Concern: If you check for a file's existence and then open the 
	 * file, between the time of access checking and creation of a file someone
	 * can create a symlink or something and cause your open to fail or open 
	 * something that shouldn't be opened. That being said... I'm not concerned.
	*/
	struct stat buffer;
	return(stat (filename, &buffer) == 0);
}

/* Check if the file was modified after the lastCheckedTime */
time_t file_last_modified_after(const char * filename, time_t lastCheckedTime){
	struct stat buffer;
	stat(filename, &buffer);
	return lastCheckedTime < buffer.st_mtime;
}

int create_data_dir(){
	int dirExists = directory_exists(DATA_DIR);
	if(dirExists < 0){
		fprintf(stderr, "%s\n", FAILED_INIT FAILED_DIR_EXISTS DATA_DIR);
		return 0; /* Unknown Failure. Panic. */
	}
	if(dirExists != 1){
		/* DATA_DIR does not exist. Create it */
		dirExists = directory_create(DATA_DIR);
		if (dirExists == -1){
			fprintf(stderr,"%s\n", FAILED_INIT FAILED_DIR_CREATION DATA_DIR);
			return 0;
		}
	}
	return 1;
}

int create_users_file(){
	if(create_data_dir() != 1) return 0;

	int userFileExists = file_exists(DATA_DIR USERS_INDEX);
	if(userFileExists == 0){
		FILE *fp = fopen(DATA_DIR USERS_INDEX, "w");
		if(!fp){
			userFileExists = 0;
			fprintf(stderr,"%s\n", FAILED_INIT FAILED_FILE_CREATION);
		}else{
			userFileExists = 1;
			fclose(fp);
		}
	}
	return userFileExists;
}

int pci_init(){
	return create_users_file();
}