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
        printf("%s", "{ text : \"Your Session has expired please login again\"}");
        goto end;
    }

    char *name  = sess->getstr(sess, "username", true);
    if(name == NULL){
        printf("%s", "{ text : \"Your Session has expired and your username could not be determined, please login again\"}");
        goto end;
    }

    if(sess){
        sess->putstr(sess, "username", name, true);
        qcgisess_save(sess);                
    }

    printf("{\"text\":");
    char * target = req->getstr(req, "target", true);
    if(target == NULL){
        printf("\"you must select a user to talk to\"}");
        free(name);
        goto end;
    }

    char * filename = req->getstr(req, "f", true);
    if(filename == NULL){
        printf("\"You must choose a file to read\"}");
        free(name);
        free(target);
        goto end;
    }


    FILE * fp = getConversationByFileName(name, target, filename);
    if(fp == NULL){
        free(name);
        printf("%s%s%s","\"Could not open conversation with user for reading: ", target,"\"}");
        free(target);
        free(filename);
        goto end;
    }

    int cOrEOF;
    char c;
    printf("\"");
    while( (cOrEOF = fgetc(fp)) != EOF){
        c = (char)cOrEOF;
        if(c == '"') printf("\\%c", c);
        else if(c == '\n') printf("<br/>");
        else printf("%c", c);
    }
    fclose(fp);

    printf("\"}");
    free(filename);
    free(target);
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