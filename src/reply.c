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
	if(sess == NULL){
		qcgires_redirect(req, LOGIN);
		qcgisess_save(sess);
		goto end;
	} 

	username =  sess->getstr(sess, "username", true);
	sess->putstr(sess, "username", username, true);
    qcgisess_save(sess);

    if(username == NULL){
        fprintf(stderr, "%s\n", "Session expired");
        qcgires_redirect(req, LOGIN);
        qcgisess_save(sess);
        goto end;  
    }
    

    char * message = NULL;
    char * targetUser = NULL;
    message = req->getstr(req, "message", true);
    targetUser = req->getstr(req, "user", true);

    if(message == NULL || targetUser == NULL){
        free(username);
    	if(message != NULL) free(message);
    	if(targetUser != NULL) free(targetUser);
    	goto end;
    }

    int success = updateConversation(username, targetUser, message);
    if(success == 0) qcgires_redirect(req, BAD_TALK);
    else qcgires_redirect(req, APPLICATION);

    free(username);
    free(message);
    free(targetUser);


    // De-allocate memories
    end:
    qcgires_setcontenttype(req, "text/html");
    if(sess){
    	sess->free(sess); 
    }
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}