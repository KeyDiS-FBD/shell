#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *getCurrentDir(char *pwd) {
    char *directory;
    directory = malloc(256 * sizeof(char));
    int i = 0, j = 0;
    while (pwd[i] != '\0') {
        if (pwd[i] == '/') {
            j = 0;
            i++;
            continue;
        }
        directory[j] = pwd[i];
        i++;
        j++;
    }
    directory[j] = '\0';
    free(pwd);
    return directory;
}

void user() {
    char *pwd = getcwd(NULL, 0);
    char *userName = getenv("USER");
    char host[256];
    gethostname(host, _SC_HOST_NAME_MAX);
    pwd = getCurrentDir(pwd);
    printf("[%s@%s %s]$ ", userName, host, pwd);
    fflush(stdout);
    free(pwd);
}
