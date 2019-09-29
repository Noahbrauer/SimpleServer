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
    uint32_t secretkey;
    uint32_t type;
    uint32_t success;
    rio_t rio;
    int clientfd, port;
    unsigned int len;
    char *host;
    char *varName;
    char *val = (char *)malloc(100);
    stringstream stringVal;
    stringstream var;
    
    if (argc != 5) {
        fprintf(stderr, "usage: %s <host> <port> <SecretKey> <varName>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);
    
    clientfd = Open_clientfd(host, port);
    
    Rio_readinitb(&rio, clientfd);
    
    stringVal << argv[3];
    stringVal >> secretkey;
    secretkey = htonl(secretkey);
    Rio_writen(clientfd, &secretkey, 4);

    type = 1;
    type = htonl(type);
    Rio_writen(clientfd, &type, 4);
    
    varName = argv[4];
    Rio_writen(clientfd, varName, 16);
    
    Rio_readnb(&rio, &success, 4);
    success = ntohl(success);
    if (success == 1){
        cout << "failure" << endl;
    } else {
        Rio_readnb(&rio, &len, 3);
        len = ntohl(len);

        Rio_readnb(&rio, val, 100);
        cout << val << endl;
    }
    
    Close(clientfd); //linnetp:echoclient:close
    exit(0);
}

