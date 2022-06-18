// groceries_template.cpp: Stores Orders in a list.

#include <fstream>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>
#include "split.h"
#include <algorithm>
using namespace std;


// project notes:
// DO NOT change access modifiers
// amount cannot be made public, still need to modify value
// hint -> pointers and friend functions
// hint amount inside of payment is set inside of order becasue order has payment object member.  Has access to all private data within itself
// Add constructor for payment methods
// tab for printing data is 7 spaces
// output file should be ~1600 lines + last order is 609
// fix existing compiler errors and have no compiler errors.  Comparing different signed ints in for loops


//////////////////
// Customer code /
//////////////////
struct Customer {
    int cust_id;
    string name;
    string street;
    string city;
    string state;
    string zip;
    string phone;
    string email;
    Customer(int id, const string& name, const string& street, const string& city,
             const string& state, const string& zip, const string& phone, const string& email)
        : name(name), street(street), city(city), state(state), zip(zip), phone(phone), email(email)
    {
        cust_id = id;
    }
    string print_detail() const {
        // TODO
        return "Customer ID #" + to_string(cust_id) + ":\n" + name +
        ", ph. " + phone + ", email: " + email + "\n" + street +
        "\n" + city + ", " + state + " " + zip + "\n";
    }
};
vector<Customer> customers;

void read_customers(const string& fname) {
    // TODO
    // read data from file to customers vector using customer constructor
    ifstream cust_file(fname);
    string line;

    while(getline(cust_file, line)) {
        vector<string> substrings = split(line, ',');

        customers.emplace_back(stoi(substrings.at(0)), substrings.at(1), substrings.at(2),
        substrings.at(3),substrings.at(4),substrings.at(5),substrings.at(6),substrings.at(7));
    }
}

int find_cust_idx(int cust_id) {
    for (size_t i = 0; i < customers.size(); ++i)
        if (cust_id == customers[i].cust_id)
            return i;
    throw runtime_error("Customer not found");
}

//////////////
// Item code /
//////////////
struct Item {
    int item_id;
    string description;
    double price;
    Item(int id, const string& desc, double pric) : description(desc) {
        item_id = id;
        price = pric;
    }
};

vector<Item> items;

void read_items(const string& fname) {
    // TODO
    // Read data from file to items vector under item objects constructor
    ifstream item_file(fname);
    string line;

    while(getline(item_file, line)) {
        vector<string> substrings = split(line, ',');

        items.emplace_back(stoi(substrings.at(0)), substrings.at(1), stod(substrings.at(2)));
    }
}

int find_item_idx(int item_id) {
    for (size_t i = 0; i < items.size(); ++i)
        if (item_id == items[i].item_id)
            return i;
    throw runtime_error("Item not found");
}

class LineItem {
    int item_id;
    int qty;
    friend class Order;
public:
    LineItem(int id, int q) {
        item_id = id;
        qty = q;
    }
    double sub_total() const {
        int idx = find_item_idx(item_id);
        return items[idx].price * qty;
    }
    friend bool operator<(const LineItem& item1, const LineItem& item2) {
        return item1.item_id < item2.item_id;
    }
};

/////////////////
// Payment code /
/////////////////
class Payment {
    double amount = 0.0;
    friend class Order;
public:
    Payment() = default;
    virtual ~Payment() = default;
    virtual string print_detail() const = 0;
};

class Credit : public Payment {
    // TODO
    // Build class for payment options
    string card_number;
    string expiration;
public:
    Credit(string new_card_num, string new_exp) {
        card_number = new_card_num;
        expiration = new_exp;
    }
    string print_detail() const {
        return "Paid by Credit card " + card_number + ", exp. " + expiration;
    }

};

class Paypal : public Payment {
    // TODO
    string paypal_id;
public:
    Paypal(string new_id) {
        paypal_id = new_id;
    }
    string print_detail() const {
        return "Paid by Paypal ID: " + paypal_id;
    }
};

class WireTransfer : public Payment {
    // TODO
    string bank_id;
    string account_id;
public:
    WireTransfer(string new_bank_id, string new_account_id) {
        bank_id = new_bank_id;
        account_id = new_account_id;
    }
    string print_detail() const {
        return "Paid by Wire transfer from Bank ID " + bank_id + ", Account# " + account_id;
    }
};

///////////////
// Order code /
///////////////
class Order {
    int order_id;
    string order_date;
    int cust_id;
    vector<LineItem> line_items;
    Payment* payment;
public:
    Order(int id, const string& date, int c_id, const vector<LineItem>& items, Payment* p)
    : order_date(date), line_items(items) {
        order_id = id;
        cust_id = c_id;
        payment = p;
        sort(line_items.begin(), line_items.end());
        // Compute order_total: TODO



    }
    ~Order() {
        delete payment;
    }
    double total() const {
        return payment->amount;
    }
    string print_order() const {
        // TODO
        // return print items needed
        string items_str = "Order Detail:";
        for (size_t i = 0; i < line_items.size(); i++) {
            int item_id = line_items.at(i).item_id;
            int item_index = find_item_idx(item_id);
            string item_name = items.at(item_index).description;
            double item_price = items.at(item_index).price;

            items_str += "\n       Item " + to_string(item_id) + ": \"" + item_name +
            "\", " + std::to_string(line_items.at(i).qty) + " @ " + to_string(item_price);
        }
        items_str += "\n";

        int cust_index = find_cust_idx(cust_id);
        string cust_string = customers.at(cust_index).print_detail();

        stringstream ss;
        ss << "===========================" <<
                   "\nOrder #" << to_string(order_id) << ", Date: " << order_date <<
                   "\nAmount: $" << to_string(total()) << ", " << payment->print_detail() <<
                   "\n\n" << cust_string << // this is four lines long
                   "\n" << items_str;

        return ss.str();
    }
};
list<Order> orders;

void read_orders(const string& fname) {
    ifstream orderf(fname);
    string line1,line2;
    while (getline(orderf, line1) && getline(orderf,line2)) {
        // TODO
        // split line
        vector<string> strings1 = split(line1, ',');
        vector<string> strings2 = split(line2, ',');
        // Extract cust_id, order_id, and date
        int cust_id = stoi(strings1.at(0));
        int order_id = stoi(strings1.at(1));
        string date = strings1.at(2);
        // extract all order id's
        // Create line item vector
        vector<LineItem> line_items;
        for (size_t i = 3; i < strings1.size(); i++) {
            vector<string> item_strings = split(strings1.at(i),'-');
            line_items.emplace_back(stoi(item_strings.at(0)),stoi(item_strings.at(1)));
        }
        // TODO
        sort(begin(line_items), end(line_items));
        // Read payment method (by reading/splitting next line in file)
        // TODO
        // Create concrete Payment object on heap (pmt)
        Payment* pmt;
        // TODO
        // define what kind of payment used
        switch(strings2.at(0).at(0)) {
            case '1': // credit card (card number and expiration date)
                pmt = new Credit(strings2.at(1),strings2.at(2));
                break;
            case '2': // PayPal (paypal_id only)
                pmt = new Paypal(strings2.at(1));
                break;
            case '3': // wire transfer (bank_id and account_id)
                pmt = new WireTransfer(strings2.at(1),strings2.at(2));
                break;
            default:
                cout << "ERROR: default reached in read_orders() switch statement" << endl;
        }
        orders.emplace_back(order_id, date, cust_id, line_items, pmt);
    }
}

int main() {
    read_customers("customers.txt");
    read_items("items.txt");
    read_orders("orders.txt");

    ofstream ofs("order_report.txt");
    for (const Order& order: orders)
        ofs << order.print_order() << endl;
}
