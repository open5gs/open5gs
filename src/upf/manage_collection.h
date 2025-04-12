//
// Created by Fatemeh Shafiei Ardestani on 2025-03-06.
//

#ifndef OPEN5GS_MANAGE_COLLECTION_H
#define OPEN5GS_MANAGE_COLLECTION_H
#ifdef DEFINE_MANAGE
#define EXTERN
#define INITIALIZER(...) = __VA_ARGS__
#else
#define EXTERN extern
#define INITIALIZER(...)
#endif

EXTERN bool is_active INITIALIZER(false);
#endif //OPEN5GS_MANAGE_COLLECTION_H
