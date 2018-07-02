#include "recssl.h"
extern zlog_category_t* g_server_cat;


Recssl::Recssl()
{

}

void Recssl::Hex2Str( const char *sSrc,  char sDest[], int nSrcLen )
{
    int  i;
    char szTmp[3];

    for( i = 0; i < nSrcLen; i++ )
    {
        sprintf( szTmp, "%02x", (unsigned char) sSrc[i] );
        memcpy( &sDest[i * 2], szTmp, 2 );
    }
    return ;
}
string Recssl::encrypto(char * Plaintext)
{
    AES_KEY aes;
    unsigned char key[AES_BLOCK_SIZE];        // AES_BLOCK_SIZE = 16
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned int len;
    unsigned int i;
    unsigned char temp[]="NJITSSANBAO";
    unsigned char* encrypt_string;
    // set the encryption length
    len = 0;
    if ((strlen(Plaintext) + 1) % AES_BLOCK_SIZE == 0) {
        len = strlen(Plaintext) + 1;
    } else {
        len = ((strlen(Plaintext) + 1) / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }

    // set the input string
    unsigned char* input_string;
    input_string = (unsigned char*)calloc(len, sizeof(unsigned char));
    if (input_string == NULL) {
        zlog_error(g_server_cat, "Unable to allocate memory for input_string\n");
    }
    strncpy((char*)input_string, Plaintext, strlen(Plaintext));

    // Generate AES 128-bit key
    strncpy((char*)key,(char*)temp,AES_BLOCK_SIZE);
    // Set encryption key
    for (i=0; i<AES_BLOCK_SIZE; ++i) {
        iv[i] = 0;
    }
    if (AES_set_encrypt_key(key, 128, &aes) < 0) {
        zlog_error(g_server_cat, "Unable to set encryption key in AES\n");
    }

    // alloc encrypt_string
    encrypt_string = (unsigned char*)calloc(len, sizeof(unsigned char));
    if (encrypt_string == NULL) {
        zlog_error(g_server_cat, "Unable to allocate memory for encrypt_string\n");
    }

    // encrypt (iv will change)

    AES_cbc_encrypt(input_string, encrypt_string, len, &aes, iv, AES_ENCRYPT);
    unsigned char tmp[32];
    cout <<"input_string:" <<input_string<<endl;
    cout <<"encrypt_string:" <<encrypt_string<<endl;

    Hex2Str((char*)encrypt_string,(char*)tmp,strlen((char*)encrypt_string));
    cout <<"tmp:" <<tmp<<endl;

    string encryptStr = (char*)tmp;
    //cout <<"encryptStr:" <<encryptStr<<endl;
    //cout <<"(unsigned char*)encryptStr:" <<(unsigned char*)encryptStr.substr(0,32).c_str()<<endl;

    return  encryptStr;
}


