#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

class Book {
private:
    std::string title;
    std::string author;
    int pages;
    std::string id;
    bool isBorrowed;

public:
    Book(std::string t, std::string a, int p, std::string i, bool b)
        : title(std::move(t)), author(std::move(a)), pages(p), id(std::move(i)), isBorrowed(b) {}

    const std::string& getId() const { return id; }
    const std::string& getTitle() const { return title; }
    const std::string& getAuthor() const { return author; }
    int getPages() const { return pages; }
    bool getBorrowedStatus() const { return isBorrowed; }

    void borrowBook() { isBorrowed = true; }
    void returnBook() { isBorrowed = false; }

    void displayDetails() const {
        std::cout << "\n----------------------------------------\n";
        std::cout << "ID:       " << id << "\n";
        std::cout << "Title:    " << title << "\n";
        std::cout << "Author:   " << author << "\n";
        std::cout << "Pages:    " << pages << "\n";
        std::cout << "Status:   " << (isBorrowed ? "Borrowed" : "Available") << "\n";
        std::cout << "----------------------------------------\n";
    }
};

std::vector<Book> loadBooks(const std::string& filename) {
    std::vector<Book> books;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << "\n";
        return books;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string title, author, pagesStr, id, statusStr;

        if (std::getline(ss, title, ',') &&
            std::getline(ss, author, ',') &&
            std::getline(ss, pagesStr, ',') &&
            std::getline(ss, id, ',') &&
            std::getline(ss, statusStr)) {
            
            // Clean carriage returns if present
            if (!statusStr.empty() && statusStr.back() == '\r') {
                statusStr.pop_back();
            }

            int pages = std::stoi(pagesStr);
            bool borrowed = (statusStr == "true");
            books.emplace_back(title, author, pages, id, borrowed);
        }
    }
    return books;
}

void displayAllBooks(const std::vector<Book>& books) {
    std::cout << "\n" << std::left 
              << std::setw(6)  << "ID" 
              << std::setw(28) << "Title" 
              << std::setw(22) << "Author" 
              << std::setw(8)  << "Pages" 
              << std::setw(12) << "Status" << "\n";
    std::cout << std::string(76, '-') << "\n";

    for (const auto& book : books) {
        std::cout << std::left 
                  << std::setw(6)  << book.getId()
                  << std::setw(28) << (book.getTitle().length() > 25 ? book.getTitle().substr(0, 24) + "…" : book.getTitle())
                  << std::setw(22) << (book.getAuthor().length() > 20 ? book.getAuthor().substr(0, 19) + "…" : book.getAuthor())
                  << std::setw(8)  << book.getPages()
                  << std::setw(12) << (book.getBorrowedStatus() ? "Borrowed" : "Available") << "\n";
    }
}

Book* findBookById(std::vector<Book>& books, const std::string& id) {
    for (auto& book : books) {
        if (book.getId() == id) return &book;
    }
    return nullptr;
}

int main() {
    std::vector<Book> library = loadBooks("bookData.txt");
    if (library.empty()) {
        std::cerr << "Library database could not be loaded.\n";
        return 1;
    }

    int choice = 0;
    while (true) {
        std::cout << "\n========== Library Management System ==========\n";
        std::cout << "1. View All Books\n";
        std::cout << "2. View Book Details by ID\n";
        std::cout << "3. Borrow a Book\n";
        std::cout << "4. Return a Book\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice (1-5): ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid selection. Please enter a valid number.\n";
            continue;
        }

        if (choice == 5) {
            std::cout << "Exiting system. Goodbye!\n";
            break;
        }

        std::string searchId;
        switch (choice) {
            case 1:
                displayAllBooks(library);
                break;
            case 2: {
                std::cout << "Enter Book ID (e.g., 001): ";
                std::cin >> searchId;
                Book* book = findBookById(library, searchId);
                if (book) {
                    book->displayDetails();
                } else {
                    std::cout << "Book with ID " << searchId << " not found.\n";
                }
                break;
            }
            case 3: {
                std::cout << "Enter Book ID to borrow: ";
                std::cin >> searchId;
                Book* book = findBookById(library, searchId);
                if (!book) {
                    std::cout << "Book with ID " << searchId << " not found.\n";
                } else if (book->getBorrowedStatus()) {
                    std::cout << "Notice: '" << book->getTitle() << "' is already borrowed.\n";
                } else {
                    book->borrowBook();
                    std::cout << "Success: You borrowed '" << book->getTitle() << "'.\n";
                }
                break;
            }
            case 4: {
                std::cout << "Enter Book ID to return: ";
                std::cin >> searchId;
                Book* book = findBookById(library, searchId);
                if (!book) {
                    std::cout << "Book with ID " << searchId << " not found.\n";
                } else if (!book->getBorrowedStatus()) {
                    std::cout << "Notice: '" << book->getTitle() << "' is not currently borrowed.\n";
                } else {
                    book->returnBook();
                    std::cout << "Success: You returned '" << book->getTitle() << "'.\n";
                }
                break;
            }
            default:
                std::cout << "Invalid choice. Please select an option between 1 and 5.\n";
                break;
        }
    }

    return 0;
}