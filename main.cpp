#include <iostream>
#include <string>
#include <limits>
using namespace std;

// Entity Class
class Booking {
private:
    int bookingId, seatNo;
    string customerName;
    int popcorn, drink, sandwich;
    double totalBill;

    static int totalBookings;
    static double ticketPrice;

public:

    // Default Constructor
    Booking() {
        bookingId = 0;
        seatNo = 0;
        customerName = "";
        popcorn = drink = sandwich = 0;
        totalBill = 0;
    }

    // Parameterized Constructor using this pointer
    Booking(int id, int seat, string name) {
        this->bookingId = id;
        this->seatNo = seat;
        this->customerName = name;
        popcorn = drink = sandwich = 0;
        totalBill = ticketPrice;
    }

    // Add Snacks
    void addSnacks(int p, int d, int s) {
        popcorn = p;
        drink = d;
        sandwich = s;

        totalBill = ticketPrice + popcorn * 120
                  + drink * 60 + sandwich * 100;
    }

    // Getter Functions
    int getBookingId() const {
        return bookingId;
    }

    int getSeatNo() const {
        return seatNo;
    }

    // Inline Display Function
    inline void display() const {
        cout << "\nBooking ID: " << bookingId;
        cout << "\nCustomer: " << customerName;
        cout << "\nSeat Number: " << seatNo;
        cout << "\nMovie Ticket: Rs. " << ticketPrice;
        cout << "\nPopcorn: " << popcorn << " x 120";
        cout << "\nCold Drink: " << drink << " x 60";
        cout << "\nSandwich: " << sandwich << " x 100";
        cout << "\nTotal Bill: Rs. " << totalBill << "\n";
    }

    // Static Function
    static void showTotalBookings() {
        cout << "\nTotal Bookings: " << totalBookings;
    }

    static void increaseCount() {
        totalBookings++;
    }

    // Friend Function
    friend void showBooking(const Booking &b);
};

// Initialize Static Variables
int Booking::totalBookings = 0;
double Booking::ticketPrice = 200;

// Friend Function
void showBooking(const Booking &b) {
    cout << "\nBooking Summary";
    cout << "\nCustomer: " << b.customerName;
    cout << "\nBooking ID: " << b.bookingId << endl;
}

// Validate Snack Quantity
int getQuantity(string message) {
    int value;

    while (true) {
        cout << message;

        if (cin >> value && value >= 0)
            return value;

        cout << "Enter a valid non-negative quantity!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    const int SIZE = 20;

    Booking bookings[SIZE];       // Array of Objects
    Booking *ptr = bookings;      // Pointer to Array

    int count = 0, choice;

    do {
        cout << "\n\n CINEMA BOOKING SYSTEM ";
        cout << "\n1. Show Seat Status";
        cout << "\n2. Book Seat and Order Snacks";
        cout << "\n3. Display All Bookings";
        cout << "\n4. Exit";
        cout << "\nEnter Choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input!";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {

        case 1:
            for (int i = 0; i < SIZE; i++) {
                bool booked = false;

                for (int j = 0; j < count; j++) {
                    if (ptr[j].getSeatNo() == i + 1)
                        booked = true;
                }

                cout << "\nSeat " << i + 1 << ": ";
                cout << (booked ? "Booked" : "Available");
            }
            break;

        case 2: {
            if (count >= SIZE) {
                cout << "All seats are booked!";
                break;
            }

            int id, seat;
            string name;

            cout << "\nEnter Booking ID: ";
            cin >> id;

            bool duplicate = false;

            for (int i = 0; i < count; i++) {
                if (ptr[i].getBookingId() == id)
                    duplicate = true;
            }

            if (id <= 0 || duplicate) {
                cout << "Invalid or duplicate Booking ID!";
                break;
            }

            cout << "Enter Seat Number (1-20): ";
            cin >> seat;

            bool seatBooked = false;

            for (int i = 0; i < count; i++) {
                if (ptr[i].getSeatNo() == seat)
                    seatBooked = true;
            }

            if (seat < 1 || seat > SIZE || seatBooked) {
                cout << "Invalid or already booked seat!";
                break;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter Customer Name: ";
            getline(cin, name);

            if (name.empty()) {
                cout << "Name cannot be empty!";
                break;
            }

            Booking b(id, seat, name);

            cout << "\n--- SNACK COUNTER ---\n";

            int p = getQuantity("Popcorn (Rs.120): ");
            int d = getQuantity("Cold Drink (Rs.60): ");
            int s = getQuantity("Sandwich (Rs.100): ");

            b.addSnacks(p, d, s);

            ptr[count] = b;
            count++;
            Booking::increaseCount();

            cout << "\nBooking Successful!";
            showBooking(ptr[count - 1]);
            ptr[count - 1].display();

            break;
        }

        case 3:
            if (count == 0) {
                cout << "No bookings available!";
                break;
            }

            for (int i = 0; i < count; i++) {
                ptr[i].display();
            }

            Booking::showTotalBookings();
            break;

        case 4:
            cout << "Thank you for visiting!";
            break;

        default:
            cout << "Invalid choice!";
        }

    } while (choice != 4);

    return 0;
}
