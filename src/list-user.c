#include "base.h"

int main(void){
    pci_init();
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    qentry_t *req = qcgireq_parse(NULL, 0);
    char * username = NULL; 

    qentry_t *sess = NULL;
	sess = qcgisess_init(req, NULL);
    qcgisess_settimeout(sess, SESSION_TIME);
	if(sess){
        username =  sess->getstr(sess, "username", false);
        qcgisess_save(sess);
    } 

    qcgires_setcontenttype(req, "application/json");

    FILE * fp = getUsersFile();
    printf("[");
    if(fp){
    	char user[BUFFER_LENGTH];
    	int i = 0;
    	while(fscanf(fp, "%" TOSTR(BUFFER_LENGTH) "s\r\n", user) == 1){    		
    		if(username == NULL || strncmp(user, username, BUFFER_LENGTH) != 0){
                if(i > 0) printf(",");
    			printf("\"%s\"", user);
                i++;
    		}
    		
		}
		fclose(fp);
    }
    printf("]");


    // De-allocate memories
    if(sess){
    	sess->free(sess); 
    }
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}