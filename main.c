#include <stdio.h>
#include <string.h>
#include "structs.h"


int main() {
    files_init();
    FILE *fin = fopen("input.txt", "r");
    FILE *fout = fopen("output.txt", "w");
    FILE *in = stdin;
    FILE *out = stdout;
    help(out);
    char cmd[30];
    int exit = 0;
    while (!exit) {
        fscanf(in, "%s", cmd);
        if (strcmp(cmd, "insert-m") == 0) {

            Store store;

            fprintf(out, "Enter store index:\n");
            fscanf(in, "%d", &(store._index));
            if (is_key(store._index)) {
                fprintf(out, "Store with %d index is already exist\n\n", store._index);
                continue;
            }
            fprintf(out, "Enter store name:\n");
            fscanf(in, "%s", store.name);

            fprintf(out, "Enter address:\n");
            fscanf(in, "%s", store.address);

            store._first_custumer_pos = -5;
            insert_store(store);
            fprintf(out, "Success\n\n");
        } else if (strcmp(cmd, "insert-s") == 0) {
            Custumer custumer;
            int key;

            fprintf(out, "Enter number of index(int):\n");
            fscanf(in, "%d", &(key));

            if (!is_key(key)) {
                fprintf(out, "Store with %d index doesn't exist\n\n", key);
                continue;
            }

            fprintf(out, "Enter product that custumer bought(char[]):\n");
            fscanf(in, "%s", custumer.product);

            fprintf(out, "Enter balance(int):\n");
            fscanf(in, "%d", &(custumer.balance));
            fprintf(out, "Enter gender(char):\n");
            fscanf(in, "%s", custumer.gender);


            custumer._next_customer_pos = -5;
            insert_custumer_by_key(key, custumer);
            fprintf(out, "Success\n\n");
        } else if (strcmp(cmd, "get-m") == 0) {
            int key;
            fprintf(out, "Enter store index(int):\n");
            fscanf(in, "%d", &key);
            if (is_key(key)) {
                print_stud(out, get_stud_by_key(key));
                fprintf(out, "\n");
            } else {
                fprintf(out, "Store with %d index doesn't exist\n\n", key);
            }
        } else if (strcmp(cmd, "del-m") == 0) {
            int key;
            fprintf(out, "Enter store index(int):\n");
            fscanf(in, "%d", &key);
            if (is_key(key)) {
                del_stud_by_key(key);
                fprintf(out, "Success\n\n");
            } else {
                fprintf(out, "Store with %d index doesn't exist\n\n", key);
            }
        } else if (strcmp(cmd, "update-m") == 0) {
            int key;
            fprintf(out, "Enter store index(int):\n");
            fscanf(in, "%d", &key);
            if (!is_key(key)) {
                fprintf(out, "Store with %d index doesn't exist\n\n", key);
                continue;
            }
            Store stud = get_stud_by_key(key);
            fprintf(out, "By store name - 1\n");
            fprintf(out, "By address - 2\n");

            fprintf(out, "Enter number, what you want to change(int):\n");
            int num;
            fscanf(in, "%d", &num);
            if (num == 1) {
                fprintf(out, "Enter new store's name(char[]):\n");
                fscanf(in, "%s", stud.name);
            } else if (num == 2) {
                fprintf(out, "Enter new address:\n");
                fscanf(in, "%s", stud.address);
            } else {
                fprintf(out, "Incorrect number, try again\n");
                continue;
            }
            fscanf(in, "%d", &num);
            edit_stud_by_pos(get_ind_by_key(key).pos, stud);
            fprintf(out, "Success\n\n");
        } else if (strcmp(cmd, "update-s") == 0) {
            int key;
            fprintf(out, "Enter store index(int):\n");
            fscanf(in, "%d", &key);
            if (!is_key(key)) {
                fprintf(out, "Store with %d index doesn't exist\n\n", key);
                continue;
            }

            char product_name[30];
            fprintf(out, "Enter product name that custumer bought(char[]):\n");
            fscanf(in, "%s", product_name);

            int balance;
            fprintf(out, "Enter balance(int):\n");
            fscanf(in, "%d", &balance);

            int pos = get_customer_pos(key, product_name, balance);
            if (pos == INVALID_PROGR_MARK) {
                fprintf(out, "This custumer doesn't exist\n\n");
                continue;
            }

            Custumer custumer = get_custumer_by_pos(pos);
            fprintf(out, "Product - 1\n");
            fprintf(out, "Balance - 2\n");
            fprintf(out, "Gender - 3\n");
            int num;
            fscanf(in, "%d", &num);
            if (num == 1) {
                fprintf(out, "Enter new product name(char[]):\n");
                fscanf(in, "%s", custumer.product);
            }
            else if (num == 2) {
                fprintf(out, "Enter new balance:\n");
                fscanf(in, "%d", &custumer.balance);
            }
            else if (num == 3) {
                fprintf(out, "Enter new gender(char):\n");
                fscanf(in, "%s", custumer.gender);
            }
            else {
                fprintf(out, "Incorrect number, try again\n");
                continue;
            }
            edit_progr_by_pos(pos, custumer);
            fprintf(out, "Success\n\n");
        } else if (strcmp(cmd, "get-s") == 0) {
            int key;
            fprintf(out, "Enter store index(int):\n");
            fscanf(in, "%d", &key);

            if (!is_key(key)) {
                fprintf(out, "Store with %d index doesn't exist\n\n", key);
                continue;
            }

            char subj[30];
            fprintf(out, "Enter product:\n");
            fscanf(in, "%s", subj);

            int balance;

            fprintf(out, "Enter balance:\n");
            fscanf(in, "%d", &balance);

            Custumer progr = get_customer_by_args(key, subj, balance);
            if (progr._mark == INVALID_PROGR_MARK) {
                fprintf(out, "This custumer doesn't exist\n\n");
                continue;
            }

            print_stud(out, get_stud_by_key(key));
            print_progr(out, progr);
            fprintf(out, "\n");
        } else if (strcmp(cmd, "del-s") == 0) {
            int key;
            fprintf(out, "Enter store index(int):\n");
            fscanf(in, "%d", &key);

            if (!is_key(key)) {
                fprintf(out, "Store with %d index doesn't exist\n\n", key);
                continue;
            }

            char product_name[30];
            fprintf(out, "Enter product name that custumer bought(char[]):\n");
            fscanf(in, "%s", product_name);

            int balance;
            fprintf(out, "Enter balance(int)a:\n");
            fscanf(in, "%d", &balance);

            int pos = get_customer_pos(key, product_name, balance);
            if (pos == INVALID_PROGR_MARK) {
                fprintf(out, "This custumer doesn't exist\n\n");
                continue;
            }

            del_progr_by_args(key, product_name, balance);
            fprintf(out, "Success\n\n");
        } else if (strcmp(cmd, "dump") == 0) {
            dump(out);
        } else if (strcmp(cmd, "exit") == 0) {
            exit = 1;
            continue;
        }
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
