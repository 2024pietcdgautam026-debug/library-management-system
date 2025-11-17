#include <iostream>
#include <fstream>
using namespace std;

class Book {
protected:
    int id;
    string title, author;
    bool issued;
public:
    Book() {}
    Book(int i, string t, string a) : id(i), title(t), author(a), issued(false) {}

    int getID() const { return id; }
    bool isIssued() const { return issued; }
    void issue() { issued = true; }
    void ret() { issued = false; }

    virtual void display() const {
        cout << id << " | " << title << " | " << author
             << (issued ? " | Issued\n" : " | Available\n");
    }

    string serialize() const {
        return to_string(id) + "," + title + "," + author + "," + (issued ? "1" : "0") + "\n";
    }

    static Book deserialize(string line) {
        string a, b, c, d;
        int p1 = line.find(',');
        int p2 = line.find(',', p1 + 1);
        int p3 = line.find(',', p2 + 1);

        a = line.substr(0, p1);
        b = line.substr(p1 + 1, p2 - p1 - 1);
        c = line.substr(p2 + 1, p3 - p2 - 1);
        d = line.substr(p3 + 1);

        Book bk(stoi(a), b, c);
        if (d == "1") bk.issued = true;
        return bk;
    }
};


class RareBook : public Book {
public:
    RareBook() {}
    RareBook(int i, string t, string a) : Book(i, t, a) {}

    void display() const override {
        cout << id << " | " << title << " | " << author
             << " | RARE | " << (issued ? "Issued\n" : "Available\n");
    }
};

template <typename T>
void showBooks(T arr[], int count) {
    for (int i = 0; i < count; i++)
        arr[i].display();
}


class Library {
public:
    Book books[200];
    int count = 0;

    void addBook() {
        int id, type;
        string title, author;

        cout << "Enter Book ID: ";
        cin >> id;

        if (cin.fail()) { handleInputError(); return; }

        cin.ignore();
        cout << "Title: ";
        getline(cin, title);
        cout << "Author: ";
        getline(cin, author);

        cout << "Type (1: Normal, 2: Rare): ";
        cin >> type;

        if (cin.fail()) { handleInputError(); return; }

        if (type == 1) books[count] = Book(id, title, author);
        else books[count] = RareBook(id, title, author);

        count++;
        cout << "Book Added Successfully.\n";
    }

    void searchBook() {
        int id;
        cout << "Enter Book ID: ";
        cin >> id;

        if (cin.fail()) { handleInputError(); return; }

        for (int i = 0; i < count; i++) {
            if (books[i].getID() == id) {
                books[i].display();
                return;
            }
        }
        cout << "Book Not Found.\n";
    }

    void issueBook() {
        int id;
        cout << "Enter Book ID to Issue: ";
        cin >> id;

        if (cin.fail()) { handleInputError(); return; }

        for (int i = 0; i < count; i++) {
            if (books[i].getID() == id) {
                if (books[i].isIssued())
                    cout << "Book Already Issued.\n";
                else {
                    books[i].issue();
                    cout << "Book Issued Successfully.\n";
                }
                return;
            }
        }
        cout << "Book Not Found.\n";
    }

    void returnBook() {
        int id;
        cout << "Enter Book ID to Return: ";
        cin >> id;

        if (cin.fail()) { handleInputError(); return; }

        for (int i = 0; i < count; i++) {
            if (books[i].getID() == id) {
                if (!books[i].isIssued())
                    cout << "Book Was Not Issued.\n";
                else {
                    books[i].ret();
                    cout << "Book Returned Successfully.\n";
                }
                return;
            }
        }
        cout << "Book Not Found.\n";
    }

    void saveToFile() {
        ofstream fout("library.txt");
        for (int i = 0; i < count; i++)
            fout << books[i].serialize();
        fout.close();
        cout << "Data Saved.\n";
    }

    void loadFromFile() {
        ifstream fin("library.txt");
        if (!fin) return;

        string line;
        while (getline(fin, line)) {
            books[count] = Book::deserialize(line);
            count++;
        }
        fin.close();
    }

    void showAll() {
        if (count == 0)
            cout << "No Books Available.\n";
        else
            showBooks(books, count);
    }

public:
    void handleInputError() {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid Input! Returning to Menu.\n";
    }
};


int main() {
    Library lib;
    lib.loadFromFile();

    while (true) {
        cout << "\n========== LIBRARY MENU ==========\n";
        cout << "1. Add Book\n"
             << "2. Search Book\n"
             << "3. Issue Book\n"
             << "4. Return Book\n"
             << "5. Show All Books\n"
             << "6. Save & Exit\n";
        cout << "Enter Choice: ";

        int ch;
        cin >> ch;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid Choice!\n";
            continue;
        }

        switch (ch) {
        case 1: lib.addBook(); break;
        case 2: lib.searchBook(); break;
        case 3: lib.issueBook(); break;
        case 4: lib.returnBook(); break;
        case 5: lib.showAll(); break;
        case 6: lib.saveToFile(); return 0;
        default: cout << "Invalid Choice!\n";
        }
    }
}