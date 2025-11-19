//
// Created by Fatemeh Shafiei Ardestani on 2024-12-02.
//
#include "types.h"
#include "btree.h"
#ifndef OPEN5GS_SHARED_VARIABLES_H
#define OPEN5GS_SHARED_VARIABLES_H
#ifdef DEFINE_UPF_SHARED
#define EXTERN
#define INITIALIZER(...) = __VA_ARGS__
#else
#define EXTERN extern
#define INITIALIZER(...)
#endif

EXTERN BTreeMap mymap INITIALIZER(NULL);



#endif //OPEN5GS_SHARED_VARIABLES_H
