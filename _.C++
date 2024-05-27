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
        std::cout << "Product: " << name << ", Price: " << price << std::endl;
    }

    const std::string& getName() const {
        return name;
    }

private:
    std::string name;
    double price;
};

// Derived classes for specific product types
class Electronics : public Product {
public:
    Electronics(const std::string& name, double price, int warrantyPeriod)
        : Product(name, price), warrantyPeriod(warrantyPeriod) {}

    double calculatePrice() const override {
        return getPrice() + (0.1 * warrantyPeriod);
    }

    void displayInfo() const override {
        std::cout << "Electronics: " << getName() << ", Price: " << calculatePrice() << std::endl;
    }

private:
    int warrantyPeriod;
};

class Clothing : public Product {
public:
    Clothing(const std::string& name, double price, const std::string& size)
        : Product(name, price), size(size) {}

    double calculatePrice() const override {
        // Custom pricing logic for clothing
        return getPrice() + 5.0; // Additional cost for clothing
    }

    void displayInfo() const override {
        std::cout << "Clothing: " << getName() << ", Price: " << calculatePrice() << std::endl;
    }

private:
    std::string size;
};

class Groceries : public Product {
public:
    Groceries(const std::string& name, double price, const std::string& expirationDate)
        : Product(name, price), expirationDate(expirationDate) {}

    // Groceries have a discounted price if close to expiration
    double calculatePrice() const override {
        // Custom pricing logic for groceries
        return (getPrice() * 0.9); // 10% discount for groceries
    }

    void displayInfo() const override {
        std::cout << "Groceries: " << getName() << ", Price: " << calculatePrice() << std::endl;
    }

private:
    std::string expirationDate;
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

private:
    Customer customer;
    std::vector<Product> products;
};

// File handling functions

// Save product information to a file
void saveProductToFile(const Product& product) {
    std::ofstream file("products.txt", std::ios::app);
    file << product.getName() << " " << product.calculatePrice() << std::endl;
    // Add other necessary data to save
    file.close();
}

// Load product information from a file and create a Product object
Product loadProductFromFile(const std::string& productName) {
    std::ifstream file("products.txt");
    std::string name;
    double price;
    // Read data from file and create a Product object
    while (file >> name >> price) {
        if (name == productName) {
            file.close();
            // Return the loaded product
            if (name == "Electronics") {
                int warrantyPeriod;
                file >> warrantyPeriod;
                return Electronics(name, price, warrantyPeriod);
            } else if (name == "Clothing") {
                std::string size;
                file >> size;
                return Clothing(name, price, size);
            } else if (name == "Groceries") {
                std::string expirationDate;
                file >> expirationDate;
                return Groceries(name, price, expirationDate);
            } else {
                // Handle unknown product type
                throw std::runtime_error("Unknown product type");
            }
        }
    }
    file.close();
    // Return a default product if not found
    return Product("DefaultProduct", 0.0);
}

// Save customer information to a file
void saveCustomerToFile(const Customer& customer) {
    std::ofstream file("customers.txt", std::ios::app);
    file << customer.getName() << std::endl;
    // Add other necessary data to save
    file.close();
}

// Load customer information from a file and create a Customer object
Customer loadCustomerFromFile(const std::string& customerName) {
    std::ifstream file("customers.txt");
    std::string name;
    // Read data from file and create a Customer object
    while (file >> name) {
        if (name == customerName) {
            file.close();
            // Return the loaded customer
            return Customer(name);
        }
    }
    file.close();
    // Return a default customer if not found
    return Customer("DefaultCustomer");
}

// User Interface functions

// Display the main menu options
void displayMenu() {
    std::cout << "1. Create Invoice\n2. Manage Customers\n3. Handle Products\n4. Exit\n";
}

// Function to create a new invoice
Invoice createInvoice() {
    std::string customerName;
    std::cout << "Enter customer name: ";
    std::cin >> customerName;

    // Load or create a new customer
    Customer customer = loadCustomerFromFile(customerName);
    if (customer.getName() == "DefaultCustomer") {
        saveCustomerToFile(customer);
    }

    Invoice invoice(customer);
    std::cout << "Invoice created for customer: " << customer.getName() << std::endl;

    return invoice;
}

// Function to add a product to an invoice
void addProductToInvoice(Invoice& invoice) {
    std::string productName;
    std::cout << "Enter product name: ";
    std::cin >> productName;

    // Load or create a new product
    Product product = loadProductFromFile(productName);
    if (product.getName() == "DefaultProduct") {
        saveProductToFile(product);
    }

    // Add the product to the invoice
    invoice.addProduct(product);
    std::cout << "Product added to the invoice: " << product.getName() << std::endl;
}

// Function to display the invoice details
void displayInvoice(const Invoice& invoice) {
    std::cout << "Invoice Details\n";
    std::cout << "Customer: " << invoice.getCustomer().getName() << std::endl;
    std::cout << "Products:\n
