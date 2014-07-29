#include "base.h"

int main(void){
    pci_init();
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    qentry_t *req = qcgireq_parse(NULL, 0);
    
    char *name  = req->getstr(req, "username", true);
    if(name == NULL){
        qcgires_redirect(req, BAD_LOGIN);
        goto end;
    }

    if( user_exists(name) == 1 ){   	
    	/* Log the User in */
    	qentry_t *sess = NULL;
		sess = qcgisess_init(req, NULL);
        qcgisess_settimeout(sess, SESSION_TIME);
		if(sess){
            sess->putstr(sess, "username", name, true);
            qcgisess_save(sess);
            sess->free(sess);            
        } 
        qcgires_redirect(req, APPLICATION);
    }else{
    	fprintf(stderr, "%s%s\n", "Could not login user: ", name);
    	qcgires_redirect(req, BAD_LOGIN);
    }
    free(name);

    end:
    qcgires_setcontenttype(req, "text/html");
    // De-allocate memories
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}