#pragma once

struct Node_Type {
	long key;
	char name[32];
	struct Node* next;
};

int type_conversion();
int sizeof_basic_types();
int float_learn();
int enum_learn();
int void_learn();
int error_exit(unsigned int error_n);
int string_literal_learn();
int pointer_learn();
int incremental_learn();
int struct_op();
int Alignof_learn();
extern int function_learn();
int factorial(register unsigned int n);
int addArray(register float a1[], register float a2[], const int len);
int initNode(struct Node_Type* pNode);
inline void swapf(float* p1, float* p2);
void selection_sortf(float a[], int n);
const float* binarySearch(const float val, const float array[], const int n);
double add(const int n, ...);
//int array(int n);
int array_init();
int string_init();
int mul_array_init();
int void_pointer();
int const_pointer();
int array_pointer();
int func_pointer();
int struct_unit_bitfiled();
int DynamicMemoryManagement();
//BTree_main
int File_stream();
int scanf_test();
