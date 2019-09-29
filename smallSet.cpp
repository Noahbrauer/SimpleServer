extern "C" {
#include "csapp.h"
}
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    int clientfd, port;
    char *host;
    uint32_t secretkey;
    uint32_t type;
    uint32_t success;
    rio_t rio;
    char *varName;
    char *val;
    stringstream stringVal;
    stringstream var;

    if (argc != 6) {
        fprintf(stderr, "usage: %s <host> <port> <SecretKey> <varName> <val>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);
    val = argv[5];

    clientfd = Open_clientfd(host, port);
    
    Rio_readinitb(&rio, clientfd);
    
    stringVal << argv[3];
    stringVal >> secretkey;
    secretkey = htonl(secretkey);
    Rio_writen(clientfd, &secretkey, 4);

    type = 0;
    type = htonl(type);
    Rio_writen(clientfd, &type, 4);
    
    varName = argv[4];
    Rio_writen(clientfd, varName, 16);

    unsigned int len = strlen(val);
    len = htonl(len);
    Rio_writen(clientfd, &len, 3);

    Rio_writen(clientfd, val, 100);

    Rio_readnb(&rio, &success, 4);
    success = ntohl(success);
    if (success == 1){
        cout << "failure" << endl;
    }

    Close(clientfd);
    exit(0);
}
