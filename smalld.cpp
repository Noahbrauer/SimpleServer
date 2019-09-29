extern "C" {
#include "csapp.h"
}
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstddef>

using namespace std;

map<char*, char*> vars;
map<char*, char*>::iterator look;

int set(char *varName, char *val){
    vars[varName] = val;
    return 0;
}

int get(char *varName){
    int success = 0;
    char *find = getenv(varName);
    if (find == NULL){
        success = 1;
    }
    return success;
}

int digest(char *varName, string &varHold){
	FILE *var;
	int success = 0;
	char commandStr[150];
	char first[11] = {'/','b','i','n','/','e','c','h','o',' ','\0'};
	char second[22] = {' ','|',' ','/','u','s','r','/','b','i','n','/','s','h','a','2','5','6','s','u','m','\0'};
	strcat(commandStr, first);
	strcat(commandStr, varName);
	strcat(commandStr, second);
	char *buffer = (char *)malloc(100);
	var = popen(commandStr, "r");
	if(var == nullptr){
		success = 1;
	}
	while(fgets(buffer, sizeof(buffer), var)){
		varHold += buffer;
	}
	pclose(var);
	return success;
}

int run(char* commandStr, string &varHold){
	FILE *var;
	int success = 0;
	char buffer[100];
	var = popen(commandStr, "r");
	if(var == nullptr){
		success = 1;
	}
	while(fgets(buffer, sizeof(buffer), var)){
		varHold += buffer;
		varHold += " ";
	}
	pclose(var);
	return success;
}

int main(int argc, char** argv){
	uint32_t clientkey;
    uint32_t func, ice;
    socklen_t clientlen;
    rio_t rio;
    struct sockaddr_in clientaddr;
    int listenfd, connfd, port, success;
    unsigned int key;
    unsigned int len;
    char *varName = (char *)malloc(100);
    char *val = (char *)malloc(100);
	if (argc != 3) {
		fprintf(stderr, "usage: %s <port> <SecretKey>\n", argv[0]);
		exit(0);
	}
    port = atoi(argv[1]);
    key = atoi(argv[2]);

    listenfd = Open_listenfd(port);
    while (1) {
        success = 1;
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Rio_readinitb(&rio, connfd);

        Rio_readnb(&rio, &clientkey, 4);
        clientkey = ntohl(clientkey);

        Rio_readnb(&rio, &func, 4);
        func = ntohl(func);

        if (clientkey == key){
            cout << "Secret key = " << clientkey << endl;
            if (func == 0){
                cout << "Request type = set" << endl;
                Rio_readnb(&rio, varName, 16);
                Rio_readnb(&rio, &len, 3);
        		len = ntohl(len);
        		Rio_readnb(&rio, val, 100);
                cout << "Detail = " << varName << ": " << val << endl;
        		if(len > 100) success = 1;
                else{
					setenv(varName, val, 1);
					success = set(varName, val);
            	}	
                if (success == 0){
                    ice = 0;
                    ice = htonl(ice);
                    Rio_writen(connfd, &ice, 4);
                } else {
                    ice = 1;
                    ice = htonl(ice);
                    Rio_writen(connfd, &ice, 4);
                }
            }else if (func == 1){
                cout << "Request type = get" << endl;
                Rio_readnb(&rio, varName, 15);
                cout << "Detail = " << varName << endl;
                success = get(varName);
                if (success == 0){
                    //send success to client
                    ice = 0;
                    ice = htonl(ice);
                    Rio_writen(connfd, &ice, 4);
                    
                    char *val = getenv(varName);
                    unsigned int len = strlen(val);
                    len = htonl(len);
                    Rio_writen(connfd, &len, 3);
                    Rio_writen(connfd, val, 100);
                } else if (success == 1){
                    //send failure to client
                    ice = 1;
                    ice = htonl(ice);
                    Rio_writen(connfd, &ice, 4);
                }
            }else if (func == 2){
            	char *varHoldnew = (char *)malloc(100);
				string varHold;
            	cout << "Request type = digest" << endl;
            	Rio_readnb(&rio, &len, 3);
        		len = ntohl(len);
	            Rio_readnb(&rio, varName, 100);
	            cout << "Detail = " << varName << endl;
        		if(len > 100) success = 1;
        		else{
		            success = digest(varName, varHold);
		            varHoldnew = new char[varHold.size() + 1];
	    			memcpy(varHoldnew, varHold.c_str(), varHold.size() + 1);
    			}
				if (success == 0){
					//send success to client
					ice = 0;
					ice = htonl(ice);
					Rio_writen(connfd, &ice, 4);
					unsigned int len = strlen(varHoldnew);
					len = htonl(len);
					Rio_writen(connfd, &len, 3);
					Rio_writen(connfd, varHoldnew, 100);
				} else if (success == 1){
					//send failure to client
					ice = 1;
					ice = htonl(ice);
					Rio_writen(connfd, &ice, 4);
				}
            }else if (func == 3){
            	char *varHoldnew = (char *)malloc(100);
				string varHold;
            	char inet[18] = {'/','s','b','i','n','/','i','f','c','o','n','f','i','g',' ','-','a','\0'};
            	char hosts[20] = {'/','b','i','n','/','c','a','t',' ','/','e','t','c','/','h','o','s','t','s','\0'};
            	char services[23] = {'/','b','i','n','/','c','a','t',' ','/','e','t','c','/','s','e','r','v','i','c','e','s','\0'};
	            cout << "Request type = run" << endl;
	            Rio_readnb(&rio, varName, 9);
	            cout << "Detail = " << varName << endl;
	            if (strcmp(varName,"inet") == 0){
	            	success = run(inet, varHold);
	            	varHoldnew = new char[varHold.size() + 1];
    				memcpy(varHoldnew, varHold.c_str(), varHold.size() + 1);
	            } 
	            else if (strcmp(varName,"hosts") == 0){
					success = run(hosts, varHold);
					varHoldnew = new char[varHold.size() + 1];
    				memcpy(varHoldnew, varHold.c_str(), varHold.size() + 1);
	            } 
	            else if (strcmp(varName,"service") == 0){
	            	success = run(services, varHold);
	            	varHoldnew = new char[varHold.size() + 1];
    				memcpy(varHoldnew, varHold.c_str(), varHold.size() + 1);
	            } 
	            else{
	            	cout << "Incorrect run command" << endl;
	            	success = 1;
	            }
	            if (success == 0){
                    //send success to client
                    ice = 0;
                    ice = htonl(ice);
                    Rio_writen(connfd, &ice, 4);
                    unsigned int len = strlen(varHoldnew);
                    len = htonl(len);
                    Rio_writen(connfd, &len, 3);
                    Rio_writen(connfd, varHoldnew, 100);
                } else if (success == 1){
                    //send failure to client
                    ice = 1;
                    ice = htonl(ice);
                    Rio_writen(connfd, &ice, 4);
                }
            }
            cout << "Completion = ";
            if (success == 0){
                cout << "success" << endl;
            } else {
                cout << "failure" << endl;
            }
        } else {
            cout << "Attempted Connection With Incorrect Key" << endl;
            ice = 1;
            ice = htonl(ice);
            Rio_writen(connfd, &ice, 4);
        }
        cout << "--------------------------" << endl;
        Close(connfd);
    }
    exit(0);
}