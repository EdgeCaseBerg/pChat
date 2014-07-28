#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "base.h"
#include "time.h"


int createConversation(const char * userA, const char * userB);

int updateConversation(const char * userA, const char * userB, const char * msg);

int checkConversationModifiedAfter(const char * userA, const char * userB, time_t lastCheckedTime);

/* Calling users responsible for fclose on File **/
FILE * getConversationFileForReading(const char * userA, const char * userB);
FILE * getConversationFileForWriting(const char * userA, const char * userB);
 
#endif