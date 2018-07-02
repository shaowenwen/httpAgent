#ifndef RECSSL_H
#define RECSSL_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <string>
#include <fstream>
#include <string.h>
#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include <memory.h>
#include "zlog.h"

//#define uint8_t unsigned int
//#define BYTE    unsigned char
using namespace std;


class Recssl
{
public:
    Recssl();
    string encrypto(char * Plaintext);
    void  Hex2Str( const char *sSrc,  char sDest[], int nSrcLen );
};

#endif // RECSSL_H
