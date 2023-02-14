//
// Created by HONOR on 14.02.2023.
//

#ifndef MYALGORITHMS_MYSTRUCTURES_H
#define MYALGORITHMS_MYSTRUCTURES_H
#include <algorithm>
namespace my{
    template<typename T>
    class TreeAVL{
        struct Node{
            Node* left = nullptr;
            Node* right = nullptr;
            Node* parent;
            T value;
            int diff = 0;
            Node(Node* parent, T&& value) :parent(parent), value(std::move(value)){}

        };
        Node* root = nullptr;
        void insert(Node* head, T&& value){
            if(value == head->value){
                return;
            }
            if(value < head->value){
                if(head->left == nullptr){
                    head->left = new Node(head, std::move(value));
                }else {
                    insert(head->left, std::move(value));
                }

            }else{
                if(head->right == nullptr){
                    head->right = new Node(head, std::move(value));
                }else {
                    insert(head->right, std::move(value));
                }
            }
        }
        bool contains(Node* head, const T& value){
            if(head == nullptr){
                return false;
            }
            if(value == head->value){
                return true;
            }
            if(value < head->value){
                return contains(head->left, value);
            }else{
                return contains(head->right, value);
            }
        }
        void erase(Node*& head,  const T& value){
            if(head == nullptr){
                return;
            }
            if(value == head->value){
                Node* smallestRight = head->right;
                if(smallestRight == nullptr){
                    /*         H
                     *        /
                     *      [L]
                     */
                    if(head->left != nullptr) {
                        head->left->parent = head->parent;
                    }
                    Node* left = head->left;

                    delete head;
                    head = left;
                    return;
                }
                if(smallestRight->left == nullptr){
                    /*
                     *         H
                     *       /  \
                     *     [L]   R
                     *             \
                     *             [r]
                     */
                    smallestRight->parent = head->parent;
                    smallestRight->left = head->left;
                    if(smallestRight->left != nullptr){
                        smallestRight->left->parent = smallestRight;
                    }

                    delete head;
                    head = smallestRight;
                    return;
                }
                /*
                 *           H
                 *         /  \
                 *        L    R
                 *           /
                 *          ...
                 *          /
                 *          l
                 *           \
                 *           [r]
                 */
                while (smallestRight->left != nullptr){
                    smallestRight = smallestRight->left;
                }
                smallestRight->parent->left = smallestRight->right;
                if(smallestRight->right!= nullptr){
                    smallestRight->right->parent = smallestRight->parent;
                }
                smallestRight->parent = head->parent;
                smallestRight->left = head->left;
                if(smallestRight->left != nullptr) {
                    smallestRight->left->parent = smallestRight;
                }
                smallestRight->right = head->right;
                smallestRight->right->parent = smallestRight;

                delete head;
                head = smallestRight;
                return;
            }
            if(value < head->value){
                erase(head->left, value);
            }else{
                erase(head->right, value);
            }
        }
        void print(Node* head){
            if(head == nullptr){
                return;
            }
            print(head->left);
            std::cout << head->value << ' ';
            print(head->right);
        }
    public:
        void insert(T value){
            if(root == nullptr){
                root = new Node(nullptr, std::move(value));
                return;
            }
//            std::cout << "insert "<< value <<std::endl;
            insert(root, std::move(value));
        }
        bool contains(const T& value){
            if(root == nullptr) {
                return false;
            }
            return contains(root, value);
        }
       void erase(T value){
           erase(root, value);
        }
        void print(){
            std::cout << "{ ";
            print(root);
            std::cout << "}";
        }
    };
}
#endif //MYALGORITHMS_MYSTRUCTURES_H
