#pragma once
#include "Global Functions.h"
// Stack node structure to store undo/redo operations
struct StackNode {
    WordNode* wordNode;
    int operation;  // Operation, either 1: "insert" or 2: "delete"
    StackNode* next;

    StackNode(WordNode* w, int op) : wordNode(w), next(nullptr), operation(-1) {

    }
};

// Custom stack implementation to store words for undo/redo
class Stack {
private:
    StackNode* top;  // Pointer to the top of the stack
    int size;  // Current size of the stack

public:
    Stack() : top(nullptr), size(0) {}

    // Push a word onto the stack
    void push(WordNode* word, int operation) {
        if (size >= 4) {
            // Remove the oldest word if the stack is full
            StackNode* temp = top;
            //top = top->next;
            // move to first item pushed to stack
            for (int i = 0; i < size; i++) {
                temp = temp->next;
            }
            delete temp;
            size--;
        }

        StackNode* newNode = new StackNode(word, operation);
        newNode->next = top;
        top = newNode;
        size++;
    }

    // Pop a word from the stack
    StackNode* pop() {
        if (!isEmpty()) {
            StackNode* temp = top;
            top = top->next;
            size--;
            return temp;
        }
        return nullptr;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    ~Stack() {
        while (!isEmpty()) {
            StackNode* temp = pop();
            delete temp;
        }
    }
};