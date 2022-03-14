//
// Created by Stephan on 22.11.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

char * f_get_line(FILE * file) {
    char * res = NULL;
    char n = 0;
    int old_len = 0;
    char buf[101] = {0};
	printf("%p\n", file);

    do {
        n = fscanf(file, "%100[^\n]", buf);
        int chunck_len = strlen(buf);
        //int old_len = strlen(res);
	    printf("buf: %s\n", buf);
        if (n == 1) {
            res = (char *) realloc(res, sizeof(char) * (old_len + chunck_len + 1));
            memcpy(res + old_len, buf, chunck_len);
            old_len += chunck_len;
        } else if (!n) fscanf(file, "%*c");

        else if (!old_len) return NULL;

    } while (n > 0);

    // fscanf(file, "%*c");
    if (old_len > 0) res[old_len] = '\0';
    else res = (char *) calloc(1, sizeof(char));
    return res;
}

char * f_get_lines(char * path) {
    setlocale(LC_ALL, "Rus");
    int m_len = 0;
    char * res = (char*) calloc(1, sizeof(char));

    FILE * file = fopen(path, "r");
    if (file == NULL) {
        printf("Something came wrong. Check you pass and try again, please.\n");
        sleep(5);
        //fclose(file);
        return NULL;
    }

    char * line = NULL;
    int len = 0;
    do {
        line = f_get_line(file);
	    printf("line: %s\n", line);
	    sleep(3);
        if (line) {
            len = strlen(line);
            m_len += len+1;
            res = realloc(res, sizeof(char)*(m_len+1));
            memcpy(res+m_len-len, line, len);
            res[m_len-1] = '\n';
            res[m_len] = '\0';
        }
    } while(line);

    if (m_len > 0) {
        m_len--;
        res = (char*) realloc(res, m_len*sizeof(char));
        res[m_len] = '\0';
    }
    else res = (char *) calloc(1, sizeof(char));
    printf("%s\n", res);
    sleep(3);

    fclose(file);
    return res;
}
