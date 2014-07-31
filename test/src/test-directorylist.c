
#include "testp.h"

#define TESTTEXT "some text"
int main(){
	pci_init();
	/* There should now be a DATA_DIR directory and a users index */

	assert( directory_exists(DATA_DIR) == 1 );
	assert( file_exists(DATA_DIR USERS_INDEX) == 1 );
	assert( updateConversation("bob", "alice", TESTTEXT) == 1 );

	DIR * d;
	d = findConversationsWithUser("bob", "alice");

	assert(d != NULL);

	struct dirent *dir;
	while ((dir = readdir(d)) != NULL){
		printf("%s\n",  dir->d_name);
	}
	closedir(d);
	
}