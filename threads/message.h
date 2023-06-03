#ifndef Message_h
#define Message_h
#include <stdbool.h>

typedef void *(func) (void *) ;//puntero a funcion a ejecutar en los hilos


typedef struct message // estructura para enviar mensajes en el thread pool
{
  bool finish;
  func* job;  
  int param;
} Message;

#endif
