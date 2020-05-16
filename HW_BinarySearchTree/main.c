#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "error.h"
#include "bstree.h"

#define process_err(msg)     \
    if (!is_success(err)) {  \
        handle_error(err);   \
        if (is_error(err)) { \
            puts(msg);     \
            return err;      \
        }                    \
    }            

#define NODE_COUNT 7
const int   node_keys  [NODE_COUNT] = { 620, 550, 530, 563, 730, 658, 740};
const Value node_values[NODE_COUNT] = { 111, 222, 333, 444, 555, 666, 777 };

int main() {
    Error err = E_Success;

    puts("Creating my own tree object...");
    BSTree *my_tree, *node;
    err = init_tree(500, 123, &my_tree);
    process_err("Oh godness my tree was not initialised =(")

    print_tree(my_tree);
    
    for (uint8_t i = 0; i < NODE_COUNT; i++) {
        err = init_tree(node_keys[i], node_values[i], &node);
        process_err("New node was not initialised =((");

        err = add_node(my_tree, node);
        process_err("Node was not added =(");
    }

    print_tree(my_tree);

    puts("Trying to delete node (key 620)");

    err = delete_node(my_tree, 620);
    process_err("Oh no! Node was not deleted =(");

    print_tree(my_tree);

    err = min_node(my_tree, &node);
    process_err("Could not get min node =(");

    printf("Minimal key of tree is: %d\n", node->key);

    err = max_node(my_tree, &node);
    process_err("Could not get max node =(");

    printf("Maximal key of tree is: %d\n", node->key);

    puts("Let's delete the tree!");
    fini_tree(my_tree);

    puts("Tree deleted!");

    return err;
}