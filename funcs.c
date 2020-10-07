#include <stdio.h>
#include <string.h>
#include "structs.h"

int num_of_inds() {
    FILE *f = fopen("S.ind", "rb");
    Index res;
    fread(&res, IND_SIZ, 1, f);
    fclose(f);
    return res.key;
}

Index get_ind_by_pos(int pos) {
    FILE *f = fopen("S.ind", "rb");
    Index res = {-1, -1};
    fseek(f, (pos + 1) * IND_SIZ, SEEK_SET);
    fread(&res, IND_SIZ, 1, f);
    fclose(f);
    return res;
}

Index get_ind_by_key(int key) {
    int l = 0;
    int r = num_of_inds() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        Index temp = get_ind_by_pos(m);
        if (temp.key == key)
            return temp;
        if (temp.key < key)
            l = m + 1;
        else
            r = m - 1;
    }
    return (Index) {-1, -1};
}

int is_key(int key) {
    Index ind = get_ind_by_key(key);
    return (ind.key > 0 || ind.pos > 0);
}

int ind_pos_by_key(int key) {
    int l = 0;
    int r = num_of_inds() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        Index temp = get_ind_by_pos(m);
        if (temp.key == key)
            return m;
        if (temp.key < key)
            l = m + 1;
        else
            r = m - 1;
    }
    return -5;
}

Store get_store_by_pos(int pos) {
    FILE *f = fopen("S.fl", "rb");
    Store res = {-1, "0", "0", -1, -1};
    fseek(f, pos * STORE_SIZE, SEEK_SET);
    fread(&res, STORE_SIZE, 1, f);
    fclose(f);
    return res;
}

Store get_stud_by_key(int key) {
    Index ind = get_ind_by_key(key);
    Store res = get_store_by_pos(ind.pos);
    return res;
}

Custumer get_custumer_by_pos(int pos) {
    FILE *f = fopen("P.fl", "rb");
    Custumer res;
    fseek(f, (pos + 1) * CUSTUMER_SIZE, SEEK_SET);
    fread(&res, CUSTUMER_SIZE, 1, f);
    fclose(f);
    return res;
}

Custumer get_customer_by_args(int key, const char *subj, int term) {
    Store student = get_stud_by_key(key);
    Custumer cur = get_custumer_by_pos(student._first_custumer_pos);
    int ret = 1;
    while (cur.balance != term || strcmp(subj, cur.product) != 0) {
        if (cur._next_customer_pos > 0)
            cur = get_custumer_by_pos(cur._next_customer_pos);
        else {
            ret = 0;
            break;
        }
    }
    if (!ret)
        cur._mark = INVALID_PROGR_MARK;
    return cur;
}

int get_customer_pos(int key, const char *product_name, int balance) {
    Store student = get_stud_by_key(key);
    Custumer cur = get_custumer_by_pos(student._first_custumer_pos);
    int ret = student._first_custumer_pos;
    while (cur.balance != balance || strcmp(product_name, cur.product) != 0) {
        if (cur._next_customer_pos > 0) {
            ret = cur._next_customer_pos;
            cur = get_custumer_by_pos(cur._next_customer_pos);
        } else {
            ret = INVALID_PROGR_MARK;
            break;
        }
    }
    return ret;
}

void edit_stud_by_pos(int pos, Store stud) {
    FILE *f = fopen("S.fl", "rb+");
    fseek(f, pos * STORE_SIZE, SEEK_SET);
    fwrite(&stud, STORE_SIZE, 1, f);
    fclose(f);
}

void edit_ind_by_pos(int pos, Index ind) {
    FILE *f = fopen("S.ind", "rb+");
    fseek(f, (pos + 1) * IND_SIZ, SEEK_SET);
    fwrite(&ind, IND_SIZ, 1, f);
    fclose(f);
}

void edit_progr_by_pos(int pos, Custumer p) {
    FILE *f = fopen("P.fl", "rb+");
    fseek(f, (pos + 1) * CUSTUMER_SIZE, SEEK_SET);
    fwrite(&p, CUSTUMER_SIZE, 1, f);
    fclose(f);
}


void del_progr_by_args(int key, const char *product_name, int balance) {
    Store student = get_stud_by_key(key);
    Custumer del = get_customer_by_args(key, product_name, balance);
    int del_pos = student._first_custumer_pos;
    int prev_pos = del_pos;
    while (get_custumer_by_pos(del_pos).balance != balance || strcmp(product_name, get_custumer_by_pos(del_pos).product) != 0) {
        prev_pos = del_pos;
        del_pos = get_custumer_by_pos(del_pos)._next_customer_pos;
    }
    if (prev_pos != del_pos) {
        Custumer prev_customer = get_custumer_by_pos(prev_pos);
        prev_customer._next_customer_pos = del._next_customer_pos;
        edit_progr_by_pos(prev_pos, prev_customer);
    } else {
        student._first_custumer_pos = del._next_customer_pos;
        edit_stud_by_pos(get_ind_by_key(key).pos, student);
    }

    Custumer info = get_custumer_by_pos(-1);
    del._next_customer_pos = info._next_customer_pos;
    info._next_customer_pos = del_pos;
    --info._mark;
    edit_progr_by_pos(del_pos, del);
    edit_progr_by_pos(-1, info);
}

void del_all_progr_by_key(int key) {
    Store store = get_stud_by_key(key);
    Custumer custumer = get_custumer_by_pos(store._first_custumer_pos);
    Custumer info = get_custumer_by_pos(-1);
    int empty_pos = info._next_customer_pos;
    info._next_customer_pos = store._first_custumer_pos;
    info._mark = info._mark - progr_num_by_key(key);
    edit_progr_by_pos(-1, info);
    int pos = store._first_custumer_pos;
    store._first_custumer_pos = -5;
    edit_stud_by_pos(get_ind_by_key(key).pos, store);
    while (custumer._next_customer_pos >= 0) {
        pos = custumer._next_customer_pos;
        custumer = get_custumer_by_pos(custumer._next_customer_pos);
    }
    custumer._next_customer_pos = empty_pos;
    edit_progr_by_pos(pos, custumer);
}

void files_clear() {
    FILE *s_ind, *s_fl, *p_fl;
    s_ind = fopen("S.ind", "wb");
    Index ind = {0, 0};
    fwrite(&ind, IND_SIZ, 1, s_ind);
    fclose(s_ind);

    s_fl = fopen("S.fl", "wb");
    fclose(s_fl);

    p_fl = fopen("P.fl", "wb");
    Custumer info = {0, "\0", 0, 0, 0, 0,
                     "\0", 0};
    fwrite(&info, CUSTUMER_SIZE, 1, p_fl);
    fclose(p_fl);
}


int del_stud_by_key(int key) {
    Index del = get_ind_by_key(key);
    printf("del pos = %d", del.pos);
    if (del.key != key)
        return 0;
    Index info = get_ind_by_pos(-1);
    if(info.key == 1){
        files_clear();
        return 1;
    }
    Store last_stud = get_store_by_pos(num_of_inds() - 1);
    if (get_stud_by_key(key)._first_custumer_pos >= 0)
        del_all_progr_by_key(key);
    edit_stud_by_pos(del.pos, last_stud);
    Index last = get_ind_by_key(last_stud._index);
    last.pos = del.pos;
    edit_ind_by_pos(ind_pos_by_key(last.key), last);
    int new_size = --info.key;
    edit_ind_by_pos(-1, info);
    int del_pos = ind_pos_by_key(del.key);
    while (del_pos < new_size) {
        edit_ind_by_pos(del_pos, get_ind_by_pos(del_pos + 1));
        ++del_pos;
    }
    return 1;
}

int insert_index(Index ind) {
    int l = 0;
    int r = num_of_inds() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        Index temp = get_ind_by_pos(m);
        if (temp.key == ind.key) {
            break;
        }
        if (temp.key < ind.key)
            l = m + 1;
        else
            r = m - 1;
    }
    if (r > l) {
        return 0;
    }
    int insert_pos = l;
    int cur = num_of_inds();
    while (cur > insert_pos) {
        edit_ind_by_pos(cur, get_ind_by_pos(cur - 1));
        --cur;
    }
    edit_ind_by_pos(cur, ind);
    Index info = get_ind_by_pos(-1);
    ++info.key;
    edit_ind_by_pos(-1, info);
    //printf("insert index return 1\n");
    return 1;
}

int insert_store(Store stud) {
    Index ind = {stud._index, num_of_inds()};
    if (!insert_index(ind)) {
        return 0;
    }
    edit_stud_by_pos(ind.pos, stud);
    return 1;
}

int num_of_progr() {
    FILE *f = fopen("P.fl", "rb");
    Custumer info;
    fread(&info, CUSTUMER_SIZE, 1, f);
    fclose(f);
//    printf("info._mark = %d\n", info._mark);
    return info._mark;
}

int insert_custumer_by_key(int key, Custumer progr) {
    if (!is_key(key)) {
        return 0;
    }
    Custumer prev;
    Store stud = get_stud_by_key(key);
    int prev_pos = stud._first_custumer_pos;
    Custumer info = get_custumer_by_pos(-1);
    int insert_pos = info._next_customer_pos;
    if (stud._first_custumer_pos < 0) {
        stud._first_custumer_pos = insert_pos;
        edit_stud_by_pos(get_ind_by_key(key).pos, stud);
    } else {
        prev = get_custumer_by_pos(stud._first_custumer_pos);
        while (prev._next_customer_pos >= 0) {
            prev_pos = prev._next_customer_pos;
            prev = get_custumer_by_pos(prev._next_customer_pos);
        }
        prev._next_customer_pos = info._next_customer_pos;
        edit_progr_by_pos(prev_pos, prev);
    }

    if (info._mark == info._next_customer_pos) {
        ++info._mark;
        ++info._next_customer_pos;
    } else {
        info._next_customer_pos = get_custumer_by_pos(insert_pos)._next_customer_pos;
    }
    edit_progr_by_pos(insert_pos, progr);
    edit_progr_by_pos(-1, info);
    return 1;
}

void files_init() {
    FILE *s_ind, *s_fl, *p_fl;
    s_ind = fopen("S.ind", "rb");
    if (!s_ind) {
        s_ind = fopen("S.ind", "wb");
        Index ind = {0, 0};
        fwrite(&ind, IND_SIZ, 1, s_ind);
    }
    fclose(s_ind);

    s_fl = fopen("S.fl", "rb");
    if (!s_fl) {
        s_fl = fopen("S.fl", "wb");
    }
    fclose(s_fl);

    p_fl = fopen("P.fl", "rb");
    if (!p_fl) {
        p_fl = fopen("P.fl", "wb");
        Custumer info = {0, "\0", 0, 0, 0, 0,
                         "\0", 0};
        fwrite(&info, CUSTUMER_SIZE, 1, p_fl);
    }
    fclose(p_fl);
}

void help(FILE *out) {
    fprintf(out, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(out, "'insert-m' - Insert store\n");
    fprintf(out, "'insert-s' - Insert custumer\n");
    fprintf(out, "'get-m' - Find store\n");
    fprintf(out, "'get-s' - Find custumer\n");
    fprintf(out, "'update-m' - Update store\n");
    fprintf(out, "'update-s' - Update custumer\n");
    fprintf(out, "'del-m' - Delete store\n");
    fprintf(out, "'del-s' - Delete custumer\n");
    fprintf(out, "'dump' - Print all store and customers\n");
    fprintf(out, "'exit' - To EXIT\n");
    fprintf(out, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");
}

void dump(FILE *out) {
    //printf("dump start\n");
    int siz = num_of_inds();
    int progr_siz = num_of_progr();
    fprintf(out, "Num of stores = %d\nNum of customers = %d\n", siz, progr_siz);
    for (int i = 0; i < siz; ++i) {
        Index cur_ind = get_ind_by_pos(i);
        Store cur_stud = get_store_by_pos(cur_ind.pos);
        fprintf(out, "-- %-20.20s %-20.20s | %06d \n",
                cur_stud.address, cur_stud.name, cur_stud._index);
        if (cur_stud._first_custumer_pos >= 0) {
            Custumer cur_progr = get_custumer_by_pos(cur_stud._first_custumer_pos);
            for (int j = 0;; ++j) {
                fprintf(out, "\t|-- %-20.20s | %d | %-20.20s\n",
                        cur_progr.product,
                        cur_progr.balance, cur_progr.gender);
                if (cur_progr._next_customer_pos > 0) {
                    cur_progr = get_custumer_by_pos(cur_progr._next_customer_pos);
                } else break;
            }
        }
    }
    fprintf(out, "\n\n");
}

void print_stud(FILE *out, Store stud) {
    fprintf(out, "%-20.20s %-20.20s | %06d \n",
            stud.address, stud.name, stud._index);
}

void print_progr(FILE *out, Custumer progr) {
    fprintf(out, "\t%-20.20s | %d |  %-20.20s\n",
            progr.product,
            progr.balance, progr.gender);
}

int progr_num_by_key(int key) {
    if (!is_key(key))
        return -1;
    int pos = get_stud_by_key(key)._first_custumer_pos;
    int res = 0;
    while (pos >= 0) {
        pos = get_custumer_by_pos(pos)._next_customer_pos;
        ++res;
    }
    return res;
}