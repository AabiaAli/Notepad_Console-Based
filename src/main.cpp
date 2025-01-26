
#include "Notepad.h"


int main() {
    Notepad notepad;  // Create a Notepad instance
    bool running = true;  // Flag to keep the program running
    HANDLE rhnd = GetStdHandle(STD_INPUT_HANDLE);  // Handle for console input events

    while (running) {
        system("cls");  // Clear the console
        notepad.displayMenu();  // Display the menu

        int choice;

        cin >> choice;  // Read user choice
        cin.ignore();   // Ignore the newline character 

        fs::path filename;

        switch (choice) {
        case 1:  // New File
            cout << "New file created.\n";
            system("cls");  // Clear the console for new file input

            // Enter text input mode
            while (running) {
                DWORD events = 0, eventsRead = 0;
                GetNumberOfConsoleInputEvents(rhnd, &events);

                if (events != 0) {
                    INPUT_RECORD eventBuffer[200];
                    ReadConsoleInput(rhnd, eventBuffer, events, &eventsRead);

                    for (DWORD i = 0; i < eventsRead; ++i) {
                        if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {
                            switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {
                            case VK_UP:
                            case VK_DOWN:
                            case VK_LEFT:
                            case VK_RIGHT:
                                notepad.moveCursor(eventBuffer[i].Event.KeyEvent.wVirtualKeyCode);
                                break;
                            case VK_BACK:
                                notepad.deleteCharacter();
                                break;
                            case VK_RETURN:
                                notepad.handleEnterKey();
                                break;
                            case VK_ESCAPE:  // Exit typing mode and go back to menu
                                cout << "\nExiting text editor...\n";
                                goto show_menu;  // Go to show_menu label
                            case VK_TAB:  // Undo
                                notepad.undo();
                                notepad.handleEnterKey();
                                notepad.deleteCharacter();
                                notepad.moveCursor(VK_RIGHT);
                                break;
                            case VK_SHIFT:  // Redo
                                notepad.redo();
                                break;
                            case '0': // search
                                notepad.getTree().search();
                                break;
                            case '1': // Select suggestion 1
                            case '2': // Select suggestion 2
                            case '3': // Select suggestion 3
                            case '4': // Select suggestion 4
                            case '5': // Select suggestion 5
                            case '6': // Select suggestion 6
                            case '7': // Select suggestion 7
                            case '8': // Select suggestion 8
                                // case '9': // Select suggestion 9
                                notepad.handleSuggestionSelection(eventBuffer[i].Event.KeyEvent.wVirtualKeyCode);
                                break;
                            default:
                                // Insert the character if it's a valid character
                                char ch = eventBuffer[i].Event.KeyEvent.uChar.AsciiChar;
                                if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == ' ' || ch == '9') {
                                    notepad.insertCharacter(ch);
                                }
                                break;
                            }
                        }

                        // Handle suggestion selection if Shift key is pressed
                        if (eventBuffer[i].Event.KeyEvent.bKeyDown &&
                            eventBuffer[i].Event.KeyEvent.wVirtualKeyCode >= '0' &&
                            eventBuffer[i].Event.KeyEvent.wVirtualKeyCode <= '9' &&
                            (GetAsyncKeyState(VK_SHIFT) & 0x8000)) {
                            notepad.handleSuggestionSelection(eventBuffer[i].Event.KeyEvent.wVirtualKeyCode);
                        }
                    }
                }
            }

            break;

        case 2:  // Load File
            cout << "Enter the filename to load: ";
            cin >> filename;
            notepad.loadFile(filename);  // Pass the filename to loadFile
            cout << "File loaded successfully!" << endl;
            system("cls");  // Clear the console for new file input

            // Enter text input mode
            while (running) {
                DWORD events = 0, eventsRead = 0;
                GetNumberOfConsoleInputEvents(rhnd, &events);

                if (events != 0) {
                    INPUT_RECORD eventBuffer[200];
                    ReadConsoleInput(rhnd, eventBuffer, events, &eventsRead);

                    for (DWORD i = 0; i < eventsRead; ++i) {
                        if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {
                            switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {
                            case VK_UP:
                            case VK_DOWN:
                            case VK_LEFT:
                            case VK_RIGHT:
                                notepad.moveCursor(eventBuffer[i].Event.KeyEvent.wVirtualKeyCode);
                                break;
                            case VK_BACK:
                                notepad.deleteCharacter();
                                break;
                            case VK_RETURN:
                                notepad.handleEnterKey();
                                break;
                            case VK_ESCAPE:  // Exit typing mode and go back to menu
                                cout << "\nExiting text editor...\n";
                                goto show_menu;  // Go to show_menu label
                            case VK_TAB:  // Undo
                                notepad.undo();
                                notepad.handleEnterKey();
                                notepad.deleteCharacter();
                                notepad.moveCursor(VK_RIGHT);
                                break;
                            case VK_SHIFT:  // Redo
                                notepad.redo();
                                break;
                            case '0': // search
                                notepad.getTree().search();
                                break;
                            case '1': // Select suggestion 1
                            case '2': // Select suggestion 2
                            case '3': // Select suggestion 3
                            case '4': // Select suggestion 4
                            case '5': // Select suggestion 5
                            case '6': // Select suggestion 6
                            case '7': // Select suggestion 7
                            case '8': // Select suggestion 8
                            case '9': // Select suggestion 9
                                notepad.handleSuggestionSelection(eventBuffer[i].Event.KeyEvent.wVirtualKeyCode);
                                break;
                            default:
                                // Insert the character if it's a valid character
                                char ch = eventBuffer[i].Event.KeyEvent.uChar.AsciiChar;
                                if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == ' ' || ch == '9') {
                                    notepad.insertCharacter(ch);
                                }
                                break;
                            }
                        }

                        // Handle suggestion selection if Shift key is pressed
                        if (eventBuffer[i].Event.KeyEvent.bKeyDown &&
                            eventBuffer[i].Event.KeyEvent.wVirtualKeyCode >= '0' &&
                            eventBuffer[i].Event.KeyEvent.wVirtualKeyCode <= '9' &&
                            (GetAsyncKeyState(VK_SHIFT) & 0x8000)) {
                            notepad.handleSuggestionSelection(eventBuffer[i].Event.KeyEvent.wVirtualKeyCode);
                        }
                    }
                }
            }

            break;

        case 3:  // Save File
            cout << "Enter the filename to save: ";
            cin >> filename;
            notepad.saveFile(filename);  // Pass the filename to saveFile
            break;

        case 4:  // Exit
            std::cout << "Do you want to save before exiting? (y/n): ";
            char saveBeforeExit;
            std::cin >> saveBeforeExit;
            if (saveBeforeExit == 'y') {
                cout << "Enter file path to save: ";
                cin >> filename;
                notepad.saveFile(filename);
            }

            cout << "Exiting the program..." << endl;
            running = false;  // Exit the program
            break;

        default:
            cout << "Invalid option. Please choose again." << endl;
            system("pause");
            system("cls");
            goto show_menu;
            break;
        }

    show_menu: // Label to return to after text input mode
        continue;  // Continue to show the menu after breaking out of text mode
    }

    return 0;
}