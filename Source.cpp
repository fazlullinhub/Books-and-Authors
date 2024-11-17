#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

class BookAPI {
public:
    BookAPI() {}

    void getBookByTitle(const std::string& title) {
        std::string url = baseUrl + "bibkeys=OLID:" + title + "&format=json&jscmd=data";
        auto response = cpr::Get(cpr::Url{ url });
        handleResponse(response);
    }

    void getBookById(const std::string& id) {
        std::string url = baseUrl + "bibkeys=OLID:" + id + "&format=json&jscmd=data";
        auto response = cpr::Get(cpr::Url{ url });
        handleResponse(response);
    }

private:
    const std::string baseUrl = "https://openlibrary.org/api/books?";

    void handleResponse(const cpr::Response& response) {
        if (response.status_code != 200) {
            std::cerr << "Error fetching book information: " << response.error.message << std::endl;
            return;
        }

        auto json_response = nlohmann::json::parse(response.text);

        if (json_response.empty()) {
            std::cout << "No information found for the specified title or ID." << std::endl;
            return;
        }

        for (auto& item : json_response.items()) {
            std::cout << "Key: " << item.key() << std::endl; 
            auto bookInfo = item.value();
            std::cout << "Title: " << bookInfo["title"] << std::endl;

            if (bookInfo.contains("authors")) {
                std::cout << "Authors: ";
                for (const auto& author : bookInfo["authors"]) {
                    std::cout << author["name"] << ", ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl; 
        }
    }
};

int main() {
    BookAPI api;
    std::string input;
    int choice;

    while (true) {
        std::cout << "Welcome to the Book Info App!" << std::endl;
        std::cout << "Choose an option:" << std::endl;
        std::cout << "1. Get book information by title" << std::endl;
        std::cout << "2. Get book information by unique ID" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Your choice: ";
        std::cin >> choice;

        if (choice == 3) {
            break;
        }

        std::cout << "Enter the title or ID: ";
        std::cin.ignore();
        std::getline(std::cin, input);

        if (choice == 1) {
            api.getBookByTitle(input);
        }
        else if (choice == 2) {
            api.getBookById(input);
        }
        else {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    }

    return 0;
}