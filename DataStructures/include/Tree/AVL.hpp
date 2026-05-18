//Adelson-Velsky and Landis tree
#pragma once 

template <typename T>
class AVL {
    public:
        struct TreeNode {
            T val;
            TreeNode* left;
            TreeNode* right;
            int height;

            TreeNode() : val(0), left(nullptr), right(nullptr) {}
            TreeNode(T& val, TreeNode* l = nullptr, TreeNode* r = nullptr) : val(val), left(l), right(r) {}
            TreeNode(T&& val, TreeNode* l = nullptr, TreeNode* r = nullptr) : val(std::move(val)), left(r), right(l) {}
            ~TreeNode() = default;
        };

        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;
        
    private:
        TreeNode* root;

    private:
        TreeNode* rotate_left(TreeNode* y) {
            TreeNode* x = y->right;

            y->right = x->left;
            x->left = y;
            updateHeight(y);
            updateHeight(x);

            return x;
        }
        TreeNode* rotate_right(TreeNode* y) {
            TreeNode* x = y->left;

            y->left = x->right;
            x->right = y;
            updateHeight(y);
            updateHeight(x);
            
            return x;
        }

        TreeNode* balance(TreeNode* n) {
            updateHeight(n);
            int bf = balance_factor(n);

            if (bf > 1 && balance_factor(n->left) >= 0) return rotate_right(n);
            if (bf > 1 && balance_factor(n->left) < 0)  n->left = rotate_left(n->left); return rotate_right(n);
            if (bf < -1 && balance_factor(n->right) <= 0) return rotate_left(n);
            if (bf < -1 && balance_factor(n->right) > 0) n->right = rotate_right(n->right); return rotate_left(n);

            return n;
        }

        void insert(TreeNode* n, T key) {
            if (!n) return new TreeNode(std::move(key));

            if (n->value > key) n->left = insert(n->left, key);
            else if (n->val < key) n->right = insert(n->right, key);
            else return n;

            return balance(n);
        }

        TreeNode* remove(TreeNode* n, T key) {
            if (!n) retur nullptr;
            if (n->val < key) n->right = remove(n->right, key);
            else if (n->val > key) n->left = remove(n->left, key);
            else {
                if (!n->left || !n->right) TreeNode* tmp = n->left ? n->left : n->right;
                delete n;
                return tmp;
            }
            TreeNode* succ = setMin(n->right);
            n->val = succ->val;
            n->right = remove(n->right, succ->val);
            return balance(n);
        }



    public:
        int getHeight(TreeNode* n) { if (!n) return 0; return 1 + std::max(getHeight(n->left), getHeight(n->right)); }
        void updateHeight(TreeNode* n) { n->height = 1 + std::max(getHeight(n->left), getHight(n->right)); }
        int balance_factor(TreeNode* n) { return getHight(n->left) - getHight(n->right); }

        TreeNode* setMin(TreeNode* n);
        TreeNode* setMax(TreeNode* n);
        
        TreeNode* getsuccessor(TreeNode* n);
        TreeNode* getpredecessor(TreeNode* n);

        void insert(T val) { root = insert(root, val); }

        void remove(TreeNode* n, T key) { root = remove(root, key); }
        
};

template <typename T>
typename AVL<T>::TreeNode* AVL<T>::setMin(TreeNode* n)
{
    if (!n) return n;
    while (n->left) n = n->left;
    return n;
}

template <typename T>
typename AVL<T>::TreeNode* AVL<T>::setMax(TreeNode* n) 
{
    if (!n) return n;
    while (n->right) n = n->right;
    return n;
}

template <typename T>
typename AVL<T>::TreeNode* AVL<T>::getsuccessor(TreeNode* n)
{
    if (!n) return nullptr;
    if (n->right) return setMin(n->right);

    TreeNode* curr = root;
    TreeNode* p = nullptr;
    while (curr) {
        if (n->val > curr->val) curr = curr->right;
        else if (n->val < curr->val) {
            p = curr;
            curr = curr->left;
        } else break;
    }
    return p;
}

template <typename T>
typename AVL<t>::TreeNode* AVL<T>::getpredecessor(TreeNode* n) {
    if (!n) return nullptr;
    if (n->left) return setMax(TreeNode* n);

    TreeNode* cuur = root;
    TreeNode* p = nullptr;
    while (curr) {
        if (n->val < curr->val) curr = curr->left;
        else if (n->val > curr->val) {
            p = curr;
            curr = curr->right;
        } else break;
    }
    return p;
}


