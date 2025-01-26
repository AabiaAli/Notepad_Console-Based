#pragma once
#include "Global Functions.h"
#include "N-aryTree.h"
#include "Undo Redo.h"



// A class representing a simple notepad
class Notepad {
private:
    Node* head;    // The head of the linked list for the first line
    Node* cursor;  // The current cursor position
    int cursorX, cursorY; // Current coordinates of the cursor (visual)

    Stack undoStack;   // Stack for undo functionality
    Stack redoStack;   // Stack for redo functionality

    NaryTree searchTree;      // N-ary tree instance for word storage
    char wordBuffer[20];      // Temporary buffer to store each word (up to 20 chars)
    int wordLength;           // Current length of the word in buffer

    //  SuggestionList suggestionList; // List to hold suggestions

public:
    NaryTree getTree() {
        return searchTree;
    }


    // FILE HANDLING


     // Function to load a file (reads one character at a time using .get())
    void loadFile(const fs::path& filePath) {
        std::ifstream file(filePath, std::ios::in);
        if (!file.is_open()) {
            std::cerr << "Could not open the file!" << std::endl;
            return;
        }

        char ch;
        while (file.get(ch)) {
            if (ch == '\n') {
                handleEnterKey();
                continue;
            }
            insertCharacter(ch);
        }

        file.close();
        std::cout << "File loaded successfully." << std::endl;
        printDocument();
    }

    // Function to save the document to a file (writes character by character)
    void saveFile(const fs::path& filePath) {
        std::ofstream file(filePath, std::ios::out);
        if (!file.is_open()) {
            std::cerr << "Could not open the file for writing!" << std::endl;
            return;
        }

        Node* currentLine = head;
        while (currentLine != nullptr) {
            Node* currentNode = currentLine;
            while (currentNode != nullptr && currentNode->data != '\n') {
                char ch = currentNode->data;
                file << ch;
                currentNode = currentNode->right;
            }
            file << '\n';
            currentLine = currentLine->down;
        }

        file.close();
        std::cout << "File saved successfully." << std::endl;
    }

    // Create a new file (clear the document)
    void newFile() {
        clearDocument();  // Implement a function to clear the document (reset head and cursor)
        cursorX = 0;
        cursorY = 0;
        std::cout << "New file created." << std::endl;
    }

    // Clear the entire document (resetting the linked list)
    void clearDocument() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextLine = current->down;
            while (current != nullptr) {
                Node* temp = current;
                current = current->right;
                delete temp;
            }
            current = nextLine;
        }

        // Reinitialize the head and cursor after clearing
        head = new Node(' ');
        cursor = head;
    }

    // Display a simple text-based menu
    void displayMenu() {
        bool displaying = true;
        // while (displaying) {
        std::cout << "\n----- Notepad Menu -----\n";
        std::cout << "1. New File\n";
        std::cout << "2. Load File\n";
        std::cout << "3. Save File\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";

    }

    // Undo the last insert operation
    void undo() {
        if (!undoStack.isEmpty()) {
            StackNode* lastAction = undoStack.pop();
            redoStack.push(lastAction->wordNode, lastAction->operation);  // Push the action to redo stack

            Node* wordHead = lastAction->wordNode->head;
            Node* wordTail = wordHead;
            for (int i = 0; i < lastAction->wordNode->length - 1; i++) {
                wordTail = wordTail->right;
            }
            cout << endl << " -------" << endl;
            cout << "wordlength : " << lastAction->wordNode->length << endl;
            cout << "wordHead : " << wordHead->data << endl;
            cout << "wordTail : " << wordTail->data << endl;

            cursor = wordTail;
            int i = 0;
            if (wordTail->right != nullptr && wordTail->right->data == ' ') {
                i--;
            }

            for (i; i < lastAction->wordNode->length; i++) {
                deleteCharacter();
            }

            delete lastAction->wordNode;
            linkVerticalNodes();
            printDocument();
            gotoxy(cursorX, cursorY);
        }
    }


    // Redo the last undone operation
    void redo() {
        if (!redoStack.isEmpty()) {
            auto lastAction = redoStack.pop();
            undoStack.push(lastAction->wordNode, 1);  // Push the action to undo stack

            Node* wordHead = lastAction->wordNode->head;
            Node* wordTail = wordHead;
            for (int i = 0; i < lastAction->wordNode->length; i++) {
                wordTail = wordTail->right;
            }

            // Insert the word back into the document
            wordTail->right = cursor->right;
            if (cursor->right != nullptr) {
                cursor->right->left = wordTail;
            }
            cursor->right = wordHead;
            wordHead->left = cursor;

            // Update the cursor position
            cursor = wordTail;
            cursorX += lastAction->wordNode->length;

            printDocument();
            gotoxy(cursorX, cursorY);
        }
    }




    // A class representing a simple notepad
    Notepad() : cursorX(0), cursorY(0), wordLength(0) {
        head = new Node(' ');  // Initialize head and cursor as usual
        cursor = head;
    }

    // Move cursor to specific X, Y position on the console
    void gotoxy(int x, int y) {
        COORD coord = { x + 1, y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }


    // Print the entire document, including linked lines
    int NumLines;
    int LastLineLength;
    void printDocument() {
        system("CLS");  // Clear the screen for clarity
        int numLines = 0;
        int lineLength = 0;
        Node* currentLine = head;
        while (currentLine != nullptr) {
            Node* currentNode = currentLine;
            lineLength = 0;
            if (currentNode != nullptr) {
                /*  while (currentNode != nullptr && currentNode->data != '\n') {
                      cout << currentNode->data;
                      currentNode = currentNode->right;
                      lineLength++;
                  }*/
                while (currentNode != nullptr) {
                    if (currentNode != nullptr) {
                        if (currentNode->data == '\n') {
                            break;
                        }
                        cout << currentNode->data;
                        currentNode = currentNode->right;
                        lineLength++;
                    }
                }
            }
            for (int i = 0; i < MAX_SCREEN_WIDTH - lineLength; i++) {
                cout << " ";
            }
            cout << '|';
            if (numLines == 0) {
                cout << "Search: ";
            }
            cout << endl;
            currentLine = currentLine->down;
            numLines++;
        }

        // Print empty lines to fill the screen
        for (int i = numLines; i <= 20; i++) {
            for (int j = 0; j < MAX_SCREEN_WIDTH; j++) {
                cout << " ";
            }
            cout << '|';
            if (numLines == 0) {
                cout << "Search: ";
            }
            cout << endl;
        }
        for (int i = 0; i < MAX_SCREEN_WIDTH; i++) {
            cout << "_";
        }
        cout << "|" << endl;
        cout << "Suggestions Area: " << endl;
        NumLines = numLines;
        LastLineLength = lineLength;
    }
    void selectSuggestion(int index) {

        cout << "Selected Suggestion: " << suggestions[index - 1] << endl;
        // Now insert the selected suggestion into the document from suggestions array
        for (int i = 2; suggestions[index - 1][i] != '\0'; i++) {
            insertCharacter(suggestions[index - 1][i]);
        }

    }

    // Add a method to handle key presses for suggestion selection
    void handleSuggestionSelection(int key) {
        if (key >= '0' && key <= '9') {
            int index = key - '0'; // Convert char to int
            selectSuggestion(index);
        }
    }
    void insertCharacter(char ch) {

        if (ch == '9') {
            // Get the prefix for suggestions
            char prefix[100];
            copyN(prefix, wordBuffer, wordLength);
            prefix[wordLength] = '\0';  // Null-terminate the prefix

            // std::cout << "Suggestions Area: " << std::endl;
            searchTree.suggestWords(prefix);  // Call the suggestion method

            return;  // Skip the '@' character insertion
        }
        // if number of lines == max lines and length of current line == max line length OR if cursorY == max screen height and enter key is pressed
        // then return without insertion 
        // check condition by first calculating number of lines and checking last line length

        if (cursorY >= MAX_SCREEN_HEIGHT && cursorX >= MAX_LINE_LENGTH - 1 || NumLines >= MAX_SCREEN_HEIGHT && LastLineLength >= MAX_LINE_LENGTH - 1) {
            return;
        }

        // Create a new node with the given character
        Node* newNode = new Node(ch);

        // Insert the character at the current cursor position
        newNode->right = cursor->right;
        newNode->left = cursor;

        // Link new node to the right node, if it exists
        if (cursor->right != nullptr) {
            cursor->right->left = newNode;
        }

        cursor->right = newNode;
        cursor = newNode;  // Move cursor to the new character

        // **If inserting at the first node of the new line, ensure vertical linkage**
        if (cursor == head->down) {
            head->down = cursor;
            cursor->up = head;
        }

        // Move the visual cursor to the right
        cursorX++;

        // Print the updated document
        printDocument();
        handleWordWrap();
        linkVerticalNodes();
        gotoxy(cursorX, cursorY);  // Visually move the cursor

        // Check if the character is a space or enter key
        if (ch == ' ' || ch == '\n') {
            // Traverse back to find the entire word
            Node* wordStart = cursor;
            int wordLength = 0;
            // calculate length of word
            while (wordStart->left != nullptr && wordStart->left->data != ' ') {
                wordStart = wordStart->left;
                wordLength++;
            }

            WordNode* wordNode = new WordNode(wordStart, wordLength, cursorY, cursorX - wordLength);

            // Push the word to the undo stack
            undoStack.push(wordNode, 1);
        }

        if (ch != ' ' && ch != '\n' && ch != '9') {
            // Add character to word buffer
            wordBuffer[wordLength++] = ch;
            wordBuffer[wordLength] = '\0';  // Null-terminate
        }
        else {
            // Insert word into N-ary tree if space or newline is encountered
            if (wordLength > 0) {
                searchTree.insertWord(wordBuffer, cursorY, cursorX - wordLength);
                wordLength = 0;  // Reset buffer for the next word
            }
        }



        //  searchTree.printWords();
    }

    void handleWordWrap() {
        // check if characters are being inserted in middle of line and not at the end
        bool LastWord = true;
        int originalX;
        int originalY;
        Node* originalCursor = NULL;


        int lineX = cursorX;
        Node* lastChar = cursor;
        bool prepend = true;

        // move to last character of current line and delete \n
        while (lastChar != nullptr && lastChar->right != nullptr && lastChar->data != '\n') {
            if (lastChar->right->data == '\n')
            {
                prepend = false;
                //  lastChar->right = lastChar->right->right;
            }
            lastChar = lastChar->right;
            lineX++;
        }
        // Check if the current line exceeds the maximum length
        if (lineX >= MAX_LINE_LENGTH) {
            Node* temp = lastChar;

            // Move left to find the start of the word
            while (temp->left != nullptr && temp->data != ' ') {
                temp = temp->left;
            }

            // If a space was found, move the word to the next line
            if (temp->data == ' ') {
                /*  if (prepend == true) {
                      prependWordToNextLine(temp->right, cursor);
                  }*/
                  // else 
                moveWordToNextLine(temp->right);  // Move word after space to the new line
            }
            else {
                moveWordToNextLine(lastChar);  // No space, move entire word
            }
        }

    }

    void moveWordToNextLine(Node* wordStart) {
        // Ensure a next line exists, create one if needed
        if (cursor->down == nullptr) {
            handleEnterKey();  // Create a new line if none exists
            // get rid of \n in current line by moving back up to current line, deleting the last character
            cursor = cursor->up;
            while (cursor != nullptr && cursor->right->data != '\n') {
                cursor = cursor->right;
            }
            cursor->right = cursor->right->right;
            // move back down to new line by first going back to first character and moving down
            while (cursor->left != nullptr) {
                cursor = cursor->left;
            }
            cursor = cursor->down;


        }
        else {
            cursor = cursor->down;
            cursorY++;
        }

        // Move to the start of the next line and skip the first space if it exists
        while (cursor->left != nullptr) {
            cursor = cursor->left;
        }


        cursorX = 0;  // Reset cursorX for the new line
        gotoxy(cursorX, cursorY);  // Visually move the cursor
        Node* nextLineCursor = cursor;  // Start from the first character of the new line

        // Move the entire word to the new line
        Node* temp = wordStart;
        while (temp) {
            if (temp->data != ' ' && temp->data != '\n') {
                // Insert the character in the new line
                insertCharacter(temp->data);

                // Move to the next character in the word
                temp = temp->right;
            }
        }

        // Unlink the word from the previous line
        if (wordStart != nullptr && wordStart->left != nullptr) {
            wordStart->left->right = nullptr;  // Disconnect the word from the previous line
        }

    }



    void prependWordToNextLine(Node* wordStart, Node* nextLineStart) {
        Node* temp = wordStart;

        // Traverse to the first character of the next line
        while (nextLineStart->left != nullptr) {
            nextLineStart = nextLineStart->left;
        }

        // Insert the word at the start of the next line
        while (temp != nullptr && temp->data != ' ') {
            Node* newNode = new Node(temp->data);

            // Prepend the new node to the next line
            newNode->right = nextLineStart;
            if (nextLineStart != nullptr) {
                nextLineStart->left = newNode;
            }

            nextLineStart = newNode;
            temp = temp->right;
        }

        // Set the cursor to the start of the next line
        cursor = nextLineStart;
        cursorX = 0;  // Reset X position to the start of the next line
        cursorY++;

        // If the next line exceeds the maximum line length, handle word wrap
        handleWordWrap();
    }


    void handleEnterKey() {
        if (wordLength > 0) {
            searchTree.insertWord(wordBuffer, cursorY, cursorX - wordLength);
            wordLength = 0;  // Reset buffer
        }

        // if max number of lines is reached and enter is pressed, return
        if (cursorY >= MAX_SCREEN_HEIGHT || NumLines > MAX_SCREEN_HEIGHT) {
            return;
        }

        // Create a new node for the start of the new line
        Node* newLineStart = new Node(' ');  // New line starts with an empty space
        Node* firstChar = cursor;
        while (firstChar->left != nullptr) {
            firstChar = firstChar->left;
        }
        // Save the current line's down link (if any)
        Node* originalDown = firstChar->down;

        // Move characters after the cursor to the new line
        Node* temp = cursor->right;
        cursor->right = new Node('\n');//nullptr;  // Disconnect the right part of the current line
        // cursor->right->right = newLineStart;
        cursor->right->right = nullptr;
        Node* newLineCursor = newLineStart;

        // Move characters from the current line (right of the cursor) to the new line
        while (temp != nullptr) {
            Node* newNode = new Node(temp->data);

            // Link horizontally in the new line
            newLineCursor->right = newNode;
            newNode->left = newLineCursor;

            // Link vertically (up/down)
            newNode->up = temp;
            temp->down = newNode;

            newLineCursor = newNode;  // Move to the next node in the new line

            Node* toDelete = temp;
            temp = temp->right;
            delete toDelete;  // Clean up old nodes
        }



        // **Fix the vertical linkage between the first nodes**
        if (firstChar == head) {
            // If we're splitting the first line, head needs to point to the new line
            head->down = newLineStart;
            newLineStart->up = head;
        }
        else {
            // If it's not the first line, link the current line's first node to the new line's first node
            firstChar->down = newLineStart;
            newLineStart->up = firstChar;
        }

        // Link the new line vertically to the one below (if it exists)
        if (originalDown != nullptr) {
            newLineStart->down = originalDown;
            originalDown->up = newLineStart;
        }

        // Move the cursor to the start of the new line
        cursor = newLineStart;  // Move cursor to the new line's head
        cursorX = 0;  // Reset X position to the start of the new line
        cursorY++;    // Move down visually


        linkVerticalNodes();

        // Print document and show debug info
        printDocument();
        //    debugDocument();  // Show detailed node linkage
        gotoxy(cursorX, cursorY);  // Move the cursor visually to the new line
    }



    void debugDocument() {
        cout << "\n--- Debugging Document ---\n";
        Node* line = head;
        int lineIndex = 0;

        while (line != nullptr) {
            Node* temp = line;
            cout << "Line " << lineIndex << ": ";

            // Traverse each node horizontally
            while (temp != nullptr) {
                if (temp->data == '\n') {
                    cout << "\\n";
                }
                cout << temp->data;  // Print character
                if (temp->right != nullptr) {
                    cout << "<->";  // Show horizontal linkage
                }
                temp = temp->right;
            }

            cout << " (down: " << (line->down != nullptr ? "linked" : "none") << ")";
            cout << endl;

            // Move to the next line
            line = line->down;
            lineIndex++;
        }

        cout << "--- End of Document ---\n";
        //  system("pause");  // Pause the output so you can see the debug info
    }

    void deleteCharacter() {
        if (cursor->left != nullptr) {
            // Regular character deletion
            Node* temp = cursor;
            cursor = cursor->left;
            cursor->right = temp->right;
            if (temp->right != nullptr) {
                temp->right->left = cursor;
            }
            delete temp;
            cursorX--;  // Move visual cursor left
        }
        // if first character
        else if (cursor->up != nullptr) {
            // Backspace at the first character of a line, move up to previous line
            Node* prevLineLastChar = cursor->up;

            // Move to the last character of the previous line (ignoring null pointers)
            while (prevLineLastChar->right != nullptr && prevLineLastChar->right->data != '\n') {
                prevLineLastChar = prevLineLastChar->right;
            }

            // Check if the current line is empty (only contains '\n')
            if (cursor->right != nullptr && cursor->right->data == '\n') {
                Node* temp = cursor->right;
                cursor->up->down = cursor->down;
                if (cursor->down != nullptr) {
                    cursor->down->up = cursor->up;
                }
                linkVerticalNodes();
                cursor = prevLineLastChar;
                cursorX = findLineLength(prevLineLastChar);  // Move to the last character's X position
                cursorY--;  // Move cursor up visually
                delete temp;  // Remove '\n' and clear the empty line
            }
            else {
                // Join current line with the above line by deleting '\n'
                if (prevLineLastChar->right == nullptr || prevLineLastChar->right->data == '\n') {

                    Node* firstCharCurr = cursor;
                    Node* firstCharprev = prevLineLastChar;
                    // relink the first nodes vertically

                    while (firstCharprev->left != nullptr) {
                        firstCharprev = firstCharprev->left;
                    }
                    firstCharprev->down = firstCharCurr->down;
                    if (firstCharCurr->down != nullptr) {
                        firstCharCurr->down->up = firstCharprev;
                    }

                    // Delete '\n' and shift content
                    Node* toDelete = prevLineLastChar->right;
                    prevLineLastChar->right = cursor->right;




                    if (cursor->right != nullptr) {
                        cursor->right->left = prevLineLastChar;
                    }

                    // Delete the newline node
                    delete toDelete;

                    // Move cursor to the previous line's end
                    cursor = prevLineLastChar;
                    cursorX = findLineLength(prevLineLastChar);  // Move to the last character's X position
                    cursorY--;  // Move cursor up visually
                }
            }
        }
        else {
            // Nothing to delete if we are at the start of the first line
            return;
        }
        linkVerticalNodes();
        // Update document and visual cursor
        printDocument();
        gotoxy(cursorX, cursorY);

        // Handle word wrapping
        handleWordWrap();
    }


    // Helper to find the length of the current line for visual cursor placement
    int findLineLength(Node* lastChar) {
        Node* lineEnd = lastChar;
        int len = 0;
        while (lineEnd->left != nullptr) {
            lineEnd = lineEnd->left;
            len++;
        }
        return len;
    }


    void moveCursor(int key) {
        Node* Fchar = cursor;

        switch (key) {
        case VK_UP: // Up arrow
            // Find first character of the line
            while (Fchar->left != nullptr) {
                Fchar = Fchar->left;
            }
            if (cursor->up != nullptr) {
                cursor = cursor->up;
                cursorY--;
            }
            // If a line exists above but is shorter, move to its last character
            else if (Fchar->up != nullptr) {
                cursor = Fchar->up;
                cursorY--;
                cursorX = 0;
                while (cursor->right != nullptr && cursor->right->data != '\n') {  // Stop at '\n'
                    cursor = cursor->right;
                    cursorX++;
                }
            }
            break;

        case VK_DOWN: // Down arrow
            // Find first character of the line
            while (Fchar->left != nullptr) {
                Fchar = Fchar->left;
            }

            if (cursor->down != nullptr) {
                cursor = cursor->down;
                cursorY++;
            }
            // If a line exists below but is shorter, move to its last character
            else if (Fchar->down != nullptr) {
                cursor = Fchar->down;
                cursorY++;
                cursorX = 0;
                while (cursor->right != nullptr && cursor->right->data != '\n') {  // Stop at '\n'
                    cursor = cursor->right;
                    cursorX++;
                }
            }
            break;

        case VK_LEFT: // Left arrow
            // If there's no left node and we are at the start of the line
            if (cursor->left == nullptr) {
                if (cursor->up != nullptr) {  // Check if there's a line above
                    // Move to the line above and go to the last character
                    cursor = cursor->up;
                    cursorY--;
                    // Traverse to the last character of the line above
                    while (cursor->right != nullptr && cursor->right->data != '\n') {
                        cursor = cursor->right;
                        cursorX++;
                    }
                }
            }
            else {
                cursor = cursor->left;
                cursorX--;
            }
            break;

        case VK_RIGHT: // Right arrow
            Fchar = cursor;
            while (Fchar->left != nullptr) {
                Fchar = Fchar->left;
            }
            // If there's no right node and we are at the end of the line or at a newline
            if (cursor->right == nullptr || cursor->right->data == '\n') {
                if (Fchar->down != nullptr) {  // Check if there's a line below
                    // Move to the line below and go to the first character
                    cursor = Fchar->down;
                    cursorY++;
                    // Set cursorX to the start of the new line
                    cursorX = 0;
                }
            }
            else {
                cursor = cursor->right;
                cursorX++;
            }
            break;
        }

        gotoxy(cursorX, cursorY);  // Visually move the cursor
    }


    void linkVerticalNodes() {
        Node* currentLine = head;      // Start from the first line (head)
        Node* nextLine = head->down;   // Next line to link

        // Traverse all lines
        while (nextLine != nullptr) {
            Node* currentNode = currentLine;
            Node* nextNode = nextLine;

            // Link the nodes vertically between the two lines
            while (currentNode != nullptr && nextNode != nullptr) {
                currentNode->down = nextNode;   // Link current node's down pointer to the next line's node
                nextNode->up = currentNode;     // Link next line's node's up pointer to the current node

                // Move to the next node in both lines
                currentNode = currentNode->right;
                nextNode = nextNode->right;
            }

            // If one line is longer, ensure the remaining nodes have their down/up pointers set to nullptr
            while (currentNode != nullptr) {
                currentNode->down = nullptr;
                currentNode = currentNode->right;
            }

            while (nextNode != nullptr) {
                nextNode->up = nullptr;
                nextNode = nextNode->right;
            }

            // Move to the next pair of lines
            currentLine = nextLine;
            nextLine = nextLine->down;
        }
    }


    ~Notepad() {
        Node* currentLine = head;
        //   Node* nextLine = currentLine->down;
        while (currentLine != nullptr) {
            Node* currentNode = currentLine;
            while (currentNode != nullptr && currentNode->right != nullptr) {
                Node* temp = currentNode;
                currentNode = currentNode->right;
                delete temp;
            }
            //currentLine = nextLine;
            currentLine = currentLine->down;
        }
    }

};