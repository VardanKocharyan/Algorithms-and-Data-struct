#pragma once 

template <typename T>
class binary_search_tree {
    
    public:
        struct BSTNode {
            T value;
            BSTNode* parent;
            BSTNode* left;
            BSTNode* right;

            BSTNode() = default;
            BSTNode(T& x) : value(x), parent(nullptr), left(nullptr), right(nullptr) {}
            BSTNode(T&& x) : value(std::move(x)), parent(nullptr), left(nullptr), right(nullptr) {}
            BSTNode(T& x, BSTNode* p, BSTNode* l, BSTNode* r) : value(x), parent(p) left(l), right(r) {}
            BSTNode(T&& x, BSTNode* p, BSTNode* l, BSTNode* r) : value(std::move(x)), parent(p) left(l), right(r) {}
            ~BSTNode() = default;
        };

        using value_type      = T;
        using size_type       = size_t;
        using reference       = value_type&;
        using const_reference = const value_type&;
        using pointer         = value_type*;
        using const_pointer   = const value_type*;

        //member data
        BSTNode* root = nullptr;

    private:
        BSTNode* insert(BSTNode* node, T key) 
        {
            if (!node) return new BSTNode(key);

            if (key < node->value) {
                node->left = insert(node->left, key);
            } else if (key > node->value) {
                    node->right = insert(node->right, key);
            }
            return node;
        }

        BSTNode* rm(BSTNode* node, T& k) 
        {
            if (!node) return nullptr;

            if (k < node->value) node->left = rm(node->left, k);
            else if (k > node->value) node->right = rm(node->right, k);
            else {
                if (!node->left) {
                    BSTNode* tmp = node->right;
                    delete node;
                    return tmp;
                }
                if (!node->right) {
                    BSTNode* tmp = node->left;
                    delete node;
                    return tmp;
                }
                BSTNode* s = setMin(node->right);
                node->value = s->value;
                node->right = rm(node->right, s->value);
            }
            return node;
        }

    public:
        binary_search_tree() = default;
        binary_search_tree(T& x) : root(new BSTNode(x));
        binary_search_tree(T&& x) : root(new BSTNode(std::move(x)));
        

        void insert(T& k) { root = insert(root, k); }
        void insert(T&& k) { root = insert(root, std::move(k)); }

        int getHeight(BSTNode* node);
        BSTNode* setMin(BSTNode* n);
        BSTNode* setMax(BSTNode* n);
        
        BSTNode* getsuccessor(BSTNode* node);
        BSTNode* getpredecessor(BSTNode* node);

        void remove(T key) { root = rm(root, key); }


};

template <typename T>
int binary_search_tree<T>::getHeight(BSTNode* node)
{
    if (!node) return -1;

    int l = getHeight(node->left);
    int r = getHeight(node->right);

    return 1 + std::max(l, r);
}

template <typename T>
BSTNode* binary_search_tree<T>::setMin(BSTNode* n)
{
    while (n && n->left) {
        n = n->left;
    }
    return n;
}

template <typename T>
BSTNode* binary_search_tree<T>::setMax(BSTNode* n)
{
    while (n && n->right) {
        n = n->right;
    }
    return n;
}

template <typename T>
BSTNode* binary_search_tree<T>::getpredecessor(BSTNode* node)
{
    if (!node) return nullptr;

    if (node->left) return setMax(node->left);

    BSTNode* curr = root;
    BSTNode* p = nullptr;
    while (curr) {
        if (curr->value > node->value) {
            curr = curr->next;
        } else if (curr->value < node->value) {
            p = curr;
            curr = curr->next;
        } else break;
    }
    return p;
}

template <typename T>
BSTNode* binary_search_tree<T>::getsuccessor(BSTNode* node) 
{
    if (!node) return nullptr;

    if (node->right) return setMin(node->right);

    BSTNode* curr = root;
    BSTNode* p = nullptr;
    while (curr) {
        if (curr->value > node->value) {
            curr = curr->right;
        } else if (curr->value < node->value) {
            p = curr;
            curr = curr->left;
        } else break;
    }
    return p;
}












