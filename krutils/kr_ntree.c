/*
 * MT safe
 */

#include "kr_ntree.h"

#include <assert.h>

/**
 * SECTION:trees-nary
 * @title: N-ary Trees
 * @short_description: trees of data with any number of branches
 *
 * The #T_KRNTree struct and its associated functions provide a N-ary tree
 * data structure, where nodes in the tree can contain arbitrary data.
 *
 * To create a new tree use kr_ntree_new().
 *
 * To insert a node into a tree use kr_ntree_insert(),
 * kr_ntree_insert_before(), kr_ntree_append() and kr_ntree_prepend().
 *
 * To create a new node and insert it into a tree use
 * kr_ntree_insert_data(), kr_ntree_insert_data_before(),
 * kr_ntree_append_data() and kr_ntree_prepend_data().
 *
 * To reverse the children of a node use kr_ntree_reverse_children().
 *
 * To find a node use kr_ntree_get_root(), kr_ntree_find(),
 * kr_ntree_find_child(), kr_ntree_child_index(), kr_ntree_child_position(),
 * kr_ntree_first_child(), kr_ntree_last_child(), kr_ntree_nth_child(),
 * kr_ntree_first_sibling(), kr_ntree_prev_sibling(), kr_ntree_next_sibling()
 * or kr_ntree_last_sibling().
 *
 * To get information about a node or tree use KR_NTREE_IS_LEAF(),
 * KR_NTREE_IS_ROOT(), kr_ntree_depth(), kr_ntree_n_nodes(),
 * kr_ntree_n_children(), kr_ntree_is_ancestor() or kr_ntree_max_height().
 *
 * To traverse a tree, calling a function for each node visited in the
 * traversal, use kr_ntree_traverse() or kr_ntree_children_foreach().
 *
 * To remove a node or subtree from a tree use kr_ntree_unlink() or
 * kr_ntree_destroy().
 **/


/* --- functions --- */
/**
 * kr_ntree_new:
 * @data: the data of the new node
 *
 * Creates a new #T_KRNTree containing the given data.
 * Used to create the first node in a tree.
 *
 * Returns: a new #T_KRNTree
 */
T_KRNTree* kr_ntree_new(kr_pointer data)
{
    T_KRNTree *node = kr_calloc(sizeof(T_KRNTree));
    node->data = data;
    return node;
}

static void kr_ntrees_free(T_KRNTree *node)
{
    while (node)
    {
        T_KRNTree *next = node->next;
        if (node->children)
            kr_ntrees_free (node->children);
        kr_free (node);
        node = next;
    }
}

/**
 * kr_ntree_destroy:
 * @root: the root of the tree/subtree to destroy
 *
 * Removes @root and its children from the tree, freeing any memory
 * allocated.
 */
void kr_ntree_destroy(T_KRNTree *root)
{
    if (root == NULL) return;

    if (!KR_NTREE_IS_ROOT (root))
        kr_ntree_unlink (root);

    kr_ntrees_free (root);
}

/**
 * kr_ntree_unlink:
 * @node: the #T_KRNTree to unlink, which becomes the root of a new tree
 *
 * Unlinks a #T_KRNTree from a tree, resulting in two separate trees.
 */
void kr_ntree_unlink(T_KRNTree *node)
{
    if (node == NULL) return;

    if (node->prev)
        node->prev->next = node->next;
    else if (node->parent)
        node->parent->children = node->next;
    node->parent = NULL;
    if (node->next) {
        node->next->prev = node->prev;
        node->next = NULL;
    }
    node->prev = NULL;
}

/**
 * kr_ntree_copy_deep:
 * @node: a #T_KRNTree
 * @copy_func: the function which is called to copy the data inside each node,
 *   or %NULL to use the original data.
 * @data: data to pass to @copy_func
 * 
 * Recursively copies a #T_KRNTree and its data.
 * 
 * Return value: a new #T_KRNTree containing copies of the data in @node.
 *
 **/
T_KRNTree* 
kr_ntree_copy_deep(T_KRNTree *node, KRCopyFunc copy_func, kr_pointer data)
{
    T_KRNTree *new_node = NULL;

    if (copy_func == NULL)
        return kr_ntree_copy (node);

    if (node) {
        T_KRNTree *child, *new_child;

        new_node = kr_ntree_new (copy_func (node->data, data));

        for (child = kr_ntree_last_child (node); child; child = child->prev) {
            new_child = kr_ntree_copy_deep (child, copy_func, data);
            kr_ntree_prepend (new_node, new_child);
        }
    }

    return new_node;
}

/**
 * kr_ntree_copy:
 * @node: a #T_KRNTree
 *
 * Recursively copies a #T_KRNTree (but does not deep-copy the data inside the 
 * nodes, see kr_ntree_copy_deep() if you need that).
 *
 * Returns: a new #T_KRNTree containing the same data pointers
 */
T_KRNTree* kr_ntree_copy(T_KRNTree *node)
{
    T_KRNTree *new_node = NULL;

    if (node) {
        T_KRNTree *child;

        new_node = kr_ntree_new (node->data);

        for (child = kr_ntree_last_child (node); child; child = child->prev)
            kr_ntree_prepend (new_node, kr_ntree_copy (child));
    }

    return new_node;
}

/**
 * kr_ntree_insert:
 * @parent: the #T_KRNTree to place @node under
 * @position: the position to place @node at, with respect to its siblings
 *     If position is -1, @node is inserted as the last child of @parent
 * @node: the #T_KRNTree to insert
 *
 * Inserts a #T_KRNTree beneath the parent at the given position.
 *
 * Returns: the inserted #T_KRNTree
 */
T_KRNTree* 
kr_ntree_insert(T_KRNTree *parent, kr_int position, T_KRNTree *node)
{
    if (parent == NULL) return node;
    if (node == NULL) return node;
    if (!KR_NTREE_IS_ROOT(node)) return node;

    if (position > 0)
        return kr_ntree_insert_before (parent,
                kr_ntree_nth_child (parent, position),
                node);
    else if (position == 0)
        return kr_ntree_prepend (parent, node);
    else /* if (position < 0) */
        return kr_ntree_append (parent, node);
}

/**
 * kr_ntree_insert_before:
 * @parent: the #T_KRNTree to place @node under
 * @sibling: the sibling #T_KRNTree to place @node before. 
 *     If sibling is %NULL, the node is inserted as the last child of @parent.
 * @node: the #T_KRNTree to insert
 *
 * Inserts a #T_KRNTree beneath the parent before the given sibling.
 *
 * Returns: the inserted #T_KRNTree
 */
T_KRNTree*
kr_ntree_insert_before(T_KRNTree *parent, T_KRNTree *sibling, T_KRNTree *node)
{
    if (parent == NULL) return node;
    if (node == NULL) return node;
    if (!KR_NTREE_IS_ROOT(node)) return node;

    if (sibling)
        if (sibling->parent != parent) return node;

    node->parent = parent;

    if (sibling) {
        if (sibling->prev) {
            node->prev = sibling->prev;
            node->prev->next = node;
            node->next = sibling;
            sibling->prev = node;
        } else {
            node->parent->children = node;
            node->next = sibling;
            sibling->prev = node;
        }
    } else {
        if (parent->children) {
            sibling = parent->children;
            while (sibling->next)
                sibling = sibling->next;
            node->prev = sibling;
            sibling->next = node;
        } else
            node->parent->children = node;
    }

    return node;
}

/**
 * kr_ntree_insert_after:
 * @parent: the #T_KRNTree to place @node under
 * @sibling: the sibling #T_KRNTree to place @node after. 
 *     If sibling is %NULL, the node is inserted as the first child of @parent.
 * @node: the #T_KRNTree to insert
 *
 * Inserts a #T_KRNTree beneath the parent after the given sibling.
 *
 * Returns: the inserted #T_KRNTree
 */
T_KRNTree*
kr_ntree_insert_after(T_KRNTree *parent, T_KRNTree *sibling, T_KRNTree *node)
{
    if (parent == NULL) return node;
    if (node == NULL) return node;
    if (!KR_NTREE_IS_ROOT(node)) return node;

    if (sibling)
        if (sibling->parent != parent) return node;

    node->parent = parent;

    if (sibling) {
        if (sibling->next) {
            sibling->next->prev = node;
        }
        node->next = sibling->next;
        node->prev = sibling;
        sibling->next = node;
    } else {
        if (parent->children) {
            node->next = parent->children;
            parent->children->prev = node;
        }
        parent->children = node;
    }

    return node;
}

/**
 * kr_ntree_prepend:
 * @parent: the #T_KRNTree to place the new #T_KRNTree under
 * @node: the #T_KRNTree to insert
 *
 * Inserts a #T_KRNTree as the first child of the given parent.
 *
 * Returns: the inserted #T_KRNTree
 */
T_KRNTree* kr_ntree_prepend(T_KRNTree *parent, T_KRNTree *node)
{
    if (parent == NULL) return node;

    return kr_ntree_insert_before (parent, parent->children, node);
}

/**
 * kr_ntree_get_root:
 * @node: a #T_KRNTree
 *
 * Gets the root of a tree.
 *
 * Returns: the root of the tree
 */
T_KRNTree* kr_ntree_get_root(T_KRNTree *node)
{
    if (node == NULL) return NULL;

    while (node->parent)
        node = node->parent;

    return node;
}

/**
 * kr_ntree_is_ancestor:
 * @node: a #T_KRNTree
 * @descendant: a #T_KRNTree
 *
 * Returns %TRUE if @node is an ancestor of @descendant.
 * This is true if node is the parent of @descendant, 
 * or if node is the grandparent of @descendant etc.
 *
 * Returns: %TRUE if @node is an ancestor of @descendant
 */
kr_bool kr_ntree_is_ancestor(T_KRNTree *node, T_KRNTree *descendant)
{
    if (node == NULL) return FALSE;
    if (descendant == NULL) return FALSE;

    while (descendant)
    {
        if (descendant->parent == node)
            return TRUE;

        descendant = descendant->parent;
    }

    return FALSE;
}

/**
 * kr_ntree_depth:
 * @node: a #T_KRNTree
 *
 * Gets the depth of a #T_KRNTree.
 *
 * If @node is %NULL the depth is 0. The root node has a depth of 1.
 * For the children of the root node the depth is 2. And so on.
 *
 * Returns: the depth of the #T_KRNTree
 */
kr_uint kr_ntree_depth (T_KRNTree *node)
{
    kr_uint depth = 0;

    while (node)
    {
        depth++;
        node = node->parent;
    }

    return depth;
}

/**
 * kr_ntree_reverse_children:
 * @node: a #T_KRNTree.
 *
 * Reverses the order of the children of a #T_KRNTree.
 * (It doesn't change the order of the grandchildren.)
 */
void kr_ntree_reverse_children (T_KRNTree *node)
{
    T_KRNTree *child;
    T_KRNTree *last;

    if (node == NULL) return;

    child = node->children;
    last = NULL;
    while (child)
    {
        last = child;
        child = last->next;
        last->next = last->prev;
        last->prev = child;
    }
    node->children = last;
}

/**
 * kr_ntree_max_height:
 * @root: a #T_KRNTree
 *
 * Gets the maximum height of all branches beneath a #T_KRNTree.
 * This is the maximum distance from the #T_KRNTree to all leaf nodes.
 *
 * If @root is %NULL, 0 is returned. If @root has no children, 
 * 1 is returned. If @root has children, 2 is returned. And so on.
 *
 * Returns: the maximum height of the tree beneath @root
 */
kr_uint kr_ntree_max_height(T_KRNTree *root)
{
    T_KRNTree *child;
    kr_uint max_height = 0;

    if (!root)
        return 0;

    child = root->children;
    while (child)
    {
        kr_uint tmp_height;

        tmp_height = kr_ntree_max_height (child);
        if (tmp_height > max_height)
            max_height = tmp_height;
        child = child->next;
    }

    return max_height + 1;
}

static kr_bool
kr_ntree_traverse_pre_order (T_KRNTree        *node,
        E_KRTraverseFlags    flags,
        T_KRNTreeTraverseFunc func,
        kr_pointer         data)
{
    if (node->children) {
        T_KRNTree *child;

        if ((flags & KR_TRAVERSE_NON_LEAFS) &&
                func (node, data))
            return TRUE;

        child = node->children;
        while (child)
        {
            T_KRNTree *current;

            current = child;
            child = current->next;
            if (kr_ntree_traverse_pre_order (current, flags, func, data))
                return TRUE;
        }
    }
    else if ((flags & KR_TRAVERSE_LEAFS) &&
            func (node, data))
        return TRUE;

    return FALSE;
}

static kr_bool
kr_ntree_depth_traverse_pre_order (T_KRNTree          *node,
        E_KRTraverseFlags       flags,
        kr_uint           depth,
        T_KRNTreeTraverseFunc func,
        kr_pointer       data)
{
    if (node->children) {
        T_KRNTree *child;

        if ((flags & KR_TRAVERSE_NON_LEAFS) &&
                func (node, data))
            return TRUE;

        depth--;
        if (!depth)
            return FALSE;

        child = node->children;
        while (child)
        {
            T_KRNTree *current;

            current = child;
            child = current->next;
            if (kr_ntree_depth_traverse_pre_order (current, flags, depth, func, data))
                return TRUE;
        }
    }
    else if ((flags & KR_TRAVERSE_LEAFS) && func (node, data))
        return TRUE;

    return FALSE;
}

static kr_bool
kr_ntree_traverse_post_order (T_KRNTree         *node,
        E_KRTraverseFlags    flags,
        T_KRNTreeTraverseFunc func,
        kr_pointer          data)
{
    if (node->children) {
        T_KRNTree *child;

        child = node->children;
        while (child)
        {
            T_KRNTree *current;

            current = child;
            child = current->next;
            if (kr_ntree_traverse_post_order (current, flags, func, data))
                return TRUE;
        }

        if ((flags & KR_TRAVERSE_NON_LEAFS) && func (node, data))
            return TRUE;

    }
    else if ((flags & KR_TRAVERSE_LEAFS) && func (node, data))
        return TRUE;

    return FALSE;
}

static kr_bool
kr_ntree_depth_traverse_post_order (T_KRNTree           *node,
        E_KRTraverseFlags    flags,
        kr_uint            depth,
        T_KRNTreeTraverseFunc func,
        kr_pointer        data)
{
    if (node->children) {
        depth--;
        if (depth) {
            T_KRNTree *child;

            child = node->children;
            while (child)
            {
                T_KRNTree *current;

                current = child;
                child = current->next;
                if (kr_ntree_depth_traverse_post_order (current, flags, depth, func, data))
                    return TRUE;
            }
        }

        if ((flags & KR_TRAVERSE_NON_LEAFS) && func (node, data))
            return TRUE;

    }
    else if ((flags & KR_TRAVERSE_LEAFS) && func (node, data))
        return TRUE;

    return FALSE;
}

static kr_bool
kr_ntree_traverse_in_order (T_KRNTree           *node,
        E_KRTraverseFlags    flags,
        T_KRNTreeTraverseFunc func,
        kr_pointer        data)
{
    if (node->children) {
        T_KRNTree *child;
        T_KRNTree *current;

        child = node->children;
        current = child;
        child = current->next;

        if (kr_ntree_traverse_in_order (current, flags, func, data))
            return TRUE;

        if ((flags & KR_TRAVERSE_NON_LEAFS) && func (node, data))
            return TRUE;

        while (child)
        {
            current = child;
            child = current->next;
            if (kr_ntree_traverse_in_order (current, flags, func, data))
                return TRUE;
        }
    }
    else if ((flags & KR_TRAVERSE_LEAFS) && func (node, data))
        return TRUE;

    return FALSE;
}

static kr_bool
kr_ntree_depth_traverse_in_order (T_KRNTree         *node,
        E_KRTraverseFlags      flags,
        kr_uint          depth,
        T_KRNTreeTraverseFunc func,
        kr_pointer      data)
{
    if (node->children) {
        depth--;
        if (depth) {
            T_KRNTree *child;
            T_KRNTree *current;

            child = node->children;
            current = child;
            child = current->next;

            if (kr_ntree_depth_traverse_in_order (current, flags, depth, func, data))
                return TRUE;

            if ((flags & KR_TRAVERSE_NON_LEAFS) && func (node, data))
                return TRUE;

            while (child)
            {
                current = child;
                child = current->next;
                if (kr_ntree_depth_traverse_in_order (current, flags, depth, func, data))
                    return TRUE;
            }
        } else if ((flags & KR_TRAVERSE_NON_LEAFS) && func (node, data))
            return TRUE;
    } else if ((flags & KR_TRAVERSE_LEAFS) && func (node, data))
        return TRUE;

    return FALSE;
}

static kr_bool
kr_ntree_traverse_level (T_KRNTree         *node,
        E_KRTraverseFlags      flags,
        kr_uint          level,
        T_KRNTreeTraverseFunc  func,
        kr_pointer              data,
        kr_bool          *more_levels)
{
    if (level == 0) {
        if (node->children) {
            *more_levels = TRUE;
            return (flags & KR_TRAVERSE_NON_LEAFS) && func (node, data);
        } else {
            return (flags & KR_TRAVERSE_LEAFS) && func (node, data);
        }
    } else {
        node = node->children;

        while (node) {
            if (kr_ntree_traverse_level (node, flags, level - 1, func, data, more_levels))
                return TRUE;

            node = node->next;
        }
    }

    return FALSE;
}

static kr_bool
kr_ntree_depth_traverse_level (T_KRNTree             *node,
        E_KRTraverseFlags    flags,
        kr_uint        depth,
        T_KRNTreeTraverseFunc  func,
        kr_pointer            data)
{
    kr_uint level;
    kr_bool more_levels;

    level = 0;  
    while (level != depth) 
    {
        more_levels = FALSE;
        if (kr_ntree_traverse_level (node, flags, level, func, data, &more_levels))
            return TRUE;
        if (!more_levels)
            break;
        level++;
    }
    return FALSE;
}

/**
 * kr_ntree_traverse:
 * @root: the root #T_KRNTree of the tree to traverse
 * @order: the order in which nodes are visited - %KR_IN_ORDER, 
 *     %KR_PRE_ORDER, %KR_POST_ORDER, or %KR_LEVEL_ORDER.
 * @flags: which types of children are to be visited, one of 
 *     %KR_TRAVERSE_ALL, %KR_TRAVERSE_LEAVES and %KR_TRAVERSE_NON_LEAVES
 * @max_depth: the maximum depth of the traversal. Nodes below this
 *     depth will not be visited. If max_depth is -1 all nodes in 
 *     the tree are visited. If depth is 1, only the root is visited. 
 *     If depth is 2, the root and its children are visited. And so on.
 * @func: the function to call for each visited #T_KRNTree
 * @data: user data to pass to the function
 *
 * Traverses a tree starting at the given root #T_KRNTree.
 * It calls the given function for each node visited.
 * The traversal can be halted at any point by returning %TRUE from @func.
 */
/**
 * E_KRTraverseFlags:
 * @KR_TRAVERSE_LEAVES: only leaf nodes should be visited. This name has
 *                     been introduced in 2.6, for older version use
 *                     %KR_TRAVERSE_LEAFS.
 * @KR_TRAVERSE_NON_LEAVES: only non-leaf nodes should be visited. This
 *                         name has been introduced in 2.6, for older
 *                         version use %KR_TRAVERSE_NON_LEAFS.
 * @KR_TRAVERSE_ALL: all nodes should be visited.
 * @KR_TRAVERSE_MASK: a mask of all traverse flags.
 * @KR_TRAVERSE_LEAFS: identical to %KR_TRAVERSE_LEAVES.
 * @KR_TRAVERSE_NON_LEAFS: identical to %KR_TRAVERSE_NON_LEAVES.
 *
 * Specifies which nodes are visited during several of the tree
 * functions, including kr_ntree_traverse() and kr_ntree_find().
 **/
/**
 * T_KRNTreeTraverseFunc:
 * @node: a #T_KRNTree.
 * @data: user data passed to kr_ntree_traverse().
 * @Returns: %TRUE to stop the traversal.
 *
 * Specifies the type of function passed to kr_ntree_traverse(). The
 * function is called with each of the nodes visited, together with the
 * user data passed to kr_ntree_traverse(). If the function returns
 * %TRUE, then the traversal is stopped.
 **/
void
kr_ntree_traverse (T_KRNTree          *root,
        E_KRTraverseType       order,
        E_KRTraverseFlags       flags,
        kr_int           depth,
        T_KRNTreeTraverseFunc func,
        kr_pointer       data)
{
    if (root == NULL) return;
    if (func == NULL) return;
    if (order > KR_LEVEL_ORDER) return;
    if (flags > KR_TRAVERSE_MASK) return;
    if (depth != -1 && depth < 0) return;

    switch (order)
    {
        case KR_PRE_ORDER:
            if (depth < 0)
                kr_ntree_traverse_pre_order (root, flags, func, data);
            else
                kr_ntree_depth_traverse_pre_order (root, flags, depth, func, data);
            break;
        case KR_POST_ORDER:
            if (depth < 0)
                kr_ntree_traverse_post_order (root, flags, func, data);
            else
                kr_ntree_depth_traverse_post_order (root, flags, depth, func, data);
            break;
        case KR_IN_ORDER:
            if (depth < 0)
                kr_ntree_traverse_in_order (root, flags, func, data);
            else
                kr_ntree_depth_traverse_in_order (root, flags, depth, func, data);
            break;
        case KR_LEVEL_ORDER:
            kr_ntree_depth_traverse_level (root, flags, depth, func, data);
            break;
    }
}

static kr_bool kr_ntree_find_func(T_KRNTree *node, kr_pointer data)
{
    kr_pointer *d = data;

    if (*d != node->data)
        return FALSE;

    *(++d) = node;

    return TRUE;
}

/**
 * kr_ntree_find:
 * @root: the root #T_KRNTree of the tree to search
 * @order: the order in which nodes are visited - %KR_IN_ORDER, 
 *     %KR_PRE_ORDER, %KR_POST_ORDER, or %KR_LEVEL_ORDER
 * @flags: which types of children are to be searched, one of 
 *     %KR_TRAVERSE_ALL, %KR_TRAVERSE_LEAVES and %KR_TRAVERSE_NON_LEAVES
 * @data: the data to find
 *
 * Finds a #T_KRNTree in a tree.
 *
 * Returns: the found #T_KRNTree, or %NULL if the data is not found
 */
T_KRNTree*
kr_ntree_find (T_KRNTree  *root,
        E_KRTraverseType   order,
        E_KRTraverseFlags  flags,
        kr_pointer        data)
{
    kr_pointer d[2];

    if (root == NULL) return NULL;
    if (order > KR_LEVEL_ORDER) return NULL;
    if (flags > KR_TRAVERSE_MASK) return NULL;

    d[0] = data;
    d[1] = NULL;

    kr_ntree_traverse (root, order, flags, -1, kr_ntree_find_func, d);

    return d[1];
}

static void
kr_ntree_count_func(T_KRNTree *node, E_KRTraverseFlags flags, kr_uint *n)
{
    if (node->children) {
        T_KRNTree *child;

        if (flags & KR_TRAVERSE_NON_LEAFS)
            (*n)++;

        child = node->children;
        while (child) {
            kr_ntree_count_func (child, flags, n);
            child = child->next;
        }
    }
    else if (flags & KR_TRAVERSE_LEAFS)
        (*n)++;
}

/**
 * kr_ntree_n_nodes:
 * @root: a #T_KRNTree
 * @flags: which types of children are to be counted, one of 
 *     %KR_TRAVERSE_ALL, %KR_TRAVERSE_LEAVES and %KR_TRAVERSE_NON_LEAVES
 *
 * Gets the number of nodes in a tree.
 *
 * Returns: the number of nodes in the tree
 */
kr_uint kr_ntree_n_nodes(T_KRNTree *root, E_KRTraverseFlags  flags)
{
    kr_uint n = 0;

    if (root == NULL) return 0;
    if (flags > KR_TRAVERSE_MASK) return 0;

    kr_ntree_count_func (root, flags, &n);

    return n;
}

/**
 * kr_ntree_last_child:
 * @node: a #T_KRNTree (must not be %NULL)
 *
 * Gets the last child of a #T_KRNTree.
 *
 * Returns: the last child of @node, or %NULL if @node has no children
 */
T_KRNTree* kr_ntree_last_child (T_KRNTree *node)
{
    if (node == NULL) return NULL;

    node = node->children;
    if (node)
        while (node->next)
            node = node->next;

    return node;
}

/**
 * kr_ntree_nth_child:
 * @node: a #T_KRNTree
 * @n: the index of the desired child
 *
 * Gets a child of a #T_KRNTree, using the given index.
 * The first child is at index 0. If the index is 
 * too big, %NULL is returned.
 *
 * Returns: the child of @node at index @n
 */
T_KRNTree* kr_ntree_nth_child (T_KRNTree *node, kr_uint n)
{
    if (node == NULL) return NULL;

    node = node->children;
    if (node)
        while ((n-- > 0) && node)
            node = node->next;

    return node;
}

/**
 * kr_ntree_n_children:
 * @node: a #T_KRNTree
 *
 * Gets the number of children of a #T_KRNTree.
 *
 * Returns: the number of children of @node
 */
kr_uint kr_ntree_n_children (T_KRNTree *node)
{
    kr_uint n = 0;

    if (node == NULL) return 0;

    node = node->children;
    while (node) {
        n++;
        node = node->next;
    }

    return n;
}

/**
 * kr_ntree_find_child:
 * @node: a #T_KRNTree
 * @flags: which types of children are to be searched, one of 
 *     %KR_TRAVERSE_ALL, %KR_TRAVERSE_LEAVES and %KR_TRAVERSE_NON_LEAVES
 * @data: the data to find
 *
 * Finds the first child of a #T_KRNTree with the given data.
 *
 * Returns: the found child #T_KRNTree, or %NULL if the data is not found
 */
T_KRNTree* 
kr_ntree_find_child(T_KRNTree *node, E_KRTraverseFlags flags, kr_pointer data)
{
    if (node == NULL) return NULL;
    if (flags > KR_TRAVERSE_MASK) return NULL;

    node = node->children;
    while (node) {
        if (node->data == data) {
            if (KR_NTREE_IS_LEAF (node)) {
                if (flags & KR_TRAVERSE_LEAFS)
                    return node;
            } else {
                if (flags & KR_TRAVERSE_NON_LEAFS)
                    return node;
            }
        }
        node = node->next;
    }

    return NULL;
}

/**
 * kr_ntree_child_position:
 * @node: a #T_KRNTree
 * @child: a child of @node
 *
 * Gets the position of a #T_KRNTree with respect to its siblings.
 * @child must be a child of @node. The first child is numbered 0, 
 * the second 1, and so on.
 *
 * Returns: the position of @child with respect to its siblings
 */
kr_int kr_ntree_child_position (T_KRNTree *node, T_KRNTree *child)
{
    kr_uint n = 0;

    if (node != NULL) return -1;
    if (child != NULL) return -1;
    if (child->parent != node) return -1;

    node = node->children;
    while (node) {
        if (node == child)
            return n;
        n++;
        node = node->next;
    }

    return -1;
}

/**
 * kr_ntree_child_index:
 * @node: a #T_KRNTree
 * @data: the data to find
 *
 * Gets the position of the first child of a #T_KRNTree 
 * which contains the given data.
 *
 * Returns: the index of the child of @node which contains 
 *     @data, or -1 if the data is not found
 */
kr_int kr_ntree_child_index (T_KRNTree *node, kr_pointer data)
{
    kr_uint n = 0;

    if (node == NULL) return -1;

    node = node->children;
    while (node) {
        if (node->data == data)
            return n;
        n++;
        node = node->next;
    }

    return -1;
}

/**
 * kr_ntree_first_sibling:
 * @node: a #T_KRNTree
 *
 * Gets the first sibling of a #T_KRNTree.
 * This could possibly be the node itself.
 *
 * Returns: the first sibling of @node
 */
T_KRNTree* kr_ntree_first_sibling (T_KRNTree *node)
{
    if (node == NULL) return NULL;

    if (node->parent)
        return node->parent->children;

    while (node->prev)
        node = node->prev;

    return node;
}

/**
 * kr_ntree_last_sibling:
 * @node: a #T_KRNTree
 *
 * Gets the last sibling of a #T_KRNTree.
 * This could possibly be the node itself.
 *
 * Returns: the last sibling of @node
 */
T_KRNTree* kr_ntree_last_sibling (T_KRNTree *node)
{
    if (node == NULL) return NULL;

    while (node->next)
        node = node->next;

    return node;
}

/**
 * kr_ntree_children_foreach:
 * @node: a #T_KRNTree
 * @flags: which types of children are to be visited, one of 
 *     %KR_TRAVERSE_ALL, %KR_TRAVERSE_LEAVES and %KR_TRAVERSE_NON_LEAVES
 * @func: the function to call for each visited node
 * @data: user data to pass to the function
 *
 * Calls a function for each of the children of a #T_KRNTree.
 * Note that it doesn't descend beneath the child nodes.
 */
/**
 * T_KRNTreeForeachFunc:
 * @node: a #T_KRNTree.
 * @data: user data passed to kr_ntree_children_foreach().
 *
 * Specifies the type of function passed to kr_ntree_children_foreach().
 * The function is called with each child node, together with the user
 * data passed to kr_ntree_children_foreach().
 **/
void
kr_ntree_children_foreach (T_KRNTree          *node,
        E_KRTraverseFlags       flags,
        T_KRNTreeForeachFunc  func,
        kr_pointer       data)
{
    if (node == NULL) return;
    if (flags > KR_TRAVERSE_MASK) return;
    if (func == NULL) return;

    node = node->children;
    while (node)
    {
        T_KRNTree *current;

        current = node;
        node = current->next;
        if (KR_NTREE_IS_LEAF (current)) {
            if (flags & KR_TRAVERSE_LEAFS)
                func (current, data);
        } else {
            if (flags & KR_TRAVERSE_NON_LEAFS)
                func (current, data);
        }
    }
}
