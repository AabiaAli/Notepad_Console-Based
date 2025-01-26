#pragma once
#include "Global Functions.h"
// N-ary Tree class for managing the search functionality
class NaryTree {
private:
    TreeNode* root;

    // Helper function to convert character to index (for 'a' to 'z')
    char lowercase(char ch) {
        if (ch >= 'A' && ch <= 'Z') {
            return ch - 'A' + 'a';
        }
        return ch;
    }
    int convertToIndex(char ch) {
        return lowercase(ch) - 'a';
    }

    // Helper to mark nodes in linked list for highlighting
    void highlightText(int line, int col, int length, char* word) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coord = { col, line };
        SetConsoleCursorPosition(hConsole, coord);
        SetConsoleTextAttribute(hConsole, 5);

        for (int i = 0; i < length; i++) {
            cout << word[i];
        }
        SetConsoleTextAttribute(hConsole, 15);
    }

    // Recursive function to search a word or phrase
    //bool searchHelper(TreeNode* current, char* word, int wordIndex, int& foundLine, int& foundCol) {
    //    // Base case: if word is fully traversed
    //    if (word[wordIndex] == '\0') {
    //        if (current->isEndOfWord) {
    //            foundLine = current->lineNum;
    //            foundCol = current->colNum;
    //            return true;
    //        }
    //        return false;
    //    }
    //    if (word[wordIndex] >= 'A' && word[wordIndex] <= 'Z') {
    //        // convert to lowercase
    //        word[wordIndex] += 32;
    //    }
    //    int index = word[wordIndex] - 'a';
    //    if (index < 0 || index >= ALPHABET_SIZE || !current->children[index]) {
    //        return false;  // No match found for current character
    //    }

    //    // Recur for next character
    //    return searchHelper(current->children[index], word, wordIndex + 1, foundLine, foundCol);
    //}

public:
    NaryTree() : root(new TreeNode('\0')) {}  // Initialize with empty root

    void insertWord(char* word, int line, int col) {
        TreeNode* current = root;
        int i = 0;
        while (word[i] != '\0') {
            int index = convertToIndex(word[i]);
            if (index < 0 || index >= ALPHABET_SIZE) continue;

            if (current->children[index] == nullptr) {
                current->children[index] = new TreeNode(lowercase(word[i]));
            }
            current = current->children[index];
            i++;
        }

        // Add the new instance's line and column to the coordinates array
        current->isEndOfWord = true;
        current->frequency[current->instanceCount] = 1;  // Set frequency count for this instance
        current->coordinates[current->instanceCount] = { line, col }; // Store coordinates
        current->instanceCount = current->instanceCount + 1;
    }

    void getSuggestions(TreeNode* node, char* prefix, int depth) {
        if (node == nullptr) return;
        // clear the prefix
        prefix[0] = ' ';
        // If this node is not the root node, append its data to the prefix
        if (node->data != '\0') {
            if (node->data >= 'a' && node->data <= 'z' || node->data >= 'A' && node->data <= 'Z') {
                prefix[depth] = node->data;  // Append current character to the prefix
                depth++;  // Increment depth only when a valid character is added
            }
        }


        // If this node is the end of a word, store the suggestion
        if (node->isEndOfWord) {
            prefix[depth] = '\0';  // Null-terminate the prefix
            if (suggestionCount < MAX_SUGGESTIONS) {
                //  std::cout << "Storing suggestion: " << prefix << std::endl;  
                copy(suggestions[suggestionCount++], prefix);  // Store suggestion
            }
        }

        // Recur for all children
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i] != nullptr) {
                getSuggestions(node->children[i], prefix, depth);  // Pass the current depth
            }
        }
    }


    void sortSuggestions() {
        for (int i = 0; i < suggestionCount - 1; i++) {
            for (int j = 0; j < suggestionCount - i - 1; j++) {
                if (compareStrings(suggestions[j], suggestions[j + 1]) > 0) {
                    // Swap suggestions[j] and suggestions[j + 1]
                    char temp[100];
                    copy(temp, suggestions[j]);
                    copy(suggestions[j], suggestions[j + 1]);
                    copy(suggestions[j + 1], temp);
                }
            }
        }
    }

    void suggestWords(char* prefix) {
        TreeNode* currentNode = root;
        int length = Length(prefix);

        // Clear previous suggestions
        suggestionCount = 0;

        // Traverse the tree based on the prefix
        for (int i = 0; i < length; i++) {
            int index = convertToIndex(prefix[i]);
            if (index < 0 || index >= ALPHABET_SIZE || !currentNode->children[index]) {
                return;  // No suggestions found
            }
            currentNode = currentNode->children[index];
        }

        // Now get suggestions from this node
        char wordBuffer[100];
        getSuggestions(currentNode, wordBuffer, length);

        // Sort the suggestions
        sortSuggestions();

        // Display the suggestions
       // std::cout << "Suggestions Area: " << std::endl;
        searchgotoxy(0, 24);
        for (int i = 0; i < suggestionCount; i++) {

            cout << i + 1 << suggestions[i] << endl;  // Display each suggestion
            //  cout << suggestions[i][0] << endl;
        }

    }

    void searchHelper(TreeNode* current, char* word, int wordIndex, Coordinate* allCoordinates, int& coordCount) {
        if (word[wordIndex] == '\0') {
            if (current->isEndOfWord) {
                // Collect all coordinates for instances of this word
                for (int i = 0; i < current->instanceCount && coordCount < MAX_COORDINATES; i++) {
                    allCoordinates[coordCount++] = current->coordinates[i];
                }
            }
            return;
        }

        int index = convertToIndex(word[wordIndex]);
        if (index < 0 || index >= ALPHABET_SIZE || !current->children[index]) return;

        // Recur for next character
        searchHelper(current->children[index], word, wordIndex + 1, allCoordinates, coordCount);
    }

    void searchPhrase(char* phrase) {
        char wordBuffer[100];
        int i = 0, wordStart = 0;
        bool phraseFound = true;

        Coordinate allCoordinates[MAX_COORDINATES];
        int coordCount = 0;

        // Split and search each word in the phrase
        while (phrase[i] != '\0') {
            if (phrase[i] == ' ' || phrase[i + 1] == '\0') {
                int wordLen = (phrase[i] == ' ') ? i - wordStart : i - wordStart + 1;
                copyN(wordBuffer, &phrase[wordStart], wordLen);
                wordBuffer[wordLen] = '\0';

                // Clear previous search coordinates for each word
                coordCount = 0;
                searchHelper(root, wordBuffer, 0, allCoordinates, coordCount);

                if (coordCount == 0) {
                    phraseFound = false;
                    break;
                }
                else {
                    int printLine = 5;
                    for (int j = 0; j < coordCount; j++) {
                        highlightText(allCoordinates[j].lineNum, allCoordinates[j].colNum, wordLen, wordBuffer);
                        searchgotoxy(91, printLine);
                        if (phraseFound && coordCount > 0) {
                            cout << "Phrase found in line: " << allCoordinates[j].lineNum << "\n";
                        }
                        printLine++;
                    }
                }
                wordStart = i + 1;
            }
            i++;
        }

        searchgotoxy(91, 5);
        if (phraseFound && coordCount > 0) {
            //  cout << "Phrase found in line: " << allCoordinates[0].lineNum << "\n";
        }
        else {
            cout << "Phrase not found.\n";
        }
    }
    //
    //// Recursive helper for prefix-based matching and instance tracking
    //    void searchHelper(TreeNode* current, const char* word, int wordIndex, Coordinate* coords, int& coordCount) {
    //        if (word[wordIndex] == '\0') {
    //            // If we're at the end of the word and this node is the end of a valid word/prefix
    //            if (current->isEndOfWord) {
    //                for (int i = 0; i < current->instanceCount; i++) {
    //                    coords[coordCount++] = current->coordinates[i];
    //                }
    //            }
    //            return;
    //        }
    //
    //        int index = convertToIndex(word[wordIndex]);
    //        if (index < 0 || index >= ALPHABET_SIZE || !current->children[index]) return;
    //
    //        // Recur to the next character
    //        searchHelper(current->children[index], word, wordIndex + 1, coords, coordCount);
    //    }
    //
    //    // Searches for a multi-word phrase or prefix across instances
    //    void searchPhrase( char* phrase) {
    //        char wordBuffer[100];
    //        int i = 0, wordStart = 0;
    //        bool phraseFound = true;
    //        Coordinate coords[100];
    //        int coordCount = 0;
    //
    //        // Process each word in the phrase individually
    //        while (phrase[i] != '\0') {
    //            if (phrase[i] == ' ' || phrase[i + 1] == '\0') {
    //                int wordLen = (phrase[i] == ' ') ? i - wordStart : i - wordStart + 1;
    //                copyN_s(wordBuffer, &phrase[wordStart], wordLen);
    //                wordBuffer[wordLen] = '\0';
    //
    //                coordCount = 0;  // Reset for each word search
    //                searchHelper(root, wordBuffer, 0, coords, coordCount);
    //
    //                if (coordCount == 0) {
    //                    phraseFound = false;
    //                    break;
    //                }
    //                else {
    //                    // For phrases, check that each subsequent word matches at expected coordinates
    //                    if (phrase[i] == ' ' && phrase[i + 1] != '\0') {
    //                        for (int j = 0; j < coordCount; j++) {
    //                            int line = coords[j].lineNum, col = coords[j].colNum;
    //                            for (int k = 1; phrase[i + k] != '\0' && phrase[i + k] != ' '; k++) {
    //                                col = col + 1 + wordLen;
    //                                if (!matchNextWord(root, &phrase[i + k], line, col)) {
    //                                    phraseFound = false;
    //                                    break;
    //                                }
    //                            }
    //                            if (phraseFound) highlightText(line, col, strlen(phrase), phrase);
    //                        }
    //                    }
    //                    else {
    //                        for (int j = 0; j < coordCount; j++) {
    //                            highlightText(coords[j].lineNum, coords[j].colNum, wordLen, wordBuffer);
    //                        }
    //                    }
    //                }
    //                wordStart = i + 1;
    //            }
    //            i++;
    //        }
    //
    //        searchgotoxy(91, 5);
    //        if (phraseFound) {
    //            std::cout << "Phrase found.\n";
    //        }
    //        else {
    //            std::cout << "Phrase not found.\n";
    //        }
    //    }

        // Helper function to match next words in a phrase
        //bool matchNextWord(TreeNode* node, const char* nextWord, int& lineNum, int& colNum) {
        //    Coordinate tempCoords[100];
        //    int tempCount = 0;
        //    searchHelper(node, nextWord, 0, tempCoords, tempCount);

        //    for (int i = 0; i < tempCount; i++) {
        //        if (tempCoords[i].lineNum == lineNum && tempCoords[i].colNum == colNum) {
        //            return true;
        //        }
        //    }
        //    return false;
        //}


    void search() {
        char phrase[100];
        //move cursor to search section on screen
        // save current cursor position

        searchgotoxy(91, 2);
        std::cout << "Enter a phrase to search: ";
        searchgotoxy(91, 3);
        std::cin.getline(phrase, 100);
        searchPhrase(phrase);
    }

    void printWordsHelper(TreeNode* node, char* wordBuffer, int& depth) {
        if (node == nullptr) return;

        // Append current node's character to the word buffer
        wordBuffer[depth] = node->data;
        cout << node->data;
        // If this node is the end of a word, print it with location info
        if (node->isEndOfWord) {
            //   wordBuffer[depth + 1] = '\0';  // Null-terminate the word
            std::cout << "\nWord Position: \n";
            //   << " (" << node->lineNum
             //  << ", " << node->colNum << ")\n";
            for (int i = 0; i < node->instanceCount; i++) {
                std::cout << " Instance " << (i + 1) << " ("
                    << node->coordinates[i].lineNum << ", "
                    << node->coordinates[i].colNum << "),";
            }
            cout << endl;
        }
        ////////////////////////



        // Recursively call for all child nodes
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) {
                depth++;
                printWordsHelper(node->children[i], wordBuffer, depth);
            }
        }

    }


    // Public function to print all words in the tree
    void printWords() {
        char wordBuffer[100];  // Temporary buffer to store words
        int i = 0;
        cout << "\n";
        printWordsHelper(root, wordBuffer, i);
    }
};

