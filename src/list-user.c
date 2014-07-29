#include "base.h"

int main(void){
    pci_init();
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    qentry_t *req = qcgireq_parse(NULL, 0);
    qcgires_setcontenttype(req, "application/json");

    FILE * fp = getUsersFile();
    printf("[");
    if(fp){
    	char user[BUFFER_LENGTH];
    	int i = 0;
    	while(fscanf(fp, "%" TOSTR(BUFFER_LENGTH) "s\r\n", user) == 1){
    		if(i > 0) printf(",");
    		printf("%s", user);
    		i++;
		}
    }
    printf("]");


    // De-allocate memories
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}