#include "testp.h"


int main(){
	pci_init();
	/* There should now be a DATA_DIR directory and a users index */

	assert( directory_exists(DATA_DIR) == 1 );
	assert( file_exists(DATA_DIR USERS_INDEX) == 1 );
}