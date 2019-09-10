#include<iostream>
#include<stdexcept>

namespace RBTree{

    struct TreeNode{
        int data;   // value contained by the node
        char color;  // R = red, B = black
        bool is_null;
        TreeNode *left, *right, *parent;
        TreeNode() {
            left = NULL, right = NULL, parent = NULL, color='B';
            is_null = false;
        }
    };

    class Tree{
        private:
            TreeNode *root;
            TreeNode *null_node;                        // pointer to a null tree node
            void createLeaves(TreeNode *ptr);           // creates empty black leaf nodes for ptr
            void deleteLeaves(TreeNode *ptr);           // delete empty black leaf nodes for ptr
            void rightRotate(TreeNode *root, bool changeColor); // right rotation at root node
            void leftRotate(TreeNode *root, bool changeColor);  // left rotation at left node
            void printInOrder(TreeNode *root);          // prints the inorder traversal of the tree
            void printPreOrder(TreeNode *root);         // prints the pre-order traversal of the tree
            void printPostOrder(TreeNode *root);        // prints the post-order traversal of the tree
            TreeNode* insert(TreeNode *parent, TreeNode *root, int data); // inserts data into the tree
            TreeNode* getSibling(TreeNode *ptr);       // returns the sibling of ptr

            // delete function and it's different cases
            void delete_(TreeNode *root, int data, TreeNode **rootReference);
            void deleteOneChild(TreeNode *nodeToBeDeleted, TreeNode **rootReference);
            void deleteCase1(TreeNode *doubleBlackNode, TreeNode **rootReference);
            void deleteCase2(TreeNode *doubleBlackNode, TreeNode **rootReference);
            void deleteCase3(TreeNode *doubleBlackNode, TreeNode **rootReference);
            void deleteCase4(TreeNode *doubleBlackNode, TreeNode **rootReference);
            void deleteCase5(TreeNode *doubleBlackNode, TreeNode **rootReference);
            void deleteCase6(TreeNode *doubleBlackNode, TreeNode **rootReference);

            TreeNode* getSmallest(TreeNode *root);              // returns the inorder successor of the root node
            void replaceNode(TreeNode *root, TreeNode *child, TreeNode **rootReference); // replaces root node with child node
            void removeNode(TreeNode *root, TreeNode *child);   // deletes the root node
            bool isLeftChild(TreeNode *ptr);                    // checks whether the node is a left child or not
            
            bool noRedParentChild(TreeNode *root, char parentColor); // checks whether no red child has a red parent
            int checkBlackNodesCount(TreeNode *root, int count); // counts the number of black nodes in a path from root to leaf

        public:
            Tree() {
                root = new TreeNode();
                root->is_null = true; 
                null_node = new TreeNode();
                null_node->is_null = true;
            }
            void insertNode(int data);  // inserts a node containing data into the tree
            void deleteNode(int data);  // deletes the node containing data from the tree
            void printTree(int code);   // prints the tree
            void validateRBTree();         // checks whether the tree is Red Black Tree or not

    };

    void Tree::createLeaves(TreeNode *ptr){
        ptr->left = new TreeNode();      ptr->right = new TreeNode();
        ptr->left->is_null = true;        ptr->right->is_null = true;
        ptr->left->parent = ptr;          ptr->right->parent = ptr;
    }
    
    void Tree::deleteLeaves(TreeNode *ptr){
        if (ptr->left->is_null) delete ptr->left;
        if (ptr->right->is_null) delete ptr->right;
    }


    void Tree::rightRotate(TreeNode *root, bool changeColor){
        TreeNode *parent = root->parent;
        root->parent = parent->parent;

        if (parent->parent != NULL){
            if(parent->parent->right == parent){
                parent->parent->right = root;
            }
            else parent->parent->left = root;
        }

        TreeNode *right = root->right;
        root->right = parent;
        parent->parent = root;
        parent->left = right;
        
        right->parent = parent;
        
        if(changeColor){
            parent->color = 'R';
            root->color = 'B';
        }
    }

    void Tree::leftRotate(TreeNode *root, bool changeColor){
        TreeNode *parent = root->parent;
        root->parent = parent->parent;

        if (parent->parent != NULL){
            if(parent->parent->right == parent){
                parent->parent->right = root;
            }
            else parent->parent->left = root;
        }

        TreeNode *left = root->left;
        root->left = parent;
        parent->parent = root;
        parent->right = left;
        
        left->parent = parent;
        
        if(changeColor){
            parent->color = 'R';
            root->color = 'B';
        }
    }

    TreeNode* Tree::getSibling(TreeNode *ptr){
        TreeNode *parent = ptr->parent;

        if(parent == NULL){
            return null_node;
        }
        if (ptr == parent->right) return parent->left;
        else return parent->right;
    }

    TreeNode* Tree::insert(TreeNode *parent, TreeNode *root, int data){
        if(root->is_null == true){
            root->is_null = false;
            root->data = data;
            root->parent = parent;

            createLeaves(root);
            if (parent != NULL){
                root->color = 'R';
            }
            
            return root;
        }

        bool isLeft;
        if(root->data > data){
            TreeNode *left = insert(root, root->left, data);

            if(left == root->parent){
                return left;
            }
            root->left = left;
            isLeft = true;
        }else if(root->data == data){
            throw std::invalid_argument( "Value already present in tree." );
        }else{
            TreeNode *right = insert(root, root->right, data);

            if(right == root->parent){
                return right;
            }
            root->right = right;
            isLeft = false;
        }

        if(isLeft){
            if(root->color =='R' && root->left->color == 'R'){
                TreeNode *sibling = getSibling(root);
                if (sibling->is_null == true || sibling->color == 'B'){
                    if (root == root->parent->left){
                        rightRotate(root, true);
                    }else{
                        rightRotate(root->left, false);
                        root = root->parent;
                        leftRotate(root, true);
                    }
                }else{
                    root->color = 'B';
                    sibling->color = 'B';

                    if(root->parent->parent != NULL){
                        root->parent->color = 'R';
                    }
                }
            }
        }else{
            if(root->color =='R' && root->right->color == 'R'){
                TreeNode *sibling = getSibling(root);
                if (sibling->is_null == true || sibling->color == 'B'){
                    if (root == root->parent->right){
                        leftRotate(root, true);
                    }else{
                        leftRotate(root->right, false);
                        root = root->parent;
                        rightRotate(root, true);
                    }
                }else{
                    root->color = 'B';
                    sibling->color = 'B';

                    if(root->parent->parent != NULL){
                        root->parent->color = 'R';
                    }
                }
            }
        }
        return root;
    }

    void Tree::printInOrder(TreeNode* root){
        if(root->is_null == false){
            printInOrder(root->left);
            std::cout << root->data << " " << root->color << "\n";
            printInOrder(root->right);
        }
    }

    void Tree::printPreOrder(TreeNode* root){
        if(root->is_null == false){
            std::cout << root->data << " " << root->color << "\n";
            printPreOrder(root->left);
            printPreOrder(root->right);
        }
    }

    void Tree::printPostOrder(TreeNode* root){
        if(root->is_null == false){
            printPostOrder(root->left);
            printPostOrder(root->right);
            std::cout << root->data << " " << root->color << "\n";
        }
    }

    void Tree::printTree(int code){
        if(code == 1){
            std::cout << "PreOrder Traversal\n";
            printPreOrder(Tree::root);
            std::cout << std::endl; 
        }

        if(code == 2){
            std::cout << "InOrder Traversal\n";
            printInOrder(Tree::root);
            std::cout << std::endl; 
        }

        if(code == 3){
            std::cout << "PostOrder Traversal\n";
            printPostOrder(Tree::root);
            std::cout << std::endl; 
        }
    }

    void Tree::insertNode(int data){
        Tree::root = insert(NULL, Tree::root, data);
    }

    void Tree::deleteNode(int data){
        TreeNode *ptr = NULL;
        TreeNode **rootReference = &ptr; 
        delete_(Tree::root, data, rootReference);
        if(*rootReference == NULL){
            Tree::root = root;
        }else{
            Tree::root = *rootReference;
        }
    }

    void Tree::delete_(TreeNode *root, int data, TreeNode **rootReference){
        if (root == NULL || root->is_null){
            return ;
        }

        if (root->data == data){
            if (root->left->is_null || root->right->is_null){
                deleteOneChild(root, rootReference);
            }else{
                TreeNode *inOrderSucessor = getSmallest(root->right);
                root->data = inOrderSucessor->data;
                delete_(root->right, root->data, rootReference);
            }
        }

        if(root->data < data){
            delete_(root->right, data, rootReference);
        }else{
            delete_(root->left, data, rootReference);
        }
    }

    TreeNode* Tree::getSmallest(TreeNode *ptr){
        TreeNode *prev = NULL;
        while(ptr != NULL && !ptr->is_null){
            prev = ptr;
            ptr = ptr->left;
        }
        return prev != NULL ? prev : ptr;
    }

    bool Tree::isLeftChild(TreeNode *ptr){
        if(ptr == ptr->parent->left) return true;
        else return false;
    }

    void Tree::removeNode(TreeNode *root, TreeNode *child){
        if (child == root->left) delete root->right;
        else delete root->left;

        delete root;
    }


    void Tree::replaceNode(TreeNode *root, TreeNode *child, TreeNode **rootReference){
        child->parent = root->parent;
        if(root->parent == NULL){
            rootReference = &child;
        }else{
            if(isLeftChild(root)){
                root->parent->left = child;
            }else{
                root->parent->right = child;
            }
        }
    }

    /**
     * Assumption that node to be deleted has either 0 or 1 non leaf child
     */
    void Tree::deleteOneChild(TreeNode *nodeToBeDeleted, TreeNode **rootReference){
        TreeNode *child = nodeToBeDeleted->right->is_null ? nodeToBeDeleted->left : nodeToBeDeleted->right;
        replaceNode(nodeToBeDeleted, child, rootReference);
        char nodeColor = nodeToBeDeleted->color;
        removeNode(nodeToBeDeleted, child);

        if(nodeColor == 'B'){
            if(child->color == 'R'){
                child->color = 'B';
            }else{
                deleteCase1(child, rootReference);
            }
        }
    }


    /**
    * If double black node becomes root then we are done. Turning it into
    * single black node just reduces one black in every path.
    */
    void Tree::deleteCase1(TreeNode *doubleBlackNode, TreeNode **rootReference){
        if(doubleBlackNode->parent == NULL){
            rootReference = &doubleBlackNode;
            return;
        }
        deleteCase2(doubleBlackNode, rootReference);
    }


    /**
    * If sibling is red and parent and sibling's children are black then rotate it
    * so that sibling becomes black. Double black node is still double black so we need
    * further processing.
    */
    void Tree::deleteCase2(TreeNode *doubleBlackNode, TreeNode **rootReference){
        TreeNode *sibling = getSibling(doubleBlackNode);
        if (sibling->color == 'R'){
            if(isLeftChild(sibling)){
                rightRotate(sibling, true);
            }else{
                leftRotate(sibling, true);
            }

            if(sibling->parent == NULL) rootReference = &sibling;
        }
        deleteCase3(doubleBlackNode, rootReference);
    }


    /**
    * If sibling, sibling's children and parent are all black then turn sibling into red.
    * This reduces black node for both the paths from parent. Now parent is new double black
    * node which needs further processing by going back to case1.
    */
    void Tree::deleteCase3(TreeNode *doubleBlackNode, TreeNode **rootReference){
        TreeNode *sibling = getSibling(doubleBlackNode);
        if(doubleBlackNode->parent->color == 'B' && sibling->color == 'B' &&
           sibling->left->color == 'B' && sibling->right->color == 'B'){
               sibling->color = 'R';
               deleteCase1(doubleBlackNode->parent, rootReference);
        }else{
            deleteCase4(doubleBlackNode, rootReference);
        }
    }

    /**
    * If sibling color is black, parent color is red and sibling's children color is black then swap color b/w sibling
    * and parent. This increases one black node on double black node path but does not affect black node count on
    * sibling path. We are done if we hit this situation.
    */
    void Tree::deleteCase4(TreeNode *doubleBlackNode, TreeNode **rootReference){
        TreeNode *sibling = getSibling(doubleBlackNode);
        if(doubleBlackNode->parent->color == 'R' && sibling->color == 'B' &&
           sibling->left->color == 'B' && sibling->right->color == 'B'){
               sibling->color = 'R';
               doubleBlackNode->parent->color = 'B';
               return;
        }else{
            deleteCase5(doubleBlackNode, rootReference);
        }
    }


    /**
    * If sibling is black, double black node is left child of its parent, siblings right child is black
    * and sibling's left child is red then do a right rotation at siblings left child and swap colors.
    * This converts it to delete case6. It will also have a mirror case.
    */
    void Tree::deleteCase5(TreeNode *doubleBlackNode, TreeNode **rootReference){
        TreeNode *sibling = getSibling(doubleBlackNode);
        if(sibling->color == 'B'){
            if(isLeftChild(doubleBlackNode) && sibling->right->color == 'B' && sibling->left->color == 'R'){
                rightRotate(sibling->left, true);
            }else if(!isLeftChild(doubleBlackNode) && sibling->left->color == 'B' && sibling->right->color == 'R'){
                rightRotate(sibling->right, true);
            }else{;}
        }
        deleteCase6(doubleBlackNode, rootReference);
    }


    /**
    * If sibling is black, double black node is left child of its parent, sibling left child is black and sibling's right child is
    * red, sibling takes its parent color, parent color becomes black, sibling's right child becomes black and then do
    * left rotation at sibling without any further change in color. This removes double black and we are done. This
    * also has a mirror condition.
    */
    void Tree::deleteCase6(TreeNode *doubleBlackNode, TreeNode **rootReference){
        TreeNode *sibling = getSibling(doubleBlackNode);
        sibling->color = sibling->parent->color;
        sibling->parent->color = 'B';
        if(isLeftChild(doubleBlackNode)){
            sibling->right->color = 'B';
            leftRotate(sibling, false);
        }else {
            sibling->left->color = 'B';
            rightRotate(sibling, false);
        }

        if(sibling->parent == NULL){
            rootReference = &sibling;
        }
    }


    int Tree::checkBlackNodesCount(TreeNode *root, int count){
        if(!root->is_null){
            int curr_count = 0;
            if (root->color == 'B') curr_count++;
            int l_count = checkBlackNodesCount(root->left, curr_count);
            int r_count = checkBlackNodesCount(root->right, curr_count);

            if(l_count == r_count && l_count != -1) return count + l_count;
            else return -1;
        }
        return count;
    }


    bool Tree::noRedParentChild(TreeNode *root, char parentColor){
        if(root->is_null) return true;

        if(root->color == 'R' && parentColor == 'R') return false;

        return noRedParentChild(root->left, root->color) && noRedParentChild(root->right, root->color);
    }

    void Tree::validateRBTree(){
        bool isRBTree = true;
        if(!root->is_null){
            if(root->color == 'R'){
                isRBTree = false;
                std::cout << "Root is of Red Color.\n";
            }
            if(isRBTree){
                isRBTree = noRedParentChild(root, 'B');
                if(!isRBTree) std::cout << "There is a Red Parent and a Red Child relationship in the tree.\n";
            }

            if(isRBTree){
                int val = checkBlackNodesCount(root, 0);
                if (val == -1){
                    std::cout << "The count of black nodes on every path from root to leaf is not same.\n";
                    isRBTree = false;
                }
            }
        }

        if(isRBTree) std::cout << "The tree is a Red Black Tree.\n";
        else std::cout << "The tree is not a Red Black Tree.\n";
    }

};

int main(){
    RBTree::Tree rbt;
    int arr[] = {10, 20, -10, 15, 17, 40, 50, 60};
    for(int i = 0; i < 8; i++){
        rbt.insertNode(arr[i]);
    }
    rbt.printTree(2);
    rbt.deleteNode(17);
    rbt.printTree(2);

    rbt.validateRBTree();
    return 0;
}