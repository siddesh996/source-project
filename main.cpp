// RestaurantSystem.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <ctime>
#include <map>
#include <algorithm>
#include <limits>
using namespace std;

// MenuItem Class
class MenuItem {
public:
    int id;
    string name;
    double price;
    bool available;

    MenuItem() {}
    MenuItem(int i, string n, double p, bool a = true)
        : id(i), name(n), price(p), available(a) {}
};

// OrderItem Class
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

    Order(int id, string name, int tableNo)
        : orderId(id), customerName(name), tableNumber(tableNo) {
        orderTime = time(0);
    }

    void addItem(OrderItem item) {
        items.push_back(item);
    }

    double getSubTotal() {
        double total = 0;
        for (auto& item : items) total += item.getTotal();
        return total;
    }

    double getGST() {
        return getSubTotal() * 0.05; // 5% GST
    }

    double getServiceCharge() {
        return getSubTotal() * 0.03; // 3% Service charge
    }

    double getTotalBill() {
        return getSubTotal() + getGST() + getServiceCharge();
    }

    void printBill() {
        cout << "\n-------- BILL --------\n";
        cout << "Order ID: " << orderId << "\nCustomer: " << customerName
             << "\nTable: " << tableNumber << "\nDate/Time: " << ctime(&orderTime);
        cout << "----------------------\n";
        for (auto& item : items) {
            cout << left << setw(20) << item.item.name
                 << " x" << item.quantity
                 << " = Rs. " << fixed << setprecision(2) << item.getTotal() << endl;
        }
        cout << "Subtotal: Rs. " << getSubTotal() << "\n";
        cout << "GST (5%): Rs. " << getGST() << "\n";
        cout << "Service (3%): Rs. " << getServiceCharge() << "\n";
        cout << "----------------------\nTotal: Rs. " << getTotalBill() << "\n----------------------\n";
    }

    void saveToFile() {
        ofstream file("orders.txt", ios::app);
        file << orderId << "|" << customerName << "|" << tableNumber << "|" << ctime(&orderTime);
        for (auto& item : items) {
            file << item.item.id << "," << item.item.name << "," << item.quantity << "," << item.getTotal() << ";";
        }
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
    const string adminPassword = "admin123";

public:
    Restaurant() {
        nextOrderId = 1000;
        loadMenu();
        for (int i = 1; i <= 10; i++) tableStatus[i] = false;
    }

    void loadMenu() {
        menu = {
            MenuItem(1, "Masala Dosa", 40),
            MenuItem(2, "Idli Vada", 30),
            MenuItem(3, "Paneer Butter Masala", 90),
            MenuItem(4, "Chapati", 10),
            MenuItem(5, "Fried Rice", 80),
            MenuItem(6, "Ice Cream", 50),
            MenuItem(7, "Coffee", 25),
            MenuItem(8, "Pizza", 150),
            MenuItem(9, "Burger", 120),
            MenuItem(10, "Noodles", 90),
            MenuItem(11, "Gobi Manchurian", 100),
            MenuItem(12, "Lassi", 40)
        };
    }

    void showMenu() {
        cout << "\n------ MENU ------\n";
        for (auto& item : menu) {
            if (item.available)
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
            string lower = item.name;
            transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            if (lower.find(keyword) != string::npos) {
                cout << item.id << ". " << item.name << " - Rs. " << item.price << (item.available ? "" : " [Unavailable]") << endl;
                found = true;
            }
        }

        if (!found) cout << "No matching item found.\n";
    }

    void editMenu() {
        if (!authenticateAdmin()) return;

        int option;
        cout << "\n--- Menu Edit ---\n1. Add Item\n2. Edit Item Price\n3. Delete Item\n4. Toggle Availability\nChoose: ";
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
        } else if (option >= 2 && option <= 4) {
            int id;
            cout << "Enter item ID: ";
            cin >> id;

            for (auto& item : menu) {
                if (item.id == id) {
                    if (option == 2) {
                        cout << "Enter new price: ";
                        cin >> item.price;
                        cout << "Price updated!\n";
                    } else if (option == 3) {
                        item.available = false;
                        cout << "Item marked unavailable!\n";
                    } else if (option == 4) {
                        item.available = !item.available;
                        cout << "Availability toggled!\n";
                    }
                    return;
                }
            }
            cout << "Item ID not found!\n";
        } else {
            cout << "Invalid choice!\n";
        }
    }

    void showAvailableTables() {
        cout << "\nAvailable Tables: ";
        for (auto& pair : tableStatus)
            if (!pair.second) cout << pair.first << " ";
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
        if (tableStatus.count(tableNo)) tableStatus[tableNo] = false;
    }

    void collectFeedback(const string& name) {
        char res;
        cout << "\nLeave feedback? (y/n): ";
        cin >> res;
        cin.ignore();
        if (res == 'y' || res == 'Y') {
            string fb;
            cout << "Enter feedback: ";
            getline(cin, fb);
            ofstream f("feedback.txt", ios::app);
            time_t now = time(0);
            f << "Customer: " << name << "\nFeedback: " << fb << "\nTime: " << ctime(&now) << "------------------\n";
            cout << "Thank you!\n";
        }
    }

    void takeOrder() {
        string name;
        int table;
        cout << "Customer name: ";
        cin.ignore();
        getline(cin, name);
        showAvailableTables();
        cout << "Choose table: ";
        cin >> table;

        if (!bookTable(table)) {
            cout << "Table not available!\n";
            return;
        }

        Order order(nextOrderId++, name, table);
        char more;
        int id, qty;

        do {
            showMenu();
            cout << "Enter item ID: ";
            cin >> id;
            cout << "Enter quantity: ";
            cin >> qty;

            auto it = find_if(menu.begin(), menu.end(), [id](MenuItem& m) { return m.id == id && m.available; });
            if (it != menu.end()) {
                order.addItem(OrderItem(*it, qty));
            } else {
                cout << "Invalid item or unavailable.\n";
            }

            cout << "Add more items? (y/n): ";
            cin >> more;
        } while (more == 'y' || more == 'Y');

        order.printBill();
        order.saveToFile();
        freeTable(table);
        cout << "Order completed.\n";
        collectFeedback(name);
    }

    void showOrderHistory() {
        ifstream file("orders.txt");
        if (!file) {
            cout << "No order history found.\n";
            return;
        }
        string line;
        cout << "\n--- Order History ---\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
    }

    void showFeedback() {
        ifstream file("feedback.txt");
        string line;
        cout << "\n--- Customer Feedback ---\n";
        while (getline(file, line)) cout << line << endl;
    }

    void clearFeedback() {
        if (!authenticateAdmin()) return;
        ofstream file("feedback.txt");
        file.close();
        cout << "Feedback cleared.\n";
    }

    void printSummaryReport() {
        ifstream file("orders.txt");
        string line;
        int orders = 0;
        double total = 0;

        while (getline(file, line)) {
            size_t pos = 0;
            while ((pos = line.find(";")) != string::npos) {
                size_t start = line.rfind(",", pos - 1);
                if (start != string::npos) {
                    try {
                        double val = stod(line.substr(start + 1, pos - start - 1));
                        total += val;
                    } catch (...) {}
                }
                line.erase(0, pos + 1);
            }
            orders++;
        }

        cout << "\n--- Summary Report ---\nTotal Orders: " << orders << "\nTotal Revenue: Rs. " << total << "\n----------------------\n";
    }

    bool authenticateAdmin() {
        string pass;
        cout << "Enter admin password: ";
        cin >> pass;
        return pass == adminPassword;
    }

    void run() {
        int choice;
        do {
            cout << "\n==== RESTAURANT SYSTEM ====\n";
            cout << "1. Show Menu\n2. Take Order\n3. Order History\n4. Show Feedback\n5. Edit Menu\n";
            cout << "6. Search Item\n7. Summary Report\n8. Clear Feedback\n9. Exit\nChoose: ";
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
                case 9: cout << "Goodbye!\n"; break;
                default: cout << "Invalid option.\n";
            }
        } while (choice != 9);
    }
};

// Main Function
int main() {
    Restaurant r;
    r.run();
    return 0;
}

