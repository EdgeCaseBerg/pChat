#include "testp.h"
#include <unistd.h>

#define TESTTEXT "some text"
int main(){
	pci_init();
	/* There should now be a DATA_DIR directory and a users index */

	assert( directory_exists(DATA_DIR) == 1 );
	assert( file_exists(DATA_DIR USERS_INDEX) == 1 );

	time_t t;
	struct tm * tmp;
	t = time(NULL);
	tmp = localtime(&t);
	if(tmp == NULL){
		fprintf(stderr, "Failed to determine local time\n");
		return 0;
	}

	char buffer[512];
	bzero(buffer, sizeof(buffer));
	strftime(buffer, sizeof(buffer), DATA_DIR "alice-bob/%F.txt", tmp);
	assert( updateConversation("bob", "alice", TESTTEXT) == 1 );
	sleep(1);
	assert( file_exists(buffer) == 1  );
	assert( checkConversationModifiedAfter("bob","alice", t) == 0 );

	time_t t2;
	t2 = time(NULL);
	assert( checkConversationModifiedAfter("bob", "alice", t2) == 0);

}