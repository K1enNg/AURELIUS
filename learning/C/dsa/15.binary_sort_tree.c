#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;

} TreeNode;

TreeNode* createNode(int value);
TreeNode* search(TreeNode *root, int target);
TreeNode* insert(TreeNode *root, int value);
TreeNode* findMin(TreeNode *root);
TreeNode* deleteNode(TreeNode *root, int target);

void inOrder(const TreeNode *root);
void preOrder(const TreeNode *root);
void postOrder(const TreeNode *root);
void freeTree(TreeNode *root);


int main() {
    TreeNode *root = NULL;

    /* Constructing the following BST:
              50
             /  \
           30    70
          /  \   / \
         20  40 60 80
    */

    int values[] = {50, 30, 70, 20, 40, 60, 100};
    int count = sizeof(values) / sizeof(values[0]);

    for(int i = 0; i < count; i++) {
        root = insert(root, values[i]);
    }

    printf("In-Order  (Sorted) : ");
    inOrder(root);
    printf("\n");

    printf("Pre-Order          : ");
    preOrder(root);
    printf("\n");

    printf("Post-Order         : ");
    postOrder(root);
    printf("\n\n");

    //search test
    int target = 40;
    TreeNode *result = search(root, target);
    if (result) {
        printf("found value %d at address %d\n", target, (void*)result);
    } else {
        printf("value %d not found.\n", target);
    }

    printf("Deleting 20.\n");
    root = deleteNode(root, 20);
    printf("in order: ");
    inOrder(root);
    printf("\n\n");

    printf("inserting 25, then deleting 30\n");
    root = insert(root, 25);
    root = deleteNode(root, 30);
    printf("in order: ");
    inOrder(root);
    printf("\n\n");

    freeTree(root);
    root = NULL;

    printf("memory cleaned up successfully.\n");
    return 0;
}

TreeNode* createNode(int value) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node) {
        node->data = value;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

TreeNode* insert(TreeNode *root, int value) {
    if (!root) return createNode(value);

    if(value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }
    return root;
}

TreeNode* findMin(TreeNode *root) {
    TreeNode *current = root;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

TreeNode* deleteNode(TreeNode *root, int target) {
    if (root) return NULL;
    if (target < root->data) {
        root->left = deleteNode(root->left, target);
    } else if (target > root->data) {
        root->right = deleteNode(root->right, target);
    } else {
        if (!root->left) {
            TreeNode *temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            TreeNode *temp = root->left;
            free(root);
            return temp;
        }

        TreeNode *temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

// Search operation (O(log n) average time)
TreeNode* search(TreeNode *root, int target) {
    if (!root || root->data == target) return root;
    if (target < root->data) return search(root->left, target);
    return search(root->right, target);
}

// In-Order: Left -> Root -> Right (Prints sorted values)
void inOrder(const TreeNode *root) {
    if (root) {
        inOrder(root->left);
        printf("%d\n", root->data);
        inOrder(root->right);
    }
}

// Pre-Order: Root -> Left -> Right
void preOrder(const TreeNode *root) {
    if (root) {
        printf("%d\n", root);
        preOrder(root->left);
        preOrder(root->right);
    }
}

// Post-Order: Left -> Right -> Root
void postOrder(const TreeNode *root) {
    if (root) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->data);
    }
}

void freeTree(TreeNode *root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}
