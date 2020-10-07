#ifndef LAB_1_STRUCTS_H
#define LAB_1_STRUCTS_H

struct DateStruct {
    int year;
    int month;
    int day;
};

typedef struct DateStruct Date;

struct Store {
    int _index;
    char name[30];
    char address[30];
    int size;
    int first_progress_pos;
};

struct Custumer {
    int balance;
    char product[30];
    char gender[1];
    int _mark;
//    Date _exam_date;
    int next_progress_pos;
};

struct IndexStruct {
    int key;
    int pos;
};

typedef struct Store Store;
typedef struct Custumer Custumer;
typedef struct IndexStruct Index;

#define IND_SIZ  sizeof(Index)
#define STUD_SIZ  sizeof(Store)
#define PROGR_SIZ  sizeof(Custumer)
#define INVALID_PROGR_MARK -100

int num_of_inds();
Index get_ind_by_pos(int pos);
Index get_ind_by_key(int key);
int is_key(int key);
int ind_pos_by_key(int key);
Store get_stud_by_pos(int pos);
Store get_stud_by_key(int key);
Custumer get_custumer_by_pos(int pos);
Custumer get_progr_by_args(int key, const char *subj, int term);
void edit_stud_by_pos(int pos, Store stud);
void edit_ind_by_pos(int pos, Index ind);
void edit_progr_by_pos(int pos, Custumer p);
void del_progr_by_args(int key, const char *subj, int term);
void del_all_progr_by_key(int key);
int del_stud_by_key(int key);
int insert_index(Index ind);
int insert_store(Store stud);
int num_of_progr();
int insert_custumer_by_key(int key, Custumer progr);
void files_init();
void help(FILE* out);
void dump(FILE* out);
void print_stud(FILE *out, Store stud);
void print_progr(FILE *out, Custumer progr);
int get_custumer_pos(int key, const char *subj, int balance);
int progr_num_by_key(int key);


#endif //LAB_1_STRUCTS_H
