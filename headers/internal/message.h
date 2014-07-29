#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "base.h"
#include "time.h"


/* returns 0 on failure, 1 on success */
int createConversation(const char * userA, const char * userB);

/* returns 0 on failure, 1 on success */
int updateConversation(const char * userA, const char * userB, const char * msg);

/* returns 0 on failure, 1 on success */
int checkConversationModifiedAfter(const char * userA, const char * userB, time_t lastCheckedTime);

/* Calling users responsible for fclose on File **/
FILE * getConversationFileForReading(const char * userA, const char * userB);
FILE * getConversationFileForWriting(const char * userA, const char * userB);
 
#endif