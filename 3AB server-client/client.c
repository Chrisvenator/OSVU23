#include "client_helper_functions.c"
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <string.h>


int main(int argc, char *argv[]) {
    arguments args = parse_arguments(argc, argv);
    args.dir = "/";
//    args.url = "http://www.example.com:8080/index.html";

    char *httpResponse = performHttpGet(args);
    fprintf(stdout, "Response:\n\n:%s", httpResponse);



    free(httpResponse);
}
