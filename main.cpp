#include <iostream>
#include <map>
#include <string>
#include <exception>

// <key: article, value: number of pieces>
#define product std::string,int
// defining the conditions
#define isYes ((answer == "y") || (answer == "Y"))
#define isNo ((answer == "n") || (answer == "N"))

// types of commands
enum typeCommand {
    NONE,  // The operation is not defined
    ADD,  // the operation of adding an item to the shopping cart
    REMOVE,  // the operation of removing an item from the shopping cart
    LIST,  // output of product lists in the store's database and shopping cart
    END  // completion of work
};

// the store's database
std::map<product> data_base;
// shopping cart
std::map<product> shopping_cart;
/////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 @brief A subroutine for entering product data.
 @param [in]inCommand the code of the operation to be performed, for which certain validation needs to be performed.

Note:

use the code "NONE" so that this function does not apply a correctness check to the entered data.
 @param [out]outArticle the article of the product.
 @param [out]outCount the quantity of the product.
*/
void enter_data(const typeCommand inCommand, std::string & outArticle, int & outCount)
{
    // iterators
    std::map<product>::iterator data_base_it, shopping_cart_it;

    // entering the item number
    std::cout << "Enter the article: ";
    std::cin >> outArticle;
    
    // article validation
    switch(inCommand) {
        case NONE: {
            break;
        }
        case ADD: {
            data_base_it = data_base.find(outArticle);
            if(data_base_it == data_base.end())
                throw std::invalid_argument("There is no such article in the store's database.");
            break;
        }
        case REMOVE: {
            shopping_cart_it = shopping_cart.find(outArticle);
            if(shopping_cart_it == shopping_cart.end())
                throw std::invalid_argument("There is no such article in the shopping cart.");
            break;
        }
        default: {
            std::runtime_error("The command is not defined.");
        }
    }

    // entering the quantity of the product
    std::cout << "Enter the quantity of the product: ";
    std::cin >> outCount;

    // validation of the quantity
    switch(inCommand) {
        case NONE: {
            break;
        }
        case ADD: {
            if(outCount > data_base_it->second)
                throw std::invalid_argument("There is no product in the store in such quantity.");
            break;
        }
        case REMOVE: {
            if(outCount > shopping_cart_it->second)
                throw std::invalid_argument("There is no product in this quantity in the shopping cart.");
            break;
        }
    }
}

/*!
 @brief A subroutine for displaying list items with a title.
 @param inCaption the title of the list of list items.
 @param inList a list of items to output.
*/
void list_print(const std::string & inCaption, std::map<product> & inList)
{
    std::cout << "\n--- " << inCaption << " list: ---\n";
    if(inList.empty())
        std::cout << "No elements.\n";
    else
        for(std::map<product>::iterator it = inList.begin(); it != inList.end(); it++)
            std::cout << it->first << " - " << it->second << std::endl;
    std::cout << "--- End of " << inCaption << ". ---\n";
}

int main()
{
    //---------------------------------------------------------------------
    // filling in the store's database

    data_base.clear();
    while(true) {
        std::string article, answer;
        int count;

        // input
        do {
            std::cout << "Data entry? (Yes(y), No(n)): ";
            std::cin >> answer;
        } while(!isYes && !isNo);

        // completing the data entry in case of a negative response
        if(isNo) break;

        // entering product data
        enter_data(NONE, article, count);

        // adding data to the store's database
        data_base.insert(std::pair<product>(article, count));
    }


    //---------------------------------------------------------------------
    // implementation of the shopping cart operation

    shopping_cart.clear();
    while(true) {
        std::string command, article;
        int count;

        // entering a command
        std::cout << "\nEnter the command (add, remove, lists, end): ";
        std::cin >> command;

        if(command == "add") {
            // trying to enter the data
            try
            {
                enter_data(ADD, article, count);
            }
            catch(const std::invalid_argument & ex)
            {
                std::cout << "Error. " << ex.what() << std::endl;
                continue;
            }
            catch(const std::runtime_error & ex)
            {
                std::cout << "Error. " << ex.what() << std::endl;
                continue;
            }
            
            // changing data in two lists

            // 1) changing data in the store's database
            data_base.find(article)->second -= count;

            // 2) changing the data in the shopping cart list
            // if such an item is in the cart
            std::map<product>::iterator it = shopping_cart.find(article);
            if(it != shopping_cart.end()) {
                // then change the quantity
                it->second += count;
            }
            else
            {
                // otherwise, add a new product
                shopping_cart.insert(std::pair<product>(article, count));
            }

        }
        else if(command == "remove") {
            // trying to enter the data
            try
            {
                enter_data(REMOVE, article, count);
            }
            catch(const std::invalid_argument & ex)
            {
                std::cout << "Error. " << ex.what() << std::endl;
                continue;
            }
            catch(const std::runtime_error & ex)
            {
                std::cout << "Error. " << ex.what() << std::endl;
                continue;
            }
            
            // changing data in two lists

            // 1) changing data in the store's database
            data_base.find(article)->second += count;
            
            // 2) changing the data in the shopping cart list
            std::map<product>::iterator it = shopping_cart.find(article);
            it->second -= count;
            // if the quantity of the product is zero in the cart
            if(it->second == 0) {
                // then delete the information about it
                shopping_cart.erase(it);
            }
        }
        else if(command == "lists") {
            // output the contents of the store's database and shopping cart
            list_print("Data_base", data_base);
            list_print("Shopping_cart", shopping_cart);
        }
        else if(command == "end") {
            break;
        }
        else
        {
            std::cout << "The command is not defined.\n";
        }
    }

    //---------------------------------------------------------------------
    // output of the program shutdown message
    std::cout
        << "----------------------------------------------------------\n"
        << "End of program.\n";
    return 0;
}
