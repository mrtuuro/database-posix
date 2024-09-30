#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

#include "../include/common.h"
#include "../include/file.h"
#include "../include/parse.h"

void print_usage(char *argv[]) {
    printf("Usage: %s -n -f <database file>\n", argv[0]);
    printf("\t -n - create a new db\n");
    printf("\t -f - open a database file\n");
    return;
}


int main(int argc, char *argv[]) {

    struct dbheader_t *dbhdr = NULL;
    struct employee_t *employees = NULL;
    char *filepath = NULL;
    char *addstring = NULL;
    bool newfile = false;
    bool list = false;
    int c;
    int dbfd = -1;

    while ((c = getopt(argc, argv, "nf:a:l")) != -1 ) {

        switch (c) {
            case 'n':
                newfile = true;
                break;
            case 'f':
                filepath = optarg;
                break;
            case 'a':
                addstring = optarg;
                break;
            case 'l':
                list = true;
                break;
            case '?':
                printf("Unknown option -%c\n", c);
                break;
            default:
                return -1;
        }
    };


    if (filepath == NULL) {
        printf("Filepath is a required argument\n");
        print_usage(argv);
        return 0;
    }


    if (newfile) {
        dbfd = create_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            perror("Unable to create database file\n");
            return -1;
        }

        if (create_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
            printf("Failed to create database dbhdr\n");
            return -1;
        }

    } else {
        dbfd = open_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            perror("Unable to open database file\n");
            return -1;
        }
        if (validate_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
            printf("Failed to validate database header\n");
            return -1;
        }
    }

    if (read_employee(dbfd, dbhdr, &employees) != STATUS_SUCCESS) {
        printf("Failed to read employees\n");
        return -1;
    }

    if (addstring) {
        dbhdr->count++;
        employees = realloc(employees, dbhdr->count * (sizeof(struct employee_t)));
        add_employee(dbhdr, employees, addstring);
    }

    if (list) {
        list_employees(dbhdr, employees);
    }
    output_file(dbfd, dbhdr, employees);

    return 0;

}
