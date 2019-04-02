#include "klib/khash.h"
#include "_mtypes.h"


#ifndef TPMAP_H
#define TPMAP_H


// Initialize the kmap
KHASH_MAP_INIT_INT64("typedict", PyMTypeObject *node)

// Add Data to Dict
// int typedict_add(name, PyMTypeObject *node) {

// }

int typedict_valuecheck(name) {
    // Check the values of the 
    // pointers...
    kh_foreach_value(h, vvar, code);
}

int typedict_sizecheck(name) {
    // Method used to check the size of hashtable to see if
    // reallocation is neccesary.
    if (kh_size(khash_t(name)*) <= kh_n_buckets(khash_t(name)*)) {
        return 0;
    }
    else {
        kh_resize(name, khash_t(name)*, size_t)
    }
}

int typedict_destroy(name) {
    khash_destroy(name, khash_t(name)*)
    return 0; 
}