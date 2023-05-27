#ifndef Message_h
#define Message_h
#include <stdbool.h>

typedef void *(func) (void *) ;


typedef struct message
{
  bool finish;
  func* job;  
  int param;
} Message;

#endif
