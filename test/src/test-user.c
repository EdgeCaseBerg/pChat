#include "testp.h"


int main(){
	pci_init();
	/* There should now be a DATA_DIR directory and a users index */

	assert( directory_exists(DATA_DIR) == 1 );
	assert( file_exists(DATA_DIR USERS_INDEX) == 1 );

	assert( user_exists("testuser") == 0 );
	assert( create_user("testuser") == 1 );
	assert( user_exists("testuser") == 1 );
	assert( create_user("testuser") == 1 );
}