//
// Created by HONOR on 14.02.2023.
//

#ifndef MYALGORITHMS_MYSTRUCTURES_H
#define MYALGORITHMS_MYSTRUCTURES_H
#include <algorithm>
#include <list>

namespace my{
    template<typename T>
    class TreeAVL{
        struct Node{
            Node* left = nullptr;
            Node* right = nullptr;
            T value;
            int h = 1;
            Node(T&& value) : value(std::move(value)){
            }
            int calcDiff() {
                int leftH = left == nullptr ? 0 : left->h;
                int rightH = right == nullptr ? 0 : right->h;
                return leftH - rightH;
            }
            void recalculateHeight() {
                int leftH = left == nullptr ? 0 : left->h;
                int rightH = right == nullptr ? 0 : right->h;
                h = 1 + std::max(leftH, rightH);
            }
        };



        Node* root = nullptr;

        void rotateLeft(Node*& current){
            Node* newCurrent = current->right;
            current->right = newCurrent->left;
            newCurrent->left = current;

            current->recalculateHeight();
            newCurrent->recalculateHeight();
            current = newCurrent;
        }
        void rotateRight(Node*& current){
            Node* newCurrent = current->left;
            current->left = newCurrent->right;
            newCurrent->right = current;

            current->recalculateHeight();
            newCurrent->recalculateHeight();
            current = newCurrent;
        }
        bool balance(Node*& current) {
            if(current== nullptr){
                return true;
            }
            int currentDiff = current->calcDiff();
            int oldHeight = current->h;
            if(currentDiff == 0) {
                current->recalculateHeight();
                return oldHeight != current->h;  // not needBalance
            }
            if(currentDiff == -1 || currentDiff == 1) {
                current->recalculateHeight();
                return oldHeight != current->h; //needBalance
            }
            if(currentDiff == -2) {
                int rightDiff = current->right->calcDiff();
                if (rightDiff == 0 || rightDiff == -1) {
                    rotateLeft(current);
                } else {
                    rotateRight(current->right);
                    rotateLeft(current);
                }
//                std::cout << "[" << current->calcDiff() << "]\n";
//                return current->calcDiff() != 0;//needBalance
                return oldHeight != current->h;
            }
            if(currentDiff == 2){
                int leftDiff = current->left->calcDiff();
                if(leftDiff == 0 || leftDiff == 1){
                    rotateRight(current);
                } else{
                    rotateLeft(current->left);
                    rotateRight(current);
                }
//                std::cout << "[" << current->calcDiff() << "]\n";
//                return current->calcDiff() != 0; //needBalance
                return oldHeight != current->h;
            }
            //error tree balance
        }
        bool insert(Node*& current, T&& value){
            if(current == nullptr){
                current = new Node(std::move(value));
                return true; //needBalance
            }
            if(value == current->value){
                return false; //needBalance
            }

            bool needBalance;
            if(value < current->value){
                needBalance = insert(current->left, std::move(value));
            }else{
                needBalance = insert(current->right, std::move(value));
            }
            if(needBalance) {
                return balance(current);
            }
            return false; //needBalance
        }
        bool contains(Node* current, const T& value){
            if(current == nullptr){
                return false;
            }
            if(value == current->value){
                return true;
            }
            if(value < current->value){
                return contains(current->left, value);
            }else{
                return contains(current->right, value);
            }
        }
        bool replaceAndDelete(Node*& removable, Node*& replacement){
            if(replacement->left->left != nullptr){ //parent of replacement
                bool needBalance = replaceAndDelete(removable, replacement->left);
                if(needBalance) {
                    return balance(replacement);
                }
                return false;
            }else{
                Node* trueReplacement = replacement->left;
                replacement->left = trueReplacement->right;

                trueReplacement->left = removable->left;
                trueReplacement->right = removable->right;

                delete removable;
                removable = trueReplacement;
                return balance(replacement);
            }
        }
        bool eraseNode(Node*& current){
            Node* smallestRight = current->right;
            if(smallestRight == nullptr){
                /*         H
                 *        /
                 *      [L]
                 */
                Node* left = current->left;
                delete current;
                current = left;
                return true;
            }
            else if(smallestRight->left == nullptr){
                /*         H
                 *       /  \
                 *     [L]   R
                 *             \
                 *             [r]
                 */
                smallestRight->left = current->left;
                delete current;
                current = smallestRight;
                return true;
            }
            else {
                /*           H
                 *         /  \
                 *        L    R
                 *           /
                 *          ...
                 *          /
                 *          l
                 *           \
                 *           [r]
                 */
                bool needBalance = replaceAndDelete(current, smallestRight);
                current->right = smallestRight;
                if(needBalance){
                    return balance(current);
                }
                return false; //not needBalance
            }
        }
        bool erase(Node*& current,  const T& value){
            if(current == nullptr){
                return false;
            }
            bool needBalance;
            if(value == current->value){
                eraseNode(current);
                balance(current);
                return true;
            }else if(value < current->value){
                needBalance = erase(current->left, value);
            }else{
                needBalance = erase(current->right, value);
            }

            if(needBalance){
                return balance(current);
            }
        }
        void print(Node* current, int dep, char ch){
            if(current == nullptr){
                return;
            }
            print(current->left, dep + 1, '/');
            std::cout.width(dep * 5);
            std::cout << ch << "(" << current->value << ',' <<current->calcDiff() << ") \n";
            print(current->right, dep + 1, '\\');
        }
    public:
        class iterator{
            std::list<Node*> parents;
            Node* current;
        public:
            iterator(Node* node) : current(node){
                while(current->left!= nullptr){
                    parents.push_back(current);
                    current = current->left;
                }
            }
            iterator& operator++(){
                if(current->right != nullptr){
                    parents.push_back(current);
                    current = current->right;
                    while(current->left != nullptr){
                        parents.push_back(current);
                        current = current->left;
                    }
                }else{
                    while(parents.back()->value < current->value){
                        parents.pop_back();
                    }
                    current = parents.back();
                    parents.pop_back();
                }
                return *this;
            }
            iterator& operator--(){
                if(current->left != nullptr){
                    parents.push_back(current);
                    current = current->left;
                    while (current->right != nullptr){
                        parents.push_back(current);
                        current = current->right;
                    }
                }else{
                    while(parents.back()->value > current->value){
                        parents.pop_back();
                    }
                    current = parents.back();
                    parents.pop_back();
                }
                return *this;
            }
            const T operator*(){
                return current->value;
            }
        };
        iterator begin(){
            return iterator(root);
        }
        void insert(T value){
//            std::cout << "insert "<< value <<std::endl;
            bool needBalance = insert(root, std::move(value));
            if(needBalance){
                balance(root);
            }
        }
        bool contains(const T& value){
            if(root == nullptr) {
                return false;
            }
            return contains(root, value);
        }
       void erase(T value){
           bool needBalance = erase(root, value);
           if(needBalance){
               balance(root);
           }
        }
        void print(){
            print(root, 0, '=');
        }
    };
}
#endif //MYALGORITHMS_MYSTRUCTURES_H
