#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include "arraylist.h"
#include "foreach.h"

/******************************************
 *                  ITEMS                 *
 ******************************************/
arraylist_t *global_list;

typedef struct {
    char* name;
    int32_t id;
    double gpa;
}student_t;

typedef struct{
    int i;
    float f;
    long double ld;
    char c1:4;
    char c2:4;
    short s;
    void *some_data;
}test_item_t;

typedef struct{
    int num;

}int_t;

/******************************************
 *              HELPER FUNCS              *
 ******************************************/
void test_item_t_free_func(void *argptr){
    test_item_t* ptr = (test_item_t*) argptr;
    if(!ptr)
        free(ptr->some_data);
    else
        cr_log_warn("%s\n", "Pointer was NULL");
}

void setup(void) {
    cr_log_warn("Setting up test");
    global_list = new_al(sizeof(test_item_t));
}

void teardown(void) {
    cr_log_error("Tearing down");
    delete_al(global_list, test_item_t_free_func);
}
void* insert_itemtoal(void* data){
    insert_al(global_list,data);
    pthread_exit(NULL);
}
void* delete_itemfromal(void* data){
    test_item_t* item=(test_item_t*)data;
    remove_data_al(global_list,item);
    pthread_exit(NULL);

}
void* delete_itemfromindex(void* data){
    int* num=(int*)data;


    remove_index_al(global_list,*num);
    pthread_exit(NULL);
}
int apply_increase_one(void* data) {
    ((int_t*)data)->num++;
    return 0;
}




/******************************************
 *                  TESTS                 *
 ******************************************/
Test(al_suite, 0_creation, .timeout=2){
    arraylist_t *locallist = new_al(sizeof(test_item_t));

    cr_assert_not_null(locallist, "List returned was NULL");
}

Test(al_suite, 1_deletion, .timeout=2){
    arraylist_t *locallist = new_al(sizeof(test_item_t));

    cr_assert_not_null(locallist, "List returned was NULL");

    delete_al(locallist, test_item_t_free_func);

    cr_assert(true, "Delete completed without crashing");
}

Test(al_suite, 2_insertion, .timeout=2, .init=setup, .fini=teardown){
    cr_assert(true, "I win");
}

Test(al_suite,3_deletion, .timeout=2, .init=setup, .fini=teardown){
    int num_items=200;
    test_item_t* items[num_items];
    for(int i=0; i< num_items;i++){
        items[i] = (test_item_t*)malloc(sizeof(test_item_t));
        items[i]->i = i;
        items[i]->some_data = malloc(1);
    }
     for(int i=0;i<num_items;i++){
        insert_al(global_list,items[i]);
    }

    cr_assert(global_list->length==num_items,"test 3: wrong length after insertion");
     pthread_t pids[num_items];
    for(int i=0;i<num_items;i++){


        pthread_create(&pids[i],NULL,delete_itemfromal,items[i]);
    }
    for(int i=0;i<num_items;i++){
        pthread_join(pids[i],NULL);


    }
    cr_assert(global_list->length==0,"test3: wrong length after insertion");


}

Test(al_suite, 4_deletion, .timeout=2, .init=setup, .fini=teardown) {
    int num_del_Items = 10000;

    test_item_t* items[num_del_Items];

    int rm_num = num_del_Items + 1;

    for(int i = 0; i < num_del_Items; i++) {

        items[i] = (test_item_t*)malloc(sizeof(test_item_t));
        items[i]->i = i;
        items[i]->some_data = malloc(1);
    }


    for(int i = 0; i < num_del_Items; i++)
        insert_al(global_list, items[i]);

    printf("Test4 length of list after insertion: %lu\n", global_list->length);

    cr_assert(global_list->length == num_del_Items, "Test 4: wrong length after insertion");
    pthread_t pids[num_del_Items];

    for(int i = 0; i < num_del_Items; i++){
        pthread_create(&pids[i], NULL, delete_itemfromindex, &rm_num);
    }

    for(int i = 0; i < num_del_Items; i++){
        pthread_join(pids[i], NULL);;
    }

    cr_assert(global_list->length == 0, "Test 4: wrong length after deletion");

}
Test(al_suite, 5_foreach_wo_mt, .timeout=2, .init=setup, .fini=teardown) {

    arraylist_t *locallist = new_al(sizeof(int_t));

    int_t* test_int1 = (int_t*)malloc(sizeof(int_t));
    test_int1->num = 7;

    int_t* test_int2 = (int_t*)malloc(sizeof(int_t));
    test_int2->num = 12;

    int_t* test_int3 = (int_t*)malloc(sizeof(int_t));
    test_int3->num = 26;

    insert_al(locallist, test_int1);
    insert_al(locallist, test_int2);
    insert_al(locallist, test_int3);

    apply(locallist, apply_increase_one);

    int_t* test_int1_dup = get_index_al(locallist, 0);
    int_t* test_int2_dup = get_index_al(locallist, 1);


    int_t* test_int3_dup = get_index_al(locallist, 2);

    #ifdef DEBUG
    fprintf(stderr, "after apply, int 1: %d\n", test_int1_dup->num);
    fprintf(stderr, "after apply, int 2: %d\n", test_int2_dup->num);
    fprintf(stderr, "after apply, int 3: %d\n", test_int3_dup->num);
    #endif

    cr_assert(test_int1_dup->num == 8, "Error during apply (increase 1): 1");
    cr_assert(test_int2_dup->num == 13, "Error during apply (increase 1): 1");
    cr_assert(test_int3_dup->num == 27, "Error during apply (increase 1): 1");
}
