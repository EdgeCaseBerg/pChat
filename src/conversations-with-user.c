#include "base.h"

int main(void){
    pci_init();
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    qentry_t *req = qcgireq_parse(NULL, 0);

    qentry_t *sess = NULL;
    sess = qcgisess_init(req, NULL);
    qcgisess_settimeout(sess, SESSION_TIME);
    qcgires_setcontenttype(req, "application/json");    
    if(!sess){
        printf("%s", "{ \"error\" : \"Your Session has expired please login again\"}");
        goto end;
    }

    char *name  = sess->getstr(sess, "username", true);
    if(name == NULL){
        printf("%s", "{ \"error\" : \"Your Session has expired and your username could not be determined, please login again\"}");
        goto end;
    }

    if(sess){
        sess->putstr(sess, "username", name, true);
        qcgisess_save(sess);                
    }

    printf("{ ");
    char * target = req->getstr(req, "target", true);
    if(target == NULL){
        printf("\"error\": \"you must select a user to talk to\"}");
        free(name);
        goto end;
    }

    
    printf("\"conversations\" : [ ");
    /* Now retrieve the conversations with the target between themselves and the user */


    printf(" ]}");
    free(name);
    end:
    if(sess){
        sess->free(sess);   
    }
    // De-allocate memories
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}