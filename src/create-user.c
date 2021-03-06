#include "base.h"

int main(void){
    pci_init();
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    qentry_t *req = qcgireq_parse(NULL, 0);
    
    char *name  = req->getstr(req, "username", true);
    if(name == NULL){
        qcgires_redirect(req, BAD_REGISTER);
        goto end;
    }

    char *admin = req->getstr(req, "adminpassword", true);
    if(admin == NULL){
    	qcgires_redirect(req, BAD_REGISTER);
    	free(name);
    	goto end;
    }

    if(strncmp(admin, ADMIN_SECRET, strlen(ADMIN_SECRET)) != 0){
    	fprintf(stderr, "%s%s\n", "Invalid Registration Attempt: ", admin);
    	qcgires_redirect(req, BAD_REGISTER);
    	free(name);
    	free(admin);
    	goto end;
    }

    if( create_user(name) == 1 ){   	
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
    	fprintf(stderr, "%s%s\n", "Could not create user: ", name);
    	qcgires_redirect(req, BAD_REGISTER);
    }

    free(name);
    free(admin);

    end:
    qcgires_setcontenttype(req, "text/html");
    // De-allocate memories
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}