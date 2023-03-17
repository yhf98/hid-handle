
#ifndef  __LIST_H__
#define  __LIST_H__


//********************************************************************
#include "hmi_config.h"

//********************************************************************

#if _OS_TYPES_   == OS_WINDOWS
#pragma  message("compile with windows system")
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#elif _OS_TYPES_ == OS_LINUX
#pragma  message("compile with linux system")

#elif _OS_TYPES_ == OS_RTOS
#pragma  message("compile with rtos system")

#else

#pragma  message("compile with other system")

#endif

//********************************************************************
#ifdef __cplusplus
extern "C" {
#endif

typedef struct slist_node {
    struct slist_node *next; /**< point to next node. */
}slist_t;




#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define slist_entry(node, type, member) \
    container_of(node, type, member)

#define slist_first_entry(ptr, type, member) \
    slist_entry((ptr)->next, type, member)

#define slist_tail_entry(ptr, type, member) \
    slist_entry(slist_tail(ptr), type, member)

#define slist_first_entry_or_null(ptr, type, member) \
    (slist_isempty(ptr) ? NULL : slist_first_entry(ptr, type, member))

#define slist_for_each(pos, head) \
    for (pos = (head)->next; pos != NULL; pos = pos->next)

#define slist_for_each_safe(pos, next, head)        \
    for (pos = (head)->next, next = pos->next; pos; \
         pos = next, next = pos->next)


#define slist_for_each_entry_safe(pos, n, head, member)          \
		for (pos = slist_entry((head)->next, typeof(*pos), member),  \
			n = slist_entry(pos->member.next, typeof(*pos), member); \
			 &pos->member != (NULL);								 \
			 pos = n, n = slist_entry(pos->member.next, typeof(*pos), member))

void slist_init(slist_t* l);

void slist_add_head(slist_t* l, slist_t* n);

void slist_add_tail(slist_t* l, slist_t* n);

void slist_insert(slist_t* l, slist_t* next, slist_t* n);

slist_t* slist_remove(slist_t* l, slist_t* n);

unsigned int slist_len(const slist_t* l);

unsigned int slist_contains(slist_t* l, slist_t* n);

slist_t* slist_head(slist_t* l);

slist_t* slist_tail(slist_t* l);

slist_t* slist_next(slist_t* n);

int slist_isempty(slist_t* l);


#ifdef __cplusplus
}
#endif
//********************************************************************

#endif

