#ifndef __USER_H__
#define __USER_H__

#include "base.h"
#include <string.h>

/* 1 for succesful creation or if user is already existent, 0 otherwise */
int create_user(const char * username);

/* 1 for user exists, 0 for does not or file could not be opened */
int user_exists(const char * username);

#endif