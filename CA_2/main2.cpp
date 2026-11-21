#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <limits>

using namespace std;
struct DiskBooking {
    int bookingId;
    int seatNo;
    char customerName[50];
    int popcorn;
    int drink;
    int sandwich;
    double totalBill;
};
class Booking {
private:
    int bookingId;
    int seatNo;
    string customerName;
    int popcorn, drink, sandwich;
    double totalBill;
    static double ticketPrice;
    void calculateBill() {
        totalBill = ticketPrice + popcorn * 120 + drink * 50 + sandwich * 100;
    }
public:
   Booking() {
        bookingId = 0;
        seatNo = 0;
        customerName = "";
        popcorn = drink = sandwich = 0;
        totalBill = 0;
    }
    Booking(int id, int seat, string name) {
        bookingId = id;
        seatNo = seat;
        customerName = name;
        popcorn = drink = sandwich = 0;
        calculateBill();
    }
    Booking(const Booking &b) {
        bookingId = b.bookingId;
        seatNo = b.seatNo;
        customerName = b.customerName;
        popcorn = b.popcorn;
        drink = b.drink;
        sandwich = b.sandwich;
        totalBill = b.totalBill;
    }
    ~Booking() {
    }
    inline int getBookingId() const {
        return bookingId;
    }
    inline int getSeatNo() const {
        return seatNo;
    }
    void setSnacks(int p, int d, int s) {
        popcorn = p;
        drink = d;
        sandwich = s;
        calculateBill();
    }
    void display() const {
        cout << "\nBooking ID    : " << bookingId;
        cout << "\nSeat Number   : " << seatNo;
        cout << "\nCustomer Name : " << customerName;
        cout << "\nPopcorn       : " << popcorn;
        cout << "\nDrinks        : " << drink;
        cout << "\nSandwiches    : " << sandwich;
        cout << "\nTotal Bill    : Rs. " << totalBill << "\n";
    }
    DiskBooking toRecord() const {
        DiskBooking r{};
        r.bookingId = bookingId;
        r.seatNo = seatNo;
        strncpy(r.customerName, customerName.c_str(),
                sizeof(r.customerName) - 1);
        r.popcorn = popcorn;
        r.drink = drink;
        r.sandwich = sandwich;
        r.totalBill = totalBill;
        return r;
    }
     void fromRecord(const DiskBooking &r) {
        bookingId = r.bookingId;
        seatNo = r.seatNo;
        customerName = r.customerName;
        popcorn = r.popcorn;
        drink = r.drink;
        sandwich = r.sandwich;
        totalBill = r.totalBill;
    }
    friend void showBooking(const Booking &b);
};
double Booking::ticketPrice = 200;
void showBooking(const Booking &b) {
    b.display();
}
void createDataFile() {
    ifstream check("bookings.dat", ios::binary);
    if (!check) {
        ofstream create("bookings.dat", ios::binary);
        create.close();
    }
    check.close();
}
bool valueExists(int value, bool checkId) {
    ifstream file("bookings.dat", ios::binary);
    DiskBooking r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(r))) {
        if (checkId && r.bookingId == value) {
            file.close();
            return true;
        }
        if (!checkId && r.seatNo == value) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}
void addBooking() {
    int id, seat;
    string name;
    int p, d, s;
    cout << "\nEnter Booking ID: ";
    cin >> id;
    if (id <= 0 || valueExists(id, true)) {
        cout << "Invalid or duplicate booking ID.\n";
        return;
    }
    cout << "Enter Seat Number (1-20): ";
    cin >> seat;
    if (seat < 1 || seat > 20 || valueExists(seat, false)) {
        cout << "Invalid or already booked seat.\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Customer Name: ";
    getline(cin, name);
    if (name.empty() || name.length() >= 50) {
        cout << "Name must contain 1 to 49 characters.\n";
        return;
    }
    Booking b(id, seat, name);
    cout << "\nSnack Counter";
    cout << "\nPopcorn quantity (Rs.120 each): ";
    cin >> p;
    cout << "Drink quantity (Rs.50 each): ";
    cin >> d;
    cout << "Sandwich quantity (Rs.100 each): ";
    cin >> s;
    if (p < 0 || d < 0 || s < 0) {
        cout << "Snack quantities cannot be negative.\n";
        return;
    }
    b.setSnacks(p, d, s);
    DiskBooking r = b.toRecord();
    ofstream file("bookings.dat", ios::binary | ios::app);
    if (!file) {
        cout << "Unable to open data file.\n";
        return;
    }
    file.write(reinterpret_cast<const char*>(&r), sizeof(r));
    file.close();
    cout << "\nBooking saved successfully.\n";
    showBooking(b);
}
void displayAllBookings() {
    ifstream file("bookings.dat", ios::binary);
    DiskBooking r;
    bool found = false;
    cout << "\n ALL BOOKINGS \n";
    while (file.read(reinterpret_cast<char*>(&r), sizeof(r))) {
        Booking b;
        b.fromRecord(r);
        showBooking(b);
        found = true;
    }
    if (!found)
        cout << "No bookings found.\n";
    file.close();
}
void searchBooking() {
    int id;
    cout << "\nEnter Booking ID to search: ";
    cin >> id;
    ifstream file("bookings.dat", ios::binary);
    if (!file) {
        cout << "Unable to open data file.\n";
        return;
    }
    DiskBooking r;
    bool found = false;
    int recordNo = 0;
    while (true) {
        file.seekg(static_cast<streamoff>(recordNo) * sizeof(r), ios::beg);
        if (!file.read(reinterpret_cast<char*>(&r), sizeof(r)))
            break;
        if (r.bookingId == id) {
            Booking b;
            b.fromRecord(r);
            cout << "\nBooking found:\n";
            showBooking(b);
            found = true;
            break;
        }
        recordNo++;
    }
    if (!found)
        cout << "Booking ID not found.\n";
    file.close();
}
void updateSnacks() {
    int id;
    cout << "\nEnter Booking ID to update: ";
    cin >> id;
    fstream file("bookings.dat", ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Unable to open data file.\n";
        return;
    }
    DiskBooking r;
    bool found = false;
    int recordNo = 0;
    while (true) {
        file.seekg(static_cast<streamoff>(recordNo) * sizeof(r), ios::beg);
        if (!file.read(reinterpret_cast<char*>(&r), sizeof(r)))
            break;
        if (r.bookingId == id) {
            found = true;
            break;
        }
        recordNo++;
    }
    if (!found) {
        cout << "Booking ID not found.\n";
        file.close();
        return;
    }
    cout << "\nCurrent snack quantities:";
    cout << "\nPopcorn: " << r.popcorn;
    cout << "\nDrinks: " << r.drink;
    cout << "\nSandwiches: " << r.sandwich << "\n";
    int p, d, s;
    cout << "\nEnter new popcorn quantity: ";
    cin >> p;
    cout << "Enter new drink quantity: ";
    cin >> d;
    cout << "Enter new sandwich quantity: ";
    cin >> s;
    if (p < 0 || d < 0 || s < 0) {
        cout << "Snack quantities cannot be negative.\n";
        file.close();
        return;
    }
    r.popcorn = p;
    r.drink = d;
    r.sandwich = s;
    r.totalBill = 200 + p * 120 + d * 50 + s * 100;
    file.seekp(static_cast<streamoff>(recordNo) * sizeof(r), ios::beg);
    file.write(reinterpret_cast<const char*>(&r), sizeof(r));
    file.close();
    cout << "\nSnack order updated successfully.\n";
    Booking updated;
    updated.fromRecord(r);
    showBooking(updated);
}
void displaySeats() {
    cout << "\nSEAT STATUS\n";
    for (int seat = 1; seat <= 20; seat++) {
        if (valueExists(seat, false))
            cout << "Seat " << seat << " : BOOKED\n";
        else
            cout << "Seat " << seat << " : AVAILABLE\n";
    }
}
int main() {
    createDataFile();
    int choice;
    do {
        cout << "\n\n";
        cout << "\n CINEMA SEAT ALLOCATION & SNACK COUNTER";
        cout << "\n";
        cout << "\n1. Display Seat Status";
        cout << "\n2. Book Seat and Order Snacks";
        cout << "\n3. Display All Bookings";
        cout << "\n4. Search Booking by ID";
        cout << "\n5. Update Snack Order";
        cout << "\n6. Exit";
        cout << "\nEnter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid menu number.\n";
            continue;
        }
        switch (choice) {
            case 1:
                displaySeats();
                break;
            case 2:
                addBooking();
                break;
            case 3:
                displayAllBookings();
                break;
            case 4:
                searchBooking();
                break;
            case 5:
                updateSnacks();
                break;
            case 6:
                cout << "\nThank you for using the Cinema System.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);
    return 0;
}

