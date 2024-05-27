#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Base class representing a product
class Product {
public:
    Product(const std::string& name, double price) : name(name), price(price) {}

    virtual double calculatePrice() const {
        return price;
    }

    virtual void displayInfo() const {
        std::cout << "Product: " << name << ", Price: " << calculatePrice() << std::endl;
    }

    const std::string& getName() const {
        return name;
    }

private:
    std::string name;
    double price;
};

// Derived class for Electronics product type
class Electronics : public Product {
public:
    Electronics(const std::string& name, double price, int warrantyPeriod)
        : Product(name, price), warrantyPeriod(warrantyPeriod) {}

    double calculatePrice() const override {
        return Product::calculatePrice() + (0.1 * warrantyPeriod);
    }

    void displayInfo() const override {
        std::cout << "Electronics: " << getName() << ", Price: " << calculatePrice() << std::endl;
    }

private:
    int warrantyPeriod;
};

// Derived class for Clothing product type
class Clothing : public Product {
public:
    Clothing(const std::string& name, double price, const std::string& size)
        : Product(name, price), size(size) {}

    double calculatePrice() const override {
        return Product::calculatePrice() + 5.0;
    }

    void displayInfo() const override {
        std::cout << "Clothing: " << getName() << ", Price: " << calculatePrice() << std::endl;
    }

private:
    std::string size;
};

// Class representing a customer
class Customer {
public:
    Customer(const std::string& name) : name(name) {}

    const std::string& getName() const {
        return name;
    }

private:
    std::string name;
};

// Class representing an invoice
class Invoice {
public:
    Invoice(const Customer& customer) : customer(customer) {}

    // Add a product to the invoice
    void addProduct(const Product& product) {
        products.push_back(product);
    }

    // Calculate the total price of all products in the invoice
    double calculateTotal() const {
        double total = 0;
        for (const auto& product : products) {
            total += product.calculatePrice();
        }
        return total;
    }

    const Customer& getCustomer() const {
        return customer;
    }

    const std::vector<Product>& getProducts() const {
        return products;
    }

private:
    Customer customer;
    std::vector<Product> products;
};

// Save data to a file
void saveToFile(const std::string& fileName, const std::string& data) {
    std::ofstream file(fileName, std::ios::app);
    if (file.is_open()) {
        file << data << std::endl;
        file.close();
    } else {
        std::cerr << "Unable to open " << fileName << " for writing." << std::endl;
    }
}

// Load data from a file
std::string loadFromFile(const std::string& fileName, const std::string& targetName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Unable to open " << fileName << " for reading." << std::endl;
        return "Default" + targetName;
    }

    std::string name;
    std::string data;

    while (file >> name) {
        if (name == targetName) {
            file.close();
            return name;
        }
    }

    file.close();
    return "Default" + targetName;
}

// Display menu options
void displayMenu() {
    std::cout << "1. Create Invoice\n2. Exit\n";
}

// Create a new invoice
Invoice createInvoice() {
    std::string customerName;
    std::cout << "Enter customer name: ";
    std::cin >> customerName;

    Customer customer(customerName);
    saveToFile("customers.txt", customer.getName());

    Invoice invoice(customer);
    std::cout << "Invoice created for customer: " << customer.getName() << std::endl;

    return invoice;
}

// Add a product to an invoice
void addProductToInvoice(Invoice& invoice) {
    std::string productName;
    std::cout << "Enter product name (Electronics/Clothing): ";
    std::cin >> productName;

    double price;
    std::cout << "Enter product price: ";
    std::cin >> price;

    if (productName == "Electronics") {
        int warrantyPeriod;
        std::cout << "Enter warranty period: ";
        std::cin >> warrantyPeriod;
        Electronics electronics("Electronics", price, warrantyPeriod);
        invoice.addProduct(electronics);
    } else if (productName == "Clothing") {
        std::string size;
        std::cout << "Enter clothing size: ";
        std::cin >> size;
        Clothing clothing("Clothing", price, size);
        invoice.addProduct(clothing);
    } else {
        std::cerr << "Invalid product type." << std::endl;
    }
}

// Display invoice details
void displayInvoice(const Invoice& invoice) {
    std::cout << "Invoice Details\n";
    std::cout << "Customer: " << invoice.getCustomer().getName() << std::endl;
    std::cout << "Products:\n";

    for (const auto& product : invoice.getProducts()) {
        product.displayInfo();
    }

    std::cout << "Total: " << invoice.calculateTotal() << std::endl;
}

// Main function
int main() {
    int choice;
    Invoice currentInvoice(Customer("DefaultCustomer"));

    do {
        displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                currentInvoice = createInvoice();
                addProductToInvoice(currentInvoice);
                displayInvoice(currentInvoice);
                break;
            case 2:
                std::cout << "Exiting program." << std::endl;
                break;
            default:
                std::cerr << "Invalid choice. Try again." << std::endl;
        }
    } while (choice != 2);

    return 0;
}
