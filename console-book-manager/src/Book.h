#ifndef BOOKMANAGEMENT_H
#define BOOKMANAGEMENT_H

#include <iostream>
using namespace std;
class Book
{

    // data members
private:
    string id, title, author, year, genre;

    // method
public:
    // default constructor
    Book() : id(""), title(""), author(""), year(""), genre("") {}

    // constructor with parameter
    Book(string id, string title, string author, string year, string genre)
        : id(id), title(title), author(author), year(year), genre(genre) {}

    // getter method
    string getId()
    {
        return this->id;
    }
    string getTitle()
    {
        return this->title;
    }
    string getAuthor()
    {
        return this->author;
    }
    string getYear()
    {
        return this->year;
    }
    string getGenre()
    {
        return this->genre;
    }

    string toString()
    {
        return id + "," + title + "," + author + "," + year + "," + genre + "\n";
    }
};

#endif