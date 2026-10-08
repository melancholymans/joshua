#include "unity.h"

#define STB_DS_IMPLEMENTATION
#include "../shogi/stb_ds.h"
#include "test_stb_ds.h"

/*
https://nothings.org/stb_ds/
Arrays
Dynamic arrays are accessed using a pointer to the type of data you want in the array.
Here's some sample code for creating and manipulating a dynamic array:

 int main(void)
 {
   int *array = NULL;
   arrput(array, 2);
   arrput(array, 3);
   arrput(array, 5);
   for (int i=0; i < arrlen(array); ++i)
     printf("%d ", array[i]);
 }
arrput pushes the value onto the end of the dynamic array (like std::vector.push_back), so the above code will print 2 3 5.
Note that these macros write to the array variable if the array has to be resized. This means that if you pass a dynamic array into a function which increases its length, you need to return the updated array from the function to its caller. In other words, the semantics are the same as any realloc()-ed pointer, and are unlike the semantics of things like std::vector<>, where the core object is stable even if the array elements aren't.
The following functions are defined:

arrlen - the length of the dynamic array
arrlenu - the length of the dynamic array as an unsigned type
arrput - copy an object into the dynamic array
arrfree - free the entire array
arraddn - adds n uninitialized values to the dynamic array
arrsetlen - sets the length of the array (leaves new slots uninitialized)
arrlast - the last item in the array as an lvalue
arrins - inserts an item in the middle of the array
arrdel - deletes an item from the middle of the array, moving the following items over
arrswapdel - deletes an item from the middle of the array, replacing it with the formerly last item
arrcap - returns the internal capacity, the maximum length the array can be without reallocating it
arrsetcap - sets the internal capacity. it is not possible to shrink the capacity (currently)
*/

void test_stb_array(void) {
    int* array = NULL;
    arrput(array, 2);
    arrput(array, 3);
    arrput(array, 5);
    printf("array length %zu\n", arrlen(array));
    arrsetcap(array, 16);   //ここでarrayのアドレスが変更になった
    printf("array setlen %zu\n", arrlen(array));
    printf("arr cap %zu\n", arrcap(array));
    arrput(array, 6);

    for (int i = 0; i < arrlen(array); i++) {
        printf("%d \n", array[i]);
    }
    int t = arrpop(array);
    printf("pop data %d \n", t);
    printf("array length %zu\n", arrlen(array));
    printf("arr cap %zu\n", arrcap(array));
    arrput(array, 21);
    arrput(array, 22);
    arrput(array, 23);
    arrput(array, 24);
    arrput(array, 25);
    arrput(array, 26);
    arrput(array, 27);
    arrput(array, 28);
    arrput(array, 29);
    arrput(array, 30);
    arrput(array, 31);
    arrput(array, 32);
    arrput(array, 33);  //ここで設定したcapを超えるはずであるがarrayのアドレスは変わらなかった、同時にcapが自動的に32に上がったいた。
    arrput(array, 34);
    arrput(array, 35);
    arrput(array, 36);
    arrput(array, 37);
    printf("array length %zu\n", arrlen(array));    //20
    printf("arr cap %zu\n", arrcap(array)); //32
}