#ifndef BOOKMANAGEMENT_LOGIC_H
#define BOOKMANAGEMENT_LOGIC_H

#include "yukkk.h"
#include "Book.h"

// Forward declaration
void menu();
void addBook();
void listBook();
void searchBook();
void updateBook();
void deleteBook();

/*
=========================
=                       =
=   Book Management     =
=                       =
=========================
*/
void bookLogo()
{

    cout << "\n\n\n";

    cout << "\033[38;2;0;255;0m   ██████╗  ██████╗  ██████╗ ██╗  ██╗███████╗\033[0m" << endl;
    cout << "\033[38;2;0;255;95m   ██╔══██╗██╔═══██╗██╔═══██╗██║ ██╔╝██╔════╝\033[0m" << endl;
    cout << "\033[38;2;0;255;135m   ██████╔╝██║   ██║██║   ██║█████╔╝ ███████╗\033[0m" << endl;
    cout << "\033[38;2;0;255;135m   ██╔══██╗██║   ██║██║   ██║██╔═██╗ ╚════██║\033[0m" << endl;
    cout << "\033[38;2;0;255;175m   ██████╔╝╚██████╔╝╚██████╔╝██║  ██╗███████║\033[0m" << endl;
    cout << "\033[38;2;0;255;175m   ╚═════╝  ╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚══════╝\033[0m" << endl;

    cout << "\n\n";
}
void bookMenu()
{

    string choice;
    do
    {

        system("cls");
        bookLogo();
        cout << "\033[38;2;0;255;0m    ╔═══[1]   Add Book\033[0m" << endl;
        cout << "\033[38;2;0;255;0m    ║\033[0m" << endl;
        cout << "\033[38;2;0;255;95m    ╠════[2]    List Book\033[0m" << endl;
        cout << "\033[38;2;0;255;95m    ║\033[0m" << endl;
        cout << "\033[38;2;0;255;135m    ╠══════[3]    Search Book\033[0m" << endl;
        cout << "\033[38;2;0;255;135m    ║\033[0m" << endl;
        cout << "\033[38;2;0;255;135m    ╠════════[4]    Update Book\033[0m" << endl;
        cout << "\033[38;2;0;255;135m    ║\033[0m" << endl;
        cout << "\033[38;2;0;255;135m    ╠══════════[5]    Delete Book\033[0m" << endl;
        cout << "\033[38;2;0;255;135m    ║\033[0m" << endl;
        cout << "\033[38;2;0;255;135m    ╠════════════[6]     Go Back" << endl;
        cout << "\033[38;2;0;255;175m    ║\033[0m" << endl;
        cout << "\033[38;2;0;255;175m    ╚═══>   ";
        cin >> choice;
        cout << "\033[0m";
        if (choice == "1")
        {
            addBook();
        }
        else if (choice == "2")
        {
            listBook();
        }
        else if (choice == "3")
        {
            searchBook();
        }
        else if (choice == "4")
        {
            updateBook();
        }
        else if (choice == "5")
        {
            deleteBook();
        }
        else if (choice == "6")
        {
            menu();
        }

    } while (choice != "1" && choice != "2" && choice != "3" && choice != "4" && choice != "5" && choice != "6");
}
void addBookLogo()
{

    system("cls");
    cout << "\n\n";

    cout << "\033[38;2;0;255;0m           ___     __   __  ___            __ \033[0m" << endl;
    cout << "\033[38;2;0;255;95m          / _ |___/ /__/ / / _ )___  ___  / /__\033[0m" << endl;
    cout << "\033[38;2;0;255;135m         / __ / _  / _  / / _  / _ \\/ _ \\/  '_/\033[0m" << endl;
    cout << "\033[38;2;0;255;175m        /_/ |_\\_,_/\\_,_/ /____/\\___/\\___/_/\\_\\ \033[0m" << endl;

    cout << "\n\n";
}
bool isDuplicateId(string book_id)
{

    string documentsPath = string(getenv("USERPROFILE")) + "\\Documents\\books.csv";

    ifstream ifile(documentsPath);

    string line;
    while (getline(ifile, line))
    {
        stringstream ss(line);
        string id;
        getline(ss, id, ','); // first column is ID
        if (id == book_id)
        {
            return true; // duplicate found
        }
    }

    return false;
}
void addBook()
{

    addBookLogo();

    cout << "\033[38;2;0;255;175m"; // color text

    vector<Book *> books;

    string book_id, book_title, book_author, book_year, book_genre;

    cout << endl;

    int count = 0;

    do
    {
        if (count != 0)
        {
            cout << "\033[38;2;215;0;0m\t║\033[0m" << endl;
            cout << "\033[38;2;215;0;0m\t╚═══this id already exists.\033[0m" << endl
                 << endl;
        }
        cout << "\033[38;2;0;255;0m\t╔═══Enter book's Id    : ";
        cin >> book_id;
        cin.ignore();
        cout << "\033[0m";
        count++;
    } while (isDuplicateId(book_id));
    cout << "\033[38;2;0;255;0m\t║\033[0m" << endl;
    cout << "\033[38;2;0;255;95m\t╠═══Enter book's Title : ";
    getline(cin, book_title);
    cout << "\033[0m";
    cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
    cout << "\033[38;2;0;255;135m\t╠═══Enter book's Author: ";
    getline(cin, book_author);
    cout << "\033[0m";
    cout << "\033[38;2;0;255;135m\t║\033[0m" << endl;
    cout << "\033[38;2;0;255;135m\t╠═══Enter book's Year  : ";
    cin >> book_year;
    cin.ignore();
    cout << "\033[0m";
    cout << "\033[38;2;0;255;175m\t║\033[0m" << endl;
    cout << "\033[38;2;0;255;175m\t╚═══Enter book's Genre : ";
    getline(cin, book_genre);
    cout << "\033[0m";

    string documentsPath = string(getenv("USERPROFILE")) + "\\Documents\\books.csv";

    ofstream file(documentsPath, ios::app);

    if (!file)
    {

        fail();

        string choice;

        do
        {

            cout << "\033[38;2;0;255;0m\t╔═══[1]     Go Back\033[0m" << endl;
            cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;135m\t╠══════[2]    Exit\033[0m" << endl;
            cout << "\033[38;2;0;255;175m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;175m\t╚═══>     ";
            cin >> choice;
            if (choice == "1")
            {
                bookMenu();
            }
            else if (choice == "2")
            {
                return;
            }

        } while (choice != "1" && choice != "2");
    }
    else
    {

        books.push_back(new Book(book_id, book_title, book_author, book_year, book_genre));
        // books.push_back(new Book("003", "Qt C++", "Yan Phayu", "2025", "Programming"));

        for (Book *temp : books)
        {
            file << temp->toString();
        }

        file.close();

        for (Book *b : books)
            delete b;

        success();

        string choice;

        do
        {

            cout << "\033[38;2;0;255;0m\t╔═══[1]     Go Back\033[0m" << endl;
            cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;135m\t╠══════[2]    Exit\033[0m" << endl;
            cout << "\033[38;2;0;255;175m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;175m\t╚═══>     ";
            cin >> choice;
            if (choice == "1")
            {
                bookMenu();
            }
            else if (choice == "2")
            {
                return;
            }

        } while (choice != "1" && choice != "2");
    }
}
void listBookLogo()
{

    system("cls");
    cout << "\n\n";

    cout << "\033[38;2;0;255;0m\t    __    _      __     ____              __  \033[0m" << endl;
    cout << "\033[38;2;0;255;95m\t   / /   (_)____/ /_   / __ )____  ____  / /__\033[0m" << endl;
    cout << "\033[38;2;0;255;135m\t  / /   / / ___/ __/  / __  / __ \\/ __ \\/ //_/\033[0m" << endl;
    cout << "\033[38;2;0;255;135m\t / /___/ (__  ) /_   / /_/ / /_/ / /_/ / ,<   \033[0m" << endl;
    cout << "\033[38;2;0;255;175m\t/_____/_/____/\\__/  /_____/\\____/\\____/_/|_|  \033[0m" << endl;

    cout << "\n\n";
}
void listBook()
{

    listBookLogo();

    string path = string(getenv("USERPROFILE")) + "\\Documents\\books.csv";
    ifstream file(path);

    cout << "\033[38;2;0;255;95m";
    cout << "\t╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "\t║ "
         << left
         << setw(15) << "id"
         << setw(30) << "title"
         << setw(30) << "author"
         << setw(15) << "year"
         << setw(30) << "genre"
         << "║\n";
    cout << "\t╠═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n";

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string id, title, author, year, genre;

        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, year, ',');
        getline(ss, genre, ',');

        cout << "\t║ "
             << left
             << setw(15) << id
             << setw(30) << title
             << setw(30) << author
             << setw(15) << year
             << setw(30) << genre
             << "║\n";
    }

    cout << "\t╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n\033[0m";
    file.close();

    string choice;
    do
    {

        cout << "\033[38;2;0;255;0m\t╔═══[1]     Go Back\033[0m" << endl;
        cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
        cout << "\033[38;2;0;255;135m\t╠══════[2]    Exit\033[0m" << endl;
        cout << "\033[38;2;0;255;175m\t║\033[0m" << endl;
        cout << "\033[38;2;0;255;175m\t╚═══>     ";
        cin >> choice;
        if (choice == "1")
        {
            bookMenu();
        }
        else if (choice == "2")
        {
            return;
        }

    } while (choice != "1" && choice != "2");
}
void searchBookLogo()
{

    system("cls");
    cout << "\n\n";

    cout << "\033[38;2;0;255;0m\t   _____                      __       ____              __  \033[0m" << endl;
    cout << "\033[38;2;0;255;95m\t  / ___/___  ____ ___________/ /_     / __ )____  ____  / /__\033[0m" << endl;
    cout << "\033[38;2;0;255;135m\t  \\__ \\/ _ \\/ __ `/ ___/ ___/ __ \\   / __  / __ \\/ __ \\/ //_/\033[0m" << endl;
    cout << "\033[38;2;0;255;135m\t ___/ /  __/ /_/ / /  / /__/ / / /  / /_/ / /_/ / /_/ / ,<   \033[0m" << endl;
    cout << "\033[38;2;0;255;175m\t/____/\\___/\\__,_/_/   \\___/_/ /_/  /_____/\\____/\\____/_/|_|  \033[0m" << endl;

    cout << "\n\n";
}
void searchBook()
{

    searchBookLogo();

    string search;

    cout << "\033[38;2;0;255;0m\t╔══════   Search by Id | Title | Author | Year | Genre\033[0m" << endl;
    cout << "\033[38;2;0;255;0m\t║\033[0m" << endl;
    cout << "\033[38;2;0;255;0m\t╚═══>   ";

    cin.ignore();
    getline(cin, search);

    cout << "\n";

    string path = string(getenv("USERPROFILE")) + "\\Documents\\books.csv";
    ifstream file(path);

    cout << "\033[38;2;0;255;95m";
    cout << "\t╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "\t║ "
         << left
         << setw(15) << "id"
         << setw(30) << "title"
         << setw(30) << "author"
         << setw(15) << "year"
         << setw(30) << "genre"
         << "║\n";
    cout << "\t╠═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n";

    string line;
    bool found = false;

    while (getline(file, line))
    {

        stringstream ss(line);
        string id, title, author, year, genre;

        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, year, ',');
        getline(ss, genre, ',');

        if (search == id || search == title || search == author ||
            search == year || search == genre)
        {

            cout << "\t║ "
                 << left
                 << setw(15) << id
                 << setw(30) << title
                 << setw(30) << author
                 << setw(15) << year
                 << setw(30) << genre
                 << "║\n";

            found = true;
        }
    }

    if (found)
    {
        cout << "\t╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n";
    }
    else
    {
        cout << "\t║                                            No matching record found                                                     ║\n";
        cout << "\t╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n";
        // fail();
    }

    cout << "\033[0m";
    file.close();

    cout << "\n\n";
    string choice;
    do
    {

        cout << "\033[38;2;0;255;0m\t╔═══[1]     Go Back\033[0m" << endl;
        cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
        cout << "\033[38;2;0;255;135m\t╠══════[2]    Exit\033[0m" << endl;
        cout << "\033[38;2;0;255;175m\t║\033[0m" << endl;
        cout << "\033[38;2;0;255;175m\t╚═══>     ";
        cin >> choice;
        if (choice == "1")
        {
            bookMenu();
        }
        else if (choice == "2")
        {
            return;
        }

    } while (choice != "1" && choice != "2");
}
void updateBookLogo()
{

    system("cls");
    cout << "\n\n";

    cout << "\033[38;2;0;255;0m\t   __  __          __      __          ____              __  \033[0m" << endl;
    cout << "\033[38;2;0;255;95m\t  / / / /___  ____/ /___ _/ /____     / __ )____  ____  / /__\033[0m" << endl;
    cout << "\033[38;2;0;255;135m\t / / / / __ \\/ __  / __ `/ __/ _ \\   / __  / __ \\/ __ \\/ //_/\033[0m" << endl;
    cout << "\033[38;2;0;255;135m\t/ /_/ / /_/ / /_/ / /_/ / /_/  __/  / /_/ / /_/ / /_/ / ,<   \033[0m" << endl;
    cout << "\033[38;2;0;255;175m\t\\____/ .___/\\__,_/\\__,_/\\__/\\___/  /_____/\\____/\\____/_/|_|  \033[0m" << endl;
    cout << "\033[38;2;0;255;175m\t    /_/                                                      \033[0m" << endl;

    cout << "\n\n";
}
void updateBook()
{

    updateBookLogo();

    string searchId;
    cout << "\033[38;2;0;255;0m\t╔══════   Enter book's ID to update\033[0m" << endl;
    cout << "\033[38;2;0;255;0m\t║\033[0m" << endl;
    cout << "\033[38;2;0;255;0m\t╚═══>   ";
    cin >> searchId;
    cin.ignore();

    string path = string(getenv("USERPROFILE")) + "\\Documents\\books.csv";
    string tempPath = string(getenv("USERPROFILE")) + "\\Documents\\temp.csv";

    ifstream inFile(path);
    ofstream outFile(tempPath);

    cout << "\n\n";

    string line;
    bool updated = false;

    while (getline(inFile, line))
    {
        stringstream ss(line);
        string id, title, author, year, genre;

        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, year, ',');
        getline(ss, genre, ',');

        if (id == searchId)
        {
            // ask new data
            cout << "\033[38;2;0;255;0m\t╔══════   Current Information...\033[0m" << endl;
            cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;95m\t╠═══book's ID    : " << id << endl;
            cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;95m\t╠═══book's Title : " << title << endl;
            cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;135m\t╠═══book's Author: " << author << endl;
            cout << "\033[38;2;0;255;135m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;135m\t╠═══book's Year  : " << year << endl;
            cout << "\033[38;2;0;255;175m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;175m\t╚═══book's Genre : " << genre << endl;

            cout << "\n\n";

            string input;

            cout << "\033[38;2;0;255;0m\t╔══════   Updating...\033[0m" << endl;
            cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;95m\t╠═══Enter book's Title : ";
            getline(cin, input);
            if (!input.empty())
                title = input;
            cout << "\033[0m";
            cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;135m\t╠═══Enter book's Author: ";
            getline(cin, input);
            if (!input.empty())
                author = input;
            cout << "\033[0m";
            cout << "\033[38;2;0;255;135m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;135m\t╠═══Enter book's Year  : ";
            getline(cin, input);
            if (!input.empty())
                year = input;
            cout << "\033[0m";
            cout << "\033[38;2;0;255;175m\t║\033[0m" << endl;
            cout << "\033[38;2;0;255;175m\t╚═══Enter book's Genre : ";
            getline(cin, input);
            if (!input.empty())
                genre = input;
            cout << "\033[0m";

            outFile << id << "," << title << "," << author << ","
                    << year << "," << genre << '\n';

            updated = true;
        }
        else
        {
            outFile << line << '\n';
        }
    }

    inFile.close();
    outFile.close();

    if (updated)
    {
        remove(path.c_str());
        rename(tempPath.c_str(), path.c_str());
        success();
    }
    else
    {
        remove(tempPath.c_str());
        fail();
    }

    cout << "\n\n";
    string choice;
    do
    {

        cout << "\033[38;2;0;255;0m\t╔═══[1]     Go Back\033[0m" << endl;
        cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
        cout << "\033[38;2;0;255;135m\t╠══════[2]    Exit\033[0m" << endl;
        cout << "\033[38;2;0;255;175m\t║\033[0m" << endl;
        cout << "\033[38;2;0;255;175m\t╚═══>     ";
        cin >> choice;
        if (choice == "1")
        {
            bookMenu();
        }
        else if (choice == "2")
        {
            return;
        }

    } while (choice != "1" && choice != "2");
}
void deleteBookLogo()
{

    system("cls");
    cout << "\n\n";

    cout << "\033[38;2;0;255;0m\t    ____       __     __          ____              __  \033[0m" << endl;
    cout << "\033[38;2;0;255;95m\t   / __ \\___  / /__  / /____     / __ )____  ____  / /__\033[0m" << endl;
    cout << "\033[38;2;0;255;135m\t  / / / / _ \\/ / _ \\/ __/ _ \\   / __  / __ \\/ __ \\/ //_/\033[0m" << endl;
    cout << "\033[38;2;0;255;175m\t / /_/ /  __/ /  __/ /_/  __/  / /_/ / /_/ / /_/ / ,<   \033[0m" << endl;
    cout << "\033[38;2;0;255;175m\t/_____/\\___/_/\\___/\\__/\\___/  /_____/\\____/\\____/_/|_|  \033[0m" << endl;

    cout << "\n\n";
}
void deleteBook()
{

    deleteBookLogo();

    string searchId;
    cout << "\033[38;2;0;255;0m\t╔══════   Enter book's ID to delete\033[0m\n";
    cout << "\033[38;2;0;255;0m\t║\033[0m\n";
    cout << "\033[38;2;0;255;0m\t╚═══>   ";
    cin >> searchId;
    cin.ignore();

    string path = string(getenv("USERPROFILE")) + "\\Documents\\books.csv";
    string tempPath = string(getenv("USERPROFILE")) + "\\Documents\\temp.csv";

    ifstream inFile(path);
    ofstream outFile(tempPath);

    string line;
    bool deleted = false;

    while (getline(inFile, line))
    {
        string id;
        size_t pos = line.find(',');
        if (pos != string::npos)
            id = line.substr(0, pos);
        else
            id = line;

        if (id == searchId)
        {
            deleted = true; // skip this line
            continue;
        }
        outFile << line << '\n';
    }

    inFile.close();
    outFile.close();

    if (deleted)
    {
        remove(path.c_str());
        rename(tempPath.c_str(), path.c_str());
        success();
    }
    else
    {
        remove(tempPath.c_str());
        fail();
    }

    cout << "\n\n";
    string choice;
    do
    {

        cout << "\033[38;2;0;255;0m\t╔═══[1]     Go Back\033[0m" << endl;
        cout << "\033[38;2;0;255;95m\t║\033[0m" << endl;
        cout << "\033[38;2;0;255;135m\t╠══════[2]    Exit\033[0m" << endl;
        cout << "\033[38;2;0;255;175m\t║\033[0m" << endl;
        cout << "\033[38;2;0;255;175m\t╚═══>     ";
        cin >> choice;
        if (choice == "1")
        {
            bookMenu();
        }
        else if (choice == "2")
        {
            return;
        }

    } while (choice != "1" && choice != "2");
}
/*====================[^^^Book Manegement^^^]==========================*/
/*=====================================================================*/

#endif
