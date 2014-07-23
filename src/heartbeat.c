#include "base.h"

/* You might ask yourself, why a heartbeat?
 * well, simply put, it's a method to make sure your system is still
 * alive and well. By querying your heartbeat you can tell if your 
 * system is reachable and usable. 
 * This is helpful should you set up a cronjob for yourself that uses
 * notify-send to let you know if something is down or not.
*/

int main(void){
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    qentry_t *req = qcgireq_parse(NULL, 0);
    qcgires_setcontenttype(req, "text/plain");

    printf("%ld", time(0));

    // De-allocate memories
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}