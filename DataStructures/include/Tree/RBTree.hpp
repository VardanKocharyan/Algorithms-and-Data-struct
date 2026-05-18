#pragma once 

template <typename T>
class RBTree {
public:
    enum class Color { Red, Black };

    struct RBNode {
        T val;
        Color color;
        RBNode* left;
        RBNode* right;
        RBNode* parent;

        RBNode() = default;
        RBNode(const T& v, Color c = Color::Red, RBNode* l = nullptr, RBNode* r = nullptr, RBNode* p = nullptr) : val(v), color(c), left(l), right(r), parent(p) {}
        RBNode(T&& v, Color c = Color::Red, RBNode* l = nullptr, RBNode* r = nullptr, RBNode* p = nullptr) : val(std::move(v)), color(c), left(l), right(r), parent(p) {}
        ~RBNode() = default;
    };

//data members
private:
    RBNode* root;
    RBNode* NIL;

    void initialzeNIL() {
        NIL = new RBNode(T());
        NIL->color = Color::Black;
        NIL->parent = NIL;
        NIL->left = NIL;
        NIL->right = NIL;
    }

    void rotate_left(RBNode* x) {
        RBNode* y = x->right;
        x->right = y->left;
        if (y->left != NIL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (y->parent == NIL) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rotate_right(RBNode* x) {
        RBNode* y = x->left;
        x->left = y->right;
        if (y->right != NIL) y->right->parent = x;

        y->parent = x->parent;
        if (x->parent == NIL) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
    
        y->right = x;
        x->parent = y;
    }

    void insertFixUp(RBNode* z) {
        while (z->parent->color == Color::Red) {
            if (z->parent == z->parent->parent->right) {
                RBNode* y = z->parent->parent->left; //uncle
                if (y->color == Color::Red) { // Red uncle
                    y->color = Color::Black;
                    z->parent->color = Color::Black;
                    z = z->parent->parent;
                    z->color = Color::Red;
                } else { // Black uncle
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotate_right(z);
                    }
                    z->parent->color = Color::Black;
                    z->parent->parent->color = Color::Red;
                    rotate_left(z->parent->parent);
                }
            } else {
                RBNode* y = z->parent->parent->right;//uncle
                if (y->color == Color::Red) { //Red uncle
                    y->color = Color::Black;
                    z->parent->color = Color::Black;
                    z = z->parent->parent;
                    z->color = Color::Red;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotate_left(z);
                    }
                    z->parent->color = Color::Black;
                    z->parent->parent->color = Color::Red;
                    rotate_right(z->parent->parent);
                }
            }
        }
        root->color = Color::Black;
    }

    void transplant(RBNode* u, RBNode* v) {
        if (u->parent == NIL) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

public:
//using types
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;


    RBTree() {
        initialzeNIL();
        root = NIL;
    }

    void insert(T key) {
        RBNode* z = new RBNode(std::move(key));
        z->left = NIL;
        z->right = NIL;
        z->parent = NIL;
        z->color = Color::Red;

        RBNode* y = NIL;
        RBNode* x = root;
        while (x != NIL) {
            y = x;
            if (x->val < key) x = x->right;
            else if (x->val > key) x = x->left;
            else return;
        }

        z->parent = y;
        if (y == NIL) root = z;
        else if (key < y->val) y->left = z;
        else y->right = z;
        insertFixUp(z);
    }


};
