#include "dialog.h"
#include "sha1.h"
#include <iostream>
#include <cstring> // Для функции strlen

void printMenu() {
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "3. Delete Account" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    // Создаем объект Chat
    Chat chat;

    int choice;
    char login[LOGINLENGTH];
    char password[256];
    int pass_length;

    while (true) {
        printMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Enter login: ";
            cin >> login;
            cout << "Enter password: ";
            cin >> password;
            pass_length = strlen(password); 
            chat.reg(login, password, pass_length);
            cout << "Registration successful!" << endl;
            break;
        }
        case 2: {
            cout << "Enter login: ";
            cin >> login;
            cout << "Enter password: ";
            cin >> password;
            pass_length = strlen(password);  
            if (chat.login(login, password, pass_length)) {
                cout << "Login successful!" << endl;
            }
            else {
                cout << "Login failed!" << endl;
            }
            break;
        }
        case 3: {
            cout << "Enter login to delete: ";
            cin >> login;
            chat.del(login);
            cout << "Account deleted!" << endl;
            break;
        }
        case 4: {
            cout << "Exiting..." << endl;
            return 0;
        }
        default: {
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
        }
    }

    return 0;
}