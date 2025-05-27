// added view order history


#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <ctime>
#include <map>
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

// Restaurant System Class
class Restaurant {
private:
    vector<MenuItem> menu;
    int nextOrderId;
    map<int, bool> tableStatus;

public:
    Restaurant() {
        nextOrderId = 1000;
        loadMenu();
        for (int i = 1; i <= 10; i++) tableStatus[i] = false; // 10 tables, all free
    }

    void loadMenu() {
        menu.push_back(MenuItem(1, "Masala Dosa", 40.0));
        menu.push_back(MenuItem(2, "Idli Vada", 30.0));
        menu.push_back(MenuItem(3, "Paneer Butter Masala", 90.0));
        menu.push_back(MenuItem(4, "Chapati", 10.0));
        menu.push_back(MenuItem(5, "Fried Rice", 80.0));
        menu.push_back(MenuItem(6, "Ice Cream", 50.0));
        menu.push_back(MenuItem(7, "Coffee", 25.0));
        menu.push_back(MenuItem(8, "Pizza", 150.0));
        menu.push_back(MenuItem(9, "Burger", 120.0));
        menu.push_back(MenuItem(10, "Noodles", 90.0));
        menu.push_back(MenuItem(11, "Gobi Manchurian", 100.0));
        menu.push_back(MenuItem(12, "Lassi", 40.0));
    }

    void showMenu() {
        cout << "\n------ MENU ------\n";
        for (auto& item : menu) {
            cout << item.id << ". " << left << setw(25) << item.name
                 << "Rs. " << fixed << setprecision(2) << item.price << endl;
        }
        cout << "------------------\n";
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

    void run() {
        int choice;
        do {
            cout << "\n==== RESTAURANT SYSTEM ====\n";
            cout << "1. Show Menu\n";
            cout << "2. Take Order\n";
            cout << "3. Show Order History\n";
            cout << "4. Exit\n";
            cout << "Choose an option: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    showMenu();
                    break;
                case 2:
                    takeOrder();
                    break;
                case 3:
                    showOrderHistory();
                    break;
                case 4:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 4);
    }
};

int main() {
    Restaurant r;
    r.run();
    return 0;
}
