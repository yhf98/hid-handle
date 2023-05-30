#include "stdio.h"
#include "list.h"



void slist_init(slist_t *l)
{
    l->next = NULL;
}

void slist_add_head(slist_t *l, slist_t *n)
{
    n->next = l->next;
    l->next = n;
}

void slist_add_tail(slist_t *l, slist_t *n)
{
    while (l->next) {
        l = l->next;
    }

    /* append the node to the tail */
    l->next = n;
    n->next = NULL;
}

void slist_insert(slist_t *l, slist_t *next, slist_t *n)
{
    if (!next) {
        slist_add_tail(next, l);
        return;
    }

    while (l->next) {
        if (l->next == next) {
            l->next = n;
            n->next = next;
        }

        l = l->next;
    }
}

slist_t *slist_remove(slist_t *l, slist_t *n)
{
    /* remove slist head */
    while (l->next && l->next != n) {
        l = l->next;
    }

    /* remove node */
    if (l->next != (slist_t *)0) {
        l->next = l->next->next;
    }

    return l;
}

unsigned int slist_len(const slist_t *l)
{
    unsigned int len = 0;
    const slist_t *list = l->next;

    while (list != NULL) {
        list = list->next;
        len++;
    }

    return len;
}

unsigned int slist_contains(slist_t *l, slist_t *n)
{
    while (l->next) {
        if (l->next == n) {
            return 0;
        }

        l = l->next;
    }

    return 1;
}

slist_t *slist_head(slist_t *l)
{
    return l->next;
}

slist_t *slist_tail(slist_t *l)
{
    while (l->next) {
        l = l->next;
    }

    return l;
}

slist_t *slist_next(slist_t *n)
{
    return n->next;
}

int slist_isempty(slist_t *l)
{
    return l->next == NULL;
}


