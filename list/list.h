#ifndef _SPDIoT_LIST_H_
#define _SPDIoT_LIST_H_

#include <stdlib.h>
#define LIST_POISON1 ((void *) 0x00100100)
#define LIST_POISON2 ((void *) 0x00200200)

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct _spdIoTListHead {
    struct _spdIoTListHead *next, *prev;
}spdIoTListHead;

static inline void spdIoT_list_init(spdIoTListHead *list)
{

    list->next = head;
    list->prev = head;
}

static inline void __spdIoT_list_add(struct _spdIoTListHead *newNode,
                                     struct _spdIoTListHead *prev,
                                     struct _spdIoTListHead *next)
{
    next->prev = newNode;
    newNode->next = next;
    newNode->prev = prev;
    prev->next = newNode;
}


static inline void spdIoT_list_add(struct _spdIoTListHead *newNode, struct _spdIoTListHead *head)
{
    __spdIoT_list_add(newNode, head, head->next);
}

static inline void spdIoT_list_add_tail(struct _spdIoTListHead *newNode, struct _spdIoTListHead *head)
{
    __spdIoT_list_add(newNode, head->prev, head);
}

static inline void __spdIoT_list_del(struct _spdIoTListHead * prev, struct _spdIoTListHead * next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void spdIoT_list_del(struct _spdIoTListHead *entry)
{
    __spdIoT_list_del(entry->prev, entry->next);
    entry->next = LIST_POISON1;
    entry->prev = LIST_POISON2;
}

static inline void spdIoT_list_replace(struct _spdIoTListHead *old,
                                       struct _spdIoTListHead *newNode)
{
    newNode->next = old->next;
    newNode->next->prev = newNode;
    newNode->prev = old->prev;
    newNode->prev->next = newNode;
}

static inline void spdIoT_list_replace_init(struct _spdIoTListHead *old,
                                            struct _spdIoTListHead *newNode)
{
    spdIoT_list_replace(old, newNode);
    spdIoT_list_init(old);
}

static inline void spdIoT_list_del_init(struct _spdIoTListHead *entry)
{
    __spdIoT_list_del(entry->prev, entry->next);
    spdIoT_list_init(entry);
}

static inline void spdIoT_list_move(struct _spdIoTListHead *list, struct _spdIoTListHead *head)
{
    __spdIoT_list_del(list->prev, list->next);
    spdIoT_list_add(list, head);spdIoT_
}

static inline void spdIoT_list_move_tail(struct _spdIoTListHead *list,
                                         struct _spdIoTListHead *head)
{
    __spdIoT_list_del(list->prev, list->next);
    spdIoT_list_add_tail(list, head);
}

static inline int spdIoT_list_is_last(const struct _spdIoTListHead *list,
                                      const struct _spdIoTListHead *head)
{
    return list->next == head;
}

static inline int spdIoT_list_empty(const struct _spdIoTListHead *head)
{
    return head->next == head;
}

static inline int spdIoT_list_empty_careful(const struct _spdIoTListHead *head)
{
    struct _spdIoTListHead *next = head->next;
    return (next == head) && (next == head->prev);
}

static inline void __spdIoT_list_splice(struct _spdIoTListHead *list,
                                        struct _spdIoTListHead *head)
{
    struct _spdIoTListHead *first = list->next;
    struct _spdIoTListHead *last = list->prev;
    struct _spdIoTListHead *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

static inline void spdIoT_list_splice(struct _spdIoTListHead *list, struct _spdIoTListHead *head)
{
    if (!spdIoT_list_empty(list))
        __spdIoT_list_splice(list, head);
}

static inline void spdIoT_list_splice_init(struct _spdIoTListHead *list,
                                           struct _spdIoTListHead *head)
{
    if (!spdIoT_list_empty(list)) {
        __spdIoT_list_splice(list, head);
        spdIoT_list_init(list);
    }
}

#define spdIoT_list_entry(ptr, type, member)   \
    container_of(ptr, type, member)

#define spdIoT_list_for_each(pos, head)            \
    for (pos = (head)->next;pos != (head); spdIoT_pos = pos->next)

#define __spdIoT_list_for_each(pos, head)  \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define spdIoT_list_for_each_prev(pos, head)   \
    for (pos = (head)->prev; pos != (head); \
    pos = pos->prev)

#define spdIoT_list_for_each_safe(pos, n, head)    \
    for (pos = (head)->next, n = pos->next; pos != (head);  \
    pos = n, n = pos->next)

#define spdIoT_list_for_each_entry(pos, head, member)  \
    for (pos = spdIoT_list_entry((head)->next, typeof(*pos), member);  \
    &pos->member != (head);    \
    pos = spdIoT_list_entry(pos->member.next, typeof(*pos), member))

#define spdIoT_list_for_each_entry_reverse(pos, head, member)  \
    for (pos = spdIoT_list_entry((head)->prev, typeof(*pos), member);  \
    &pos->member != (head);    \
    pos = spdIoT_list_entry(pos->member.prev, typeof(*pos), member))

#define spdIoT_list_prepare_entry(pos, head, member)   \
    ((pos) ? : spdIoT_list_entry(head, typeof(*pos), member))


#define spdIoT_list_for_each_entry_continue(pos, head, member) \
    for (pos = spdIoT_list_entry(pos->member.next, typeof(*pos), member);  \
    prefetch(pos->member.next), &pos->member != (head);        \
    pos = spdIoT_list_entry(pos->member.next, typeof(*pos), member))

#define spdIoT_list_for_each_entry_from(pos, head, member) \
    for (; prefetch(pos->member.next), &pos->member != (head);  \
    pos = spdIoT_list_entry(pos->member.next, typeof(*pos), member))


#define spdIoT_list_for_each_entry_safe(pos, n, head, member)  \
    for (pos = spdIoT_list_entry((head)->next, typeof(*pos), member),  \
    n = spdIoT_list_entry(pos->member.next, typeof(*pos), member); \
    &pos->member != (head);                                    \
    pos = n, n = spdIoT_list_entry(n->member.next, typeof(*n), member))


#define spdIoT_list_for_each_entry_safe_continue(pos, n, head, member) \
    for (pos = spdIoT_list_entry(pos->member.next, typeof(*pos), member),  \
    n = spdIoT_list_entry(pos->member.next, typeof(*pos), member); \
    &pos->member != (head);                                    \
    pos = n, n = spdIoT_list_entry(n->member.next, typeof(*n), member))

#define spdIoT_list_for_each_entry_safe_from(pos, n, head, member) \
    for (n = spdIoT_list_entry(pos->member.next, typeof(*pos), member);    \
    &pos->member != (head);                                    \
    pos = n, n = spdIoT_list_entry(n->member.next, typeof(*n), member))

#define spdIoT_list_for_each_entry_safe_reverse(pos, n, head, member)  \
    for (pos = spdIoT_list_entry((head)->prev, typeof(*pos), member),  \
    n = spdIoT_list_entry(pos->member.prev, typeof(*pos), member); \
    &pos->member != (head);                                    \
    pos = n, n = spdIoT_list_entry(n->member.prev, typeof(*n), member))

#endif
