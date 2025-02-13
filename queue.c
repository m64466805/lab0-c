#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head) {
        return;
    }
    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list)
        q_release_element(entry);
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }
    element_t *new = malloc(sizeof(element_t));
    if (!new) {
        return false;
    }
    new->value = test_strdup(s);
    if (!new->value) {
        q_release_element(new);
        return false;
    }
    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }
    element_t *new = malloc(sizeof(element_t));
    if (!new) {
        return false;
    }
    new->value = test_strdup(s);
    if (!new->value) {
        q_release_element(new);
        return false;
    }
    list_add_tail(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *node = list_entry(head->next, element_t, list);
    list_del(head->next);
    if (sp && node) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *node = list_entry(head->prev, element_t, list);
    list_del(head->prev);
    if (sp && node) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int qsize = 0;
    if (head == NULL || list_empty(head)) {
        return 0;
    }
    struct list_head *node;
    list_for_each (node, head)
        qsize++;
    return qsize;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head)) {
        return false;
    }
    struct list_head *first = head->next;
    struct list_head *second = head->prev;
    while (first != second && first->next != second) {
        first = first->next;
        second = second->prev;
    }
    element_t *node = list_entry(first, element_t, list);
    list_del(first);
    q_release_element(node);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head)) {
        return false;
    }
    struct list_head *cur = head->next;
    struct list_head *next = cur->next;
    bool dup = false;
    while (cur != head && next != head) {
        element_t *cur_entry = list_entry(cur, element_t, list);
        element_t *next_entry = list_entry(next, element_t, list);
        while (next != head && !strcmp(cur_entry->value, next_entry->value)) {
            list_del(next);
            q_release_element(next_entry);
            next = cur->next;
            next_entry = list_entry(next, element_t, list);
            dup = true;
        }
        if (dup) {
            list_del(cur);
            q_release_element(cur_entry);
            dup = false;
        }
        cur = next;
        next = cur->next;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *cur = head->next;
    while (cur != head && cur->next != head) {
        list_move(cur, cur->next);
        cur = cur->next;
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *node;
    struct list_head *safe;
    list_for_each_safe (node, safe, head)
        list_move(node, head);
    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head)) {
        return;
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}
/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head)) {
        return 0;
    }
    int num = 0;
    struct list_head *cur = head->prev;
    while (cur != head) {
        num++;
        if (cur->prev != head) {
            break;
        }
        element_t *cur_entry = list_entry(cur, element_t, list);
        element_t *prev_entry = list_entry(cur->prev, element_t, list);
        if (strcmp(cur_entry->value, prev_entry->value) < 0) {
            list_del(&prev_entry->list);
            q_release_element(prev_entry);
            num--;
        } else {
            cur = cur->prev;
        }
    }
    return num;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head)) {
        return 0;
    }
    int num = 0;
    struct list_head *cur = head->prev;
    while (cur != head) {
        num++;
        if (cur->prev != head) {
            break;
        }
        element_t *cur_entry = list_entry(cur, element_t, list);
        element_t *prev_entry = list_entry(cur->prev, element_t, list);
        if (strcmp(cur_entry->value, prev_entry->value) > 0) {
            list_del(&prev_entry->list);
            q_release_element(prev_entry);
            num--;
        } else {
            cur = cur->prev;
        }
    }
    return num;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
