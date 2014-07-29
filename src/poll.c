#include "base.h"

static void printUpdated(int updated){
    printf("{\"updated\": %s}", updated ? "true" : "false");
}

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
		qcgisess_save(sess);
        printUpdated(0);
        qcgires_setcontenttype(req, "application/json");
		goto end;
	} 
    qcgires_setcontenttype(req, "application/json");

	username =  sess->getstr(sess, "username", true);
	sess->putstr(sess, "username", username, true);
    qcgisess_save(sess);

    if(username == NULL){
        fprintf(stderr, "%s\n", "Session expired");
        printUpdated(0);
        qcgisess_save(sess);
        goto end;  
    }

    char * target = req->getstr(req, "target", true);
    if(target == NULL){
        fprintf(stderr, "%s\n", "No Target Specified");
        printUpdated(0);
        free(username);
        goto end;
    }

    time_t lastChecked = (time_t)req->getint(req, "date");
    fprintf(stderr, "lastChecked %zu\n", lastChecked);
    int updated = checkConversationModifiedAfter(username, target, lastChecked);
    printUpdated(updated);
    
    free(username);
    free(target);
    // De-allocate memories
    end:
    
    if(sess){
    	sess->free(sess); 
    }
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}