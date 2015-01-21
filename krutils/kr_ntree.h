#ifndef __KR_NTREE_H__
#define __KR_NTREE_H__

#include "kr_alloc.h"
#include "kr_types.h"

typedef struct _kr_ntree_t        T_KRNTree;

/* Tree traverse flags */
typedef enum {
    KR_TRAVERSE_LEAVES     = 1 << 0,
    KR_TRAVERSE_NON_LEAVES = 1 << 1,
    KR_TRAVERSE_ALL        = KR_TRAVERSE_LEAVES | KR_TRAVERSE_NON_LEAVES,
    KR_TRAVERSE_MASK       = 0x03,
    KR_TRAVERSE_LEAFS      = KR_TRAVERSE_LEAVES,
    KR_TRAVERSE_NON_LEAFS  = KR_TRAVERSE_NON_LEAVES
} E_KRTraverseFlags;

/* Tree traverse orders */
typedef enum {
    KR_IN_ORDER,
    KR_PRE_ORDER,
    KR_POST_ORDER,
    KR_LEVEL_ORDER
} E_KRTraverseType;

typedef kr_bool (*T_KRNTreeTraverseFunc)(T_KRNTree *node, kr_pointer data);
typedef void (*T_KRNTreeForeachFunc)(T_KRNTree *node, kr_pointer data);

/**
 * KRCopyFunc:
 * @src: A pointer to the data which should be copied
 * @data: Additional data
 *
 * A function of this signature is used to copy the node data 
 * when doing a deep-copy of a tree.
 *
 * Returns: A pointer to the copy
 *
 * Since: 2.4
 */
typedef kr_pointer (*KRCopyFunc) (kr_constpointer  src, kr_pointer data);

/* N-way tree implementation
*/
struct _kr_ntree_t
{
    kr_pointer      data;
    T_KRNTree      *next;
    T_KRNTree      *prev;
    T_KRNTree      *parent;
    T_KRNTree      *children;
};

/**
 * KR_NTREE_IS_ROOT:
 * @node: a #T_KRNTree
 *
 * Returns %TRUE if a #T_KRNTree is the root of a tree.
 *
 * Returns: %TRUE if the #T_KRNTree is the root of a tree 
 *     (i.e. it has no parent or siblings)
 */
#define KR_NTREE_IS_ROOT(node)  (((T_KRNTree*) (node))->parent == NULL && \
        ((T_KRNTree*) (node))->prev == NULL && \
        ((T_KRNTree*) (node))->next == NULL)

/**
 * KR_NTREE_IS_LEAF:
 * @node: a #T_KRNTree
 *
 * Returns %TRUE if a #T_KRNTree is a leaf node.
 *
 * Returns: %TRUE if the #T_KRNTree is a leaf node 
 *     (i.e. it has no children)
 */
#define KR_NTREE_IS_LEAF(node)    (((T_KRNTree*) (node))->children == NULL)

T_KRNTree*     kr_ntree_new(kr_pointer data);
void     kr_ntree_destroy(T_KRNTree *root);
void     kr_ntree_unlink(T_KRNTree *node);
T_KRNTree*     kr_ntree_copy_deep(T_KRNTree *node, 
        KRCopyFunc copy_func, 
        kr_pointer data);
T_KRNTree*     kr_ntree_copy(T_KRNTree            *node);
T_KRNTree*     kr_ntree_insert(T_KRNTree          *parent,
        kr_int           position,
        T_KRNTree          *node);
T_KRNTree*     kr_ntree_insert_before(T_KRNTree          *parent,
        T_KRNTree          *sibling,
        T_KRNTree          *node);
T_KRNTree*   kr_ntree_insert_after(T_KRNTree            *parent,
        T_KRNTree            *sibling,
        T_KRNTree            *node); 
T_KRNTree*     kr_ntree_prepend(T_KRNTree          *parent,
        T_KRNTree          *node);
kr_uint     kr_ntree_n_nodes(T_KRNTree          *root,
        E_KRTraverseFlags       flags);
T_KRNTree*     kr_ntree_get_root(T_KRNTree          *node);
kr_bool kr_ntree_is_ancestor(T_KRNTree          *node,
        T_KRNTree          *descendant);
kr_uint     kr_ntree_depth(T_KRNTree          *node);
T_KRNTree*     kr_ntree_find(T_KRNTree          *root,
        E_KRTraverseType       order,
        E_KRTraverseFlags       flags,
        kr_pointer       data);

/* convenience macros */
/**
 * kr_ntree_append:
 * @parent: the #T_KRNTree to place the new #T_KRNTree under
 * @node: the #T_KRNTree to insert
 *
 * Inserts a #T_KRNTree as the last child of the given parent.
 *
 * Returns: the inserted #T_KRNTree
 */
#define kr_ntree_append(parent, node)                \
    kr_ntree_insert_before ((parent), NULL, (node))

/**
 * kr_ntree_insert_data:
 * @parent: the #T_KRNTree to place the new #T_KRNTree under
 * @position: the position to place the new #T_KRNTree at. If position is -1, 
 *     the new #T_KRNTree is inserted as the last child of @parent
 * @data: the data for the new #T_KRNTree
 *
 * Inserts a new #T_KRNTree at the given position.
 *
 * Returns: the new #T_KRNTree
 */
#define    kr_ntree_insert_data(parent, position, data)        \
    kr_ntree_insert ((parent), (position), kr_ntree_new (data))

/**
 * kr_ntree_insert_data_before:
 * @parent: the #T_KRNTree to place the new #T_KRNTree under
 * @sibling: the sibling #T_KRNTree to place the new #T_KRNTree before
 * @data: the data for the new #T_KRNTree
 *
 * Inserts a new #T_KRNTree before the given sibling.
 *
 * Returns: the new #T_KRNTree
 */
#define    kr_ntree_insert_data_before(parent, sibling, data)    \
    kr_ntree_insert_before ((parent), (sibling), kr_ntree_new (data))

/**
 * kr_ntree_prepend_data:
 * @parent: the #T_KRNTree to place the new #T_KRNTree under
 * @data: the data for the new #T_KRNTree
 *
 * Inserts a new #T_KRNTree as the first child of the given parent.
 *
 * Returns: the new #T_KRNTree
 */
#define    kr_ntree_prepend_data(parent, data)            \
    kr_ntree_prepend ((parent), kr_ntree_new (data))

/**
 * kr_ntree_append_data:
 * @parent: the #T_KRNTree to place the new #T_KRNTree under
 * @data: the data for the new #T_KRNTree
 *
 * Inserts a new #T_KRNTree as the last child of the given parent.
 *
 * Returns: the new #T_KRNTree
 */
#define    kr_ntree_append_data(parent, data)            \
    kr_ntree_insert_before ((parent), NULL, kr_ntree_new (data))

/* traversal function, assumes that `node' is root
 * (only traverses `node' and its subtree).
 * this function is just a high level interface to
 * low level traversal functions, optimized for speed.
 */
void     kr_ntree_traverse    (T_KRNTree          *root,
        E_KRTraverseType       order,
        E_KRTraverseFlags       flags,
        kr_int           max_depth,
        T_KRNTreeTraverseFunc func,
        kr_pointer       data);

/* return the maximum tree height starting with `node', this is an expensive
 * operation, since we need to visit all nodes. this could be shortened by
 * adding `kr_uint height' to struct _kr_ntree_t, but then again, this is not very
 * often needed, and would make kr_ntree_insert() more time consuming.
 */
kr_uint     kr_ntree_max_height     (T_KRNTree *root);

void     kr_ntree_children_foreach (T_KRNTree          *node,
        E_KRTraverseFlags   flags,
        T_KRNTreeForeachFunc func,
        kr_pointer       data);
void     kr_ntree_reverse_children (T_KRNTree          *node);
kr_uint     kr_ntree_n_children     (T_KRNTree          *node);
T_KRNTree*     kr_ntree_nth_child     (T_KRNTree          *node,
        kr_uint           n);
T_KRNTree*     kr_ntree_last_child     (T_KRNTree          *node);
T_KRNTree*     kr_ntree_find_child     (T_KRNTree          *node,
        E_KRTraverseFlags   flags,
        kr_pointer       data);
kr_int     kr_ntree_child_position     (T_KRNTree          *node,
        T_KRNTree          *child);
kr_int     kr_ntree_child_index     (T_KRNTree          *node,
        kr_pointer       data);

T_KRNTree*     kr_ntree_first_sibling     (T_KRNTree          *node);
T_KRNTree*     kr_ntree_last_sibling     (T_KRNTree          *node);

/**
 * kr_ntree_prev_sibling:
 * @node: a #T_KRNTree
 *
 * Gets the previous sibling of a #T_KRNTree.
 *
 * Returns: the previous sibling of @node, or %NULL if @node is the first
 *     node or %NULL
 */
#define     kr_ntree_prev_sibling(node)    ((node) ? \
        ((T_KRNTree*) (node))->prev : NULL)

/**
 * kr_ntree_next_sibling:
 * @node: a #T_KRNTree
 *
 * Gets the next sibling of a #T_KRNTree.
 *
 * Returns: the next sibling of @node, or %NULL if @node is the last node
 *     or %NULL
 */
#define     kr_ntree_next_sibling(node)    ((node) ? \
        ((T_KRNTree*) (node))->next : NULL)

/**
 * kr_ntree_first_child:
 * @node: a #T_KRNTree
 *
 * Gets the first child of a #T_KRNTree.
 *
 * Returns: the first child of @node, or %NULL if @node is %NULL 
 *     or has no children
 */
#define     kr_ntree_first_child(node)    ((node) ? \
        ((T_KRNTree*) (node))->children : NULL)


#endif /* __KR_NTREE_H__ */
