#ifndef __CONFIG_H__
#define __CONFIG_H__

/*  Data Layer Constants */
#define DATA_DIR "/home/guest/bgi/data/"
#define DATA_DIR_PERM 0755 
#define USERS_INDEX ".users"
#define BUFFER_LENGTH 1024

/*  CGI Layer Constants */
#define SESSION_TIME 3600 /* Hour */
#define BASE_URL "http://www.pci.test"
#define ADMIN_SECRET "admin"

#endif
