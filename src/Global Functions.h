#pragma once
#include <iostream>
#include <windows.h> // for console handling and input events
#include <fstream>
#include <filesystem>

using namespace std;

namespace fs = std::filesystem;


// Constants for maximum line length and screen size
const int MAX_LINE_LENGTH = 89;
const int MAX_SCREEN_WIDTH = 90;
const int MAX_SCREEN_HEIGHT = 20;
const int MAX_COORDINATES = 100;
const int ALPHABET_SIZE = 26;
const int MAX_SUGGESTIONS = 100;  // Maximum number of suggestions
// Array to hold suggestions and a counter for the number of suggestions found
char suggestions[MAX_SUGGESTIONS][100];  // Assuming a max length of 99 for each suggestion
int suggestionCount = 0;


// string functions :
int Length(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}
int compareStrings(const char* str1, const char* str2) {
    // Compare the strings character by character
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];  // Return difference of first mismatched character
        }
        i++;
    }

    // If one string ends earlier, return the difference
    return (unsigned char)str1[i] - (unsigned char)str2[i];
}
void copyN(char* dest, const char* src, int count) {
    int i = 0;
    // Copy characters from src to dest up to 'count' or until null terminator
    for (i = 0; i < count && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    // If there are remaining spaces, null-terminate the destination
    for (; i < count; i++) {
        dest[i] = '\0';
    }
}
void copy(char* dest, const char* src) {
    int i = 0;
    // Copy each character until the null terminator is found
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';  // Null-terminate the destination string
}


void searchgotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//char suggestions[MAX_SUGGESTIONS][100];  // Assuming a max length of 99 for each suggestion
//int suggestionCount = 0;

void addSuggestion(const char* suggestion) {
    if (suggestionCount < MAX_SUGGESTIONS) {
        copy(suggestions[suggestionCount++], suggestion);  // Copy the suggestion into the array
    }
}
//
//struct SuggestionNode {
//    char suggestion[100]; // Store the suggestion string
//    SuggestionNode* next;
//
//    SuggestionNode(const char* str) {
//        copy(suggestion, str);
//        next = nullptr;
//    }
//};
//
//class SuggestionList {
//public:
//    SuggestionNode* head;
//
//    SuggestionList() : head(nullptr) {}
//
//    void insert(const char* suggestion) {
//        SuggestionNode* newNode = new SuggestionNode(suggestion);
//        newNode->next = head;
//        head = newNode;  // Insert at the head
//    }
//
//    void display() {
//        SuggestionNode* current = head;
//        int index = 0;
//        while (current != nullptr) {
//            cout << index++ << ": " << current->suggestion << endl;  // Display each suggestion
//            current = current->next;
//        }
//    }
//
//    ~SuggestionList() {
//        while (head != nullptr) {
//            SuggestionNode* temp = head;
//            head = head->next;
//            delete temp;
//        }
//    }
//};


struct Coordinate {
    int lineNum;
    int colNum;
};

struct TreeNode {
    char data;
    bool isEndOfWord;
    int frequency[100];  // Array to store frequency count (for each instance)
    Coordinate coordinates[100]; // Array to store coordinates of each instance
    int instanceCount; // Track number of instances stored in frequency and coordinates arrays
    TreeNode* children[ALPHABET_SIZE];

    TreeNode(char c) : data(c), isEndOfWord(false), instanceCount(0) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
    }
};



// Node to represent each character in the editor
struct Node {
    char data;
    Node* left;
    Node* right;
    Node* up;
    Node* down;
    Node(char ch) : data(ch), left(nullptr), right(nullptr), up(nullptr), down(nullptr) {}
};

// Structure to store word-related data in the stack
struct WordNode {
    Node* head;  // Head of the linked list for the word
    int length;  // Length of the word
    int row;     // Row of the word
    int col;     // Column of the word

    WordNode(Node* h, int len, int r, int c) : head(h), length(len), row(r), col(c) {}
};
