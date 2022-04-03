#ifndef _DRAGONSTD_CALLBACK_H_ // include guard
#define _DRAGONSTD_CALLBACK_H_

typedef void *(*Transformer)(void *dat);
typedef void (*Callback)(void *dat);
typedef void (*Iterator)(void *dat, void *arg);
typedef int (*Comparator)(const void *dat, const void *key);

#endif // _DRAGONSTD_CALLBACK_H_
