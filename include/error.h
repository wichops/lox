#ifndef ERROR_H_
#define ERROR_H_


extern int HAD_ERROR;

void report(int line, char* where, char* message);
void error(int line, char* message);

#endif // ERROR_H_
