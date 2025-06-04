#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <ctime>
#include <map>
#include <algorithm>
using namespace std;

// Menu Item Class
class MenuItem {
public:
    int id;
    string name;
    double price;

    MenuItem() {}
    MenuItem(int i, string n, double p) : id(i), name(n), price(p) {}
};

// Order Item Class
class OrderItem {
public:
    MenuItem item;
    int quantity;

    OrderItem(MenuItem i, int q) : item(i), quantity(q) {}

    double getTotal() {
        return quantity * item.price;
    }
};

// Order Class
class Order {
public:
    int orderId;
    string customerName;
    int tableNumber;
    vector<OrderItem> items;
    time_t orderTime;

    Order(int id, string name, int tableNo) : orderId(id), customerName(name), tableNumber(tableNo) {
        orderTime = time(0);
    }

    void addItem(OrderItem item) {
        items.push_back(item);
    }

    double getTotalBill() {
        double total = 0;
        for (auto& item : items)
            total += item.getTotal();
        return total;
    }

    void printBill() {
        cout << "\n-------- BILL --------\n";
        cout << "Order ID: " << orderId << "\nCustomer: " << customerName << "\nTable: " << tableNumber << endl;
        cout << "Date/Time: " << ctime(&orderTime);
        cout << "----------------------\n";
        for (auto& item : items) {
            cout << left << setw(20) << item.item.name
                 << " x" << item.quantity
                 << " = Rs. " << fixed << setprecision(2) << item.getTotal() << endl;
        }
        cout << "----------------------\nTotal: Rs. " << fixed << setprecision(2) << getTotalBill() << endl;
        cout << "----------------------\n";
    }

    void saveToFile() {
        ofstream file("orders.txt", ios::app);
        file << orderId << "|" << customerName << "|" << tableNumber << "|" << ctime(&orderTime);
        for (auto& item : items)
            file << item.item.id << "," << item.item.name << "," << item.quantity << "," << item.getTotal() << ";";
        file << "\n";
        file.close();
    }
};

// Restaurant Class
class Restaurant {
private:
    vector<MenuItem> menu;
    int nextOrderId;
    map<int, bool> tableStatus;

public:
    Restaurant() {
        nextOrderId = 1000;
        loadMenu();
        for (int i = 1; i <= 10; i++) tableStatus[i] = false;
    }

    void loadMenu() {
        menu = {
            MenuItem(1, "Masala Dosa", 40.0),
            MenuItem(2, "Idli Vada", 30.0),
            MenuItem(3, "Paneer Butter Masala", 90.0),
            MenuItem(4, "Chapati", 10.0),
            MenuItem(5, "Fried Rice", 80.0),
            MenuItem(6, "Ice Cream", 50.0),
            MenuItem(7, "Coffee", 25.0),
            MenuItem(8, "Pizza", 150.0),
            MenuItem(9, "Burger", 120.0),
            MenuItem(10, "Noodles", 90.0),
            MenuItem(11, "Gobi Manchurian", 100.0),
            MenuItem(12, "Lassi", 40.0)
        };
    }

    void showMenu() {
        cout << "\n------ MENU ------\n";
        for (auto& item : menu) {
            cout << item.id << ". " << left << setw(25) << item.name
                 << "Rs. " << fixed << setprecision(2) << item.price << endl;
        }
        cout << "------------------\n";
    }

    void searchMenuItem() {
        string keyword;
        cout << "Enter item name to search: ";
        cin.ignore();
        getline(cin, keyword);

        transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
        bool found = false;
        for (auto& item : menu) {
            string nameLower = item.name;
            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            if (nameLower.find(keyword) != string::npos) {
                cout << item.id << ". " << item.name << " - Rs. " << item.price << endl;
                found = true;
            }
        }
        if (!found) cout << "No items matched your search.\n";
    }

    void editMenu() {
        int option;
        cout << "\n--- Menu Edit ---\n1. Add Item\n2. Edit Item Price\n3. Delete Item\nChoose: ";
        cin >> option;

        if (option == 1) {
            int id = menu.size() + 1;
            string name;
            double price;
            cout << "Enter item name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter item price: ";
            cin >> price;
            menu.push_back(MenuItem(id, name, price));
            cout << "Item added!\n";
        } else if (option == 2) {
            int id;
            double newPrice;
            cout << "Enter item ID to edit: ";
            cin >> id;
            for (auto& item : menu) {
                if (item.id == id) {
                    cout << "Current price of " << item.name << ": Rs. " << item.price << endl;
                    cout << "Enter new price: ";
                    cin >> newPrice;
                    item.price = newPrice;
                    cout << "Price updated!\n";
                    return;
                }
            }
            cout << "Item ID not found!\n";
        } else if (option == 3) {
            int id;
            cout << "Enter item ID to delete: ";
            cin >> id;
            menu.erase(remove_if(menu.begin(), menu.end(), [id](MenuItem& i) { return i.id == id; }), menu.end());
            cout << "Item deleted!\n";
        } else {
            cout << "Invalid choice!\n";
        }
    }

    void showAvailableTables() {
        cout << "\nAvailable Tables: ";
        for (auto& pair : tableStatus) {
            if (!pair.second)
                cout << pair.first << " ";
        }
        cout << endl;
    }

    bool bookTable(int tableNo) {
        if (tableStatus.count(tableNo) && !tableStatus[tableNo]) {
            tableStatus[tableNo] = true;
            return true;
        }
        return false;
    }

    void freeTable(int tableNo) {
        if (tableStatus.count(tableNo))
            tableStatus[tableNo] = false;
    }

    void collectFeedback(const string& customerName) {
        string feedback;
        cout << "\nWould you like to leave feedback? (y/n): ";
        char response;
        cin >> response;
        cin.ignore();
        if (response == 'y' || response == 'Y') {
            cout << "Enter your feedback: ";
            getline(cin, feedback);
            ofstream file("feedback.txt", ios::app);
            time_t now = time(0);
            file << "Customer: " << customerName << "\nFeedback: " << feedback << "\nDate/Time: " << ctime(&now) << "-----------------------------\n";
            file.close();
            cout << "Thank you for your feedback!\n";
        }
    }

    void takeOrder() {
        string customer;
        int tableNo;
        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, customer);

        showAvailableTables();
        cout << "Choose a table number: ";
        cin >> tableNo;

        if (!bookTable(tableNo)) {
            cout << "Table not available or invalid!\n";
            return;
        }

        Order order(nextOrderId++, customer, tableNo);
        int choice, qty;
        char more;

        do {
            showMenu();
            cout << "Enter item ID: ";
            cin >> choice;
            cout << "Enter quantity: ";
            cin >> qty;

            bool found = false;
            for (auto& item : menu) {
                if (item.id == choice) {
                    order.addItem(OrderItem(item, qty));
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Invalid item ID!\n";

            cout << "Add more items? (y/n): ";
            cin >> more;
        } while (more == 'y' || more == 'Y');

        order.printBill();
        order.saveToFile();
        freeTable(tableNo);
        cout << "Order placed successfully!\n";

        collectFeedback(customer);
    }

    void showOrderHistory() {
        ifstream file("orders.txt");
        string line;
        cout << "\n---- Order History ----\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    void showFeedback() {
        ifstream file("feedback.txt");
        string line;
        cout << "\n---- Customer Feedback ----\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    void clearFeedback() {
        ofstream file("feedback.txt");
        file.close();
        cout << "Feedback cleared!\n";
    }

    void printSummaryReport() {
        ifstream file("orders.txt");
        string line;
        int totalOrders = 0;
        double totalRevenue = 0;
        while (getline(file, line)) {
            size_t pos = line.find_last_of(';');
            while (pos != string::npos) {
                size_t pricePos = line.find_last_of(',', pos - 1);
                if (pricePos != string::npos) {
                    double price = stod(line.substr(pricePos + 1, pos - pricePos - 1));
                    totalRevenue += price;
                }
                pos = line.find_last_of(';', pos - 1);
            }
            totalOrders++;
        }
        cout << "\n--- Summary Report ---\nTotal Orders: " << totalOrders << "\nTotal Revenue: Rs. " << totalRevenue << "\n----------------------\n";
        file.close();
    }

    void run() {
        int choice;
        do {
            cout << "\n==== RESTAURANT SYSTEM ====\n";
            cout << "1. Show Menu\n";
            cout << "2. Take Order\n";
            cout << "3. Show Order History\n";
            cout << "4. Show Feedback\n";
            cout << "5. Edit Menu\n";
            cout << "6. Search Menu Item\n";
            cout << "7. Summary Report\n";
            cout << "8. Clear Feedback\n";
            cout << "9. Exit\n";
            cout << "Choose an option: ";
            cin >> choice;

            switch (choice) {
                case 1: showMenu(); break;
                case 2: takeOrder(); break;
                case 3: showOrderHistory(); break;
                case 4: showFeedback(); break;
                case 5: editMenu(); break;
                case 6: searchMenuItem(); break;
                case 7: printSummaryReport(); break;
                case 8: clearFeedback(); break;
                case 9: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice!\n";
            }
        } while (choice != 9);
    }
};

int main() {
    Restaurant r;
    r.run();
    return 0;
}

