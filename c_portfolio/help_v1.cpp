#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <set>


using namespace std;

tm first_day_of_week = {};



void registerUser();
bool loginUser();
bool isLoggedIn = false;
string generateBookingNumber();
bool isValidID(const string& id, bool isIC);
bool isValidPhoneNumber(const string& phoneNumber);
bool isValidDate(const string& date);
int processPayment();
void confirmBookingDetails();
void displayCustomerOptions();
bool isValidBookingTime(const string& time);
bool isTimeDifferenceValid(const string& time1, const string& time2, int requiredMinutes); 
void displayLogo();
void displayBookingSchedule();
void displayConsultationSchedule();
bool isValidDate(int year, int month, int day);
tm getInputDate(int year, int month, int day);
void displayCustomerDetails();
void handleAdminChoice(int choice, bool& isLoggedIn);
void showAdminMenu();
void updateBookingSchedule(const string& preferredDate, const string& preferredTime, const string& expert);
void updateConsultationSchedule(const string& preferredDate, const string& preferredTime, const string& expert);
string getTimeSlot(const string& time);
void resetBookingDetails(); 



struct Customer {
    string name;
    string id;
    string contactNumber;
    string preferredDate;
    string preferredTime;
};

struct BookingDetails {
    string bookingNo;
    string service;
    string expert;
    string sessionType;
    string consultationService;     // Service for consultation
    string consultationDate;        // Date for consultation
    string consultationExpert;      // Expert for consultation
    string consultationTime;
    string treatmentService;     // Service for consultation
    string treatmentDate;        // Date for consultation
    string treatmentExpert;      // Expert for consultation
    string treatmentTime;
    double consultationPrice = 0;   // Price for consultation (if any)
    double treatmentPrice = 0;      // Price for treatment (if any)
    int sessionsBooked;
};

Customer customer;
BookingDetails bookingDetails = {};  // Initialize all fields to default values
map<string, Customer> registeredCustomers;
set<string> uniqueIDs;
set<string> uniqueContactNumbers;

pair<int, int> convertToTimeSimple(const string& timeStr) {
    int hour = stoi(timeStr.substr(0, 2));  // Extract hour
    int minute = stoi(timeStr.substr(3, 2));  // Extract minute
    return make_pair(hour, minute);  // Return as a pair (hour, minute)
}



class Expert {
public:
    string name;
    int dailyHoursWorked;
    int weeklyHoursWorked;
    const int maxDailyHours = 6;
    const int maxWeeklyHours = 30;
    const int consultationHours = 1;
    const int treatmentHours = 2;

    Expert(string n) : name(n), dailyHoursWorked(0), weeklyHoursWorked(0) {}

    bool bookSession(char type) {
        int hours = 0;

        if (type == 'C' || type == 'c') {
            hours = consultationHours;
            bookingDetails.sessionType = "Consultation";
        }
        else if (type == 'T' || type == 't') {
            hours = treatmentHours;
            bookingDetails.sessionType = "Treatment";
        }
        else {
            cout << "Invalid session type." << endl;
            return false;
        }

        if (dailyHoursWorked + hours <= maxDailyHours && weeklyHoursWorked + hours <= maxWeeklyHours) {
            dailyHoursWorked += hours;
            weeklyHoursWorked += hours;
            cout << setfill(' ') << endl;
            cout << "Session booked for " << bookingDetails.sessionType << " with " << hours << " hours." << endl;
            return true;
        }
        else {
            cout << "Cannot book session for " << bookingDetails.sessionType << ". Maximum hours reached." << endl;
            return false;
        }
    }

    void displaySchedule() const {
        cout << setfill(' ') << endl;
        cout << "Expert: " << name << endl;
        cout << "Daily hours worked: " << dailyHoursWorked << " / " << maxDailyHours << endl;
        cout << "Weekly hours worked: " << weeklyHoursWorked << " / " << maxWeeklyHours << endl;
    }
};

void displayLogo() {
    ifstream file("C:\\logo 1.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cout << "Unable to open file." << endl;
    }
}

void displayAllExperts(const Expert experts[], int size) {
    for (int i = 0; i < size; i++) {
        cout << i + 1 << ". " << experts[i].name << endl;
    }
}

void resetBookingDetails() {
    bookingDetails.consultationService = "";
    bookingDetails.consultationDate = "";
    bookingDetails.consultationExpert = "";
    bookingDetails.consultationTime = "";
    bookingDetails.consultationPrice = 0.0;

    bookingDetails.treatmentService = "";
    bookingDetails.treatmentDate = "";
    bookingDetails.treatmentExpert = "";
    bookingDetails.treatmentTime = "";
    bookingDetails.treatmentPrice = 0.0;
}

void selectExpertSlot(Expert& expert) {
    char sessionType;
    char continueBooking;
    int slotCount = 0;
 

    // Variables to track previous bookings
    string previousTime = "";
    string previousDate = "";
    bool isConsultationFirst = false;  // Track if consultation is booked first

    do {
        cout << setfill(' ') << endl;
        cout << "Enter session type (C for consultation, T for treatment): ";
        cin >> sessionType;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (sessionType == 'C' || sessionType == 'c') {
            // Consultation flow
            cout << "Enter your preferred date for consultation (DD/MM/YYYY): ";
            getline(cin, customer.preferredDate);

            if (!isValidDate(customer.preferredDate)) {
                cout << "Invalid date format. Please try again." << endl;
                break;
            }

            cout << "Enter your preferred time (HH:MM) using 24 hr format: ";
            getline(cin, customer.preferredTime);
            if (!isValidBookingTime(customer.preferredTime)) {
                cout << "Invalid time. Please book between 09:00 and 17:00." << endl;
                break;
            }


        
            // Store the consultation details
            bookingDetails.consultationService = "Consultation";
            bookingDetails.consultationDate = customer.preferredDate;
            bookingDetails.consultationExpert = expert.name;

            // Set the price for the consultation to RM0.00
            bookingDetails.consultationPrice = 0.00;  // Free consultation

            if (expert.bookSession('C')) {
                expert.displaySchedule();
                slotCount++;
            }
            else {
                cout << "Booking failed for consultation. Cannot proceed further." << endl;
                break;
            }
        }

        else if (sessionType == 'T' || sessionType == 't') {
            // Treatment flow
            cout << "Enter your preferred date for treatment (DD/MM/YYYY): ";
            getline(cin, customer.preferredDate);

            if (!isValidDate(customer.preferredDate)) {
                cout << "Invalid date format. Please try again." << endl;
                break;
            }

            cout << "Enter your preferred time (HH:MM) using 24 hr format: ";
            getline(cin, customer.preferredTime);
            if (!isValidBookingTime(customer.preferredTime)) {
                cout << "Invalid time. Please book between 09:00 and 17:00." << endl;
                break;
            }

            // Check if the selected date is the same as consultation date
            if (customer.preferredDate == previousDate) {
                // Ensure the treatment is scheduled at least 2 hours after consultation
                cout << "Enter your preferred time for treatment (HH:MM) using 24-hour format: ";
                getline(cin, customer.preferredTime);

                if (!isValidBookingTime(customer.preferredTime)) {
                    cout << "Invalid time. Please book between 09:00 and 17:00." << endl;
                    break;
                }

                // Convert consultation and treatment times into total minutes
                int consultationHour = stoi(previousTime.substr(0, 2));
                int consultationMinute = stoi(previousTime.substr(3, 2));
                int treatmentHour = stoi(customer.preferredTime.substr(0, 2));
                int treatmentMinute = stoi(customer.preferredTime.substr(3, 2));

                // Convert both times to total minutes
                int consultationTotalMinutes = consultationHour * 60 + consultationMinute;
                int treatmentTotalMinutes = treatmentHour * 60 + treatmentMinute;

                // Ensure the treatment is at least 120 minutes (2 hours) after consultation
                if (treatmentTotalMinutes < consultationTotalMinutes + 120) {
                    cout << "Error: Treatment must be scheduled at least 2 hours after consultation." << endl;
                    cout << "Please choose a time after "
                        << setw(2) << setfill('0') << (consultationTotalMinutes + 120) / 60 << ":"
                        << setw(2) << setfill('0') << (consultationTotalMinutes + 120) % 60 << "." << endl;
                    break;
                }
            }
            else {
                // Different date, no time conflict
                cout << "Enter your preferred time for treatment (HH:MM) using 24-hour format: ";
                getline(cin, customer.preferredTime);
            }

            // Store the treatment details
            bookingDetails.treatmentService = "Treatment";
            bookingDetails.treatmentDate = customer.preferredDate;
            bookingDetails.treatmentExpert = expert.name;

            if (expert.bookSession('T')) {
                expert.displaySchedule();
                slotCount++;
            }
            else {
                cout << "Booking failed for treatment. Cannot proceed further." << endl;
                break;
            }
        }
        else {
            cout << "Invalid session type. Please enter 'C' for consultation or 'T' for treatment." << endl;
        }

        // Allow booking of multiple sessions
        if (slotCount >= 2) {
            cout << "You have reached the maximum number of sessions you can book." << endl;
            break;
        }
        cout << setfill('-') << endl;
        cout << "Do you want to book another session? (y/n): ";
        cin >> continueBooking;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while ((continueBooking == 'y' || continueBooking == 'Y') && slotCount < 2);

    // Display final schedule after booking sessions
    cout << "Final Schedule for " << expert.name << ":" << endl;
    expert.displaySchedule();
    cout << string(15, '-') << endl;

    // Confirm the booking details
    confirmBookingDetails();
}

void displayCustomerOptions() {
    char customerChoice = ' ';
    bool isIC = false;

    while (true) {
        system("cls");  // Clear the screen
        displayLogo();  // Show the logo
        cout << "Customer Service List:" << endl;
        cout << string(15, '-') << endl;
        cout << "R: Register new Account" << endl;
        cout << "E: Log into Existing Account" << endl;
        cout << "B: Book Service" << endl;
        cout << "V: View Appointment" << endl;
        cout << "M: Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> customerChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        system("cls");  // Clear the screen

        switch (toupper(customerChoice)) {
        case 'R':
            displayLogo();
            registerUser();

            break;

        case 'E':
            displayLogo();
            if (loginUser()) {
                isLoggedIn = true;
                cout << setfill(' ') << endl;
                cout << "Customer Logged In Successfully" << endl;
                cout << setfill(' ') << endl;
                cout << "Proceeding to Customer Service List..." << endl;
            }

            break;

        case 'B': {
            displayLogo();
            if (!isLoggedIn) {
                cout << "You must log in before booking a service." << endl;
                break;
            }
            
            resetBookingDetails();

            cout << "Booking Service..." << endl;
            cout << string(15, '-') << endl;
            cout << "Customer Name: " << customer.name << endl;
            cout << "Contact Number: " << customer.contactNumber << endl;
            cout << "Customer ID:" << customer.id << endl;
            cout << setfill(' ') << endl;
            cout << setfill(' ') << endl;
            string generatedBookingNo = generateBookingNumber();
            bookingDetails.bookingNo = generatedBookingNo;
            cout << "Booking No: " << generatedBookingNo << " generated." << endl;

            cout << "Press Enter to continue...";
            cin.get();

            Expert experts[3] = { Expert("Ms. Eunice"), Expert("Ms. Candy"), Expert("Ms. Sophia") };

            system("cls");
            displayLogo();
            cout << "Choose Service for Booking:" << endl;
            cout << string(15, '-') << endl;
            cout << "1: Hair Extensions (RM330) - Expert: Ms. Eunice" << endl;
            cout << "2: Nail Art (RM350) - Expert: Ms. Candy" << endl;
            cout << "3: Facial Treatment (RM360) - Expert: Ms. Sophia" << endl;
            cout << setfill(' ') << endl;
            cout << "Enter your choice: ";
            int expertChoice;
            cin >> expertChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            
            if (expertChoice < 1 || expertChoice > 3) {
                cout << "Invalid Expert choice entered. Returning to previous menu." << endl;
                break;
            }

            switch (expertChoice) {
            case 1:
                bookingDetails.service = "Hair Extensions";
                bookingDetails.treatmentPrice = 330;
                break;
            case 2:
                bookingDetails.service = "Nail Art";
                bookingDetails.treatmentPrice = 350;
                break;
            case 3:
                bookingDetails.service = "Facial Treatment";
                bookingDetails.treatmentPrice = 360;
                break;
            }
            cout << setfill(' ') << endl;
            bookingDetails.expert = experts[expertChoice - 1].name;
            cout << "Expert " << bookingDetails.expert << " selected." << endl;

            selectExpertSlot(experts[expertChoice - 1]);

            confirmBookingDetails();
            system("cls");
            processPayment();
            break;
        }

        case 'V':
            displayLogo();
            cout << "Viewing Appointment..." << endl;
            cout << string(22, '-') << endl;

            if (bookingDetails.bookingNo.empty()) {
                cout << "No appointment booked yet." << endl;
            }
            else {
                cout << "Customer Name: " << customer.name << endl;
                cout << "IC/Passport: " << customer.id << endl;
                cout << "Contact Number: " << customer.contactNumber << endl;
                cout << "Booking Number: " << bookingDetails.bookingNo << endl;

                if (!bookingDetails.consultationService.empty()) {
                    cout << "Consultation Service: " << bookingDetails.consultationService << endl;
                    cout << "Consultation Date: " << bookingDetails.consultationDate << endl;
                    cout << "Consultation Expert: " << bookingDetails.consultationExpert << endl;
                    cout << "Preferred Time: " << customer.preferredTime << endl;
                }

                if (!bookingDetails.service.empty()) {
                    cout << "Treatment Service: " << bookingDetails.treatmentService << endl;
                    cout << "Treatment Date: " << customer.preferredDate << endl;
                    cout << "Treatment Expert: " << bookingDetails.expert << endl;
                    cout << "Preferred Time: " << customer.preferredTime << endl;
                }

                // Calculate total amount (sum of consultation and treatment prices)
                double totalAmount = bookingDetails.consultationPrice + bookingDetails.treatmentPrice;
                cout << "Total Amount: RM" << fixed << setprecision(2) << totalAmount << endl;
            }
            break;

        case 'M':
            system("cls");  // Clear the screen
            return;  // Exit the function and go back to the main menu

        default:
            displayLogo();
            cout << "Invalid Customer Service option entered." << endl;
            break;
        }
        cout << endl;
        system("pause");
        system("cls");
    }
}

void registerUser() {
    cout << "Registration" << endl;
    cout << string(15, '-') << endl;
    cout << "Enter your name: ";
    getline(cin, customer.name);
    cout << setfill(' ') << endl;
    char idChoice;
    bool isIC = false;
    cout << "Enter 'I' for IC or 'P' for Passport: ";
    cin >> idChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << setfill(' ') << endl;

    if (toupper(idChoice) == 'I') {
        cout << "Enter your IC (12 digits): ";
        isIC = true;
    }

    else if (toupper(idChoice) == 'P') {
        cout << "Enter your Passport (7 or 8 characters and digits): ";
        isIC = false;
    }
    else {
        cout << "Invalid ID choice. Returning to previous menu." << endl;
        return;
    }

    getline(cin, customer.id);

    if (!isValidID(customer.id, isIC)) {
        cout << "Invalid ID format. Please try again." << endl;
        return;
    }

    if (uniqueIDs.find(customer.id) != uniqueIDs.end()) {
        cout << "This ID already exists. Please try again." << endl;
        return;
    }
    uniqueIDs.insert(customer.id);
    cout << setfill(' ') << endl;
    cout << "Enter your contact number (11 digits): ";
    getline(cin, customer.contactNumber);

    if (!isValidPhoneNumber(customer.contactNumber)) {
        cout << "Invalid phone number format. Please try again." << endl;
        return;
    }

    if (uniqueContactNumbers.find(customer.contactNumber) != uniqueContactNumbers.end()) {
        cout << "This contact number already exists. Please try again." << endl;
        return;
    }
    uniqueContactNumbers.insert(customer.contactNumber);
    cout << setfill(' ') << endl;
    cout << "ID and contact number successfully added." << endl;
    registeredCustomers[customer.id] = customer;
    cout << "Account registered successfully! Please log in to continue." << endl;
}

bool loginUser() {
    string id;
    char idChoice;
    bool isIC = false;

    cout << "Log into Account" << endl;
    cout << string(15, '-') << endl;
    cout << "Enter 'I' for IC or 'P' for Passport: ";
    cin >> idChoice;
    cout << setfill(' ') << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(idChoice) == 'I') {
        cout << "Enter your IC (12 digits): ";
        isIC = true;
    }
    else if (toupper(idChoice) == 'P') {
        cout << "Enter your Passport (7 or 8 characters and digits): ";
        isIC = false;
    }
    else {
        cout << "Invalid ID choice. Returning to previous menu." << endl;
        return false;
    }

    getline(cin, id);

    if (!isValidID(id, isIC)) {
        cout << "Invalid ID format. Please try again." << endl;
        return false;
    }

    if (registeredCustomers.find(id) != registeredCustomers.end()) {
        customer = registeredCustomers[id];
        return true;
    }
    else {
        cout << "Customer not found. Please register." << endl;
        return false;
    }
}

string generateBookingNumber() {
    srand(static_cast<unsigned int>(time(0)));
    stringstream ss;
    ss << "BKG" << setw(5) << setfill('0') << (rand() % 100000);
    return ss.str();
}

bool isValidID(const string& id, bool isIC) {
    if (isIC) {
        if (id.length() != 12) return false;
        for (char c : id) {
            if (!isdigit(c)) return false;
        }
    }
    else {
        if (id.length() < 7 || id.length() > 8) return false;
        for (char c : id) {
            if (!isalnum(c)) return false;
        }
    }
    return true;
}

bool isValidPhoneNumber(const string& phoneNumber) {
    if (phoneNumber.length() != 11) return false;
    for (char c : phoneNumber) {
        if (!isdigit(c)) return false;
    }
    return true;
}


bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;

    string dayStr = date.substr(0, 2);
    string monthStr = date.substr(3, 2);
    string yearStr = date.substr(6, 4);

    int day = stoi(dayStr);
    int month = stoi(monthStr);
    int year = stoi(yearStr);

    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year < 1900 || year > 2100) return false;

    return true;
}


bool isValidBookingTime(const string& time) {
    if (time.length() != 5 || time[2] != ':') return false;

    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));

    if (hour < 9 || hour > 17 || (hour == 17 && minute > 0)) return false;

    return true;

}

bool isTimeDifferenceValid(const string& firstTime, const string& secondTime, int requiredMinutes) {
    int firstHours = stoi(firstTime.substr(0, 2));
    int firstMinutes = stoi(firstTime.substr(3, 2));
    int secondHours = stoi(secondTime.substr(0, 2));
    int secondMinutes = stoi(secondTime.substr(3, 2));

    int totalFirstMinutes = firstHours * 60 + firstMinutes;
    int totalSecondMinutes = secondHours * 60 + secondMinutes;

    return (totalSecondMinutes - totalFirstMinutes) >= requiredMinutes;
}


bool isValidCreditCard(const string& cardNumber) {
    // Validate length (most card numbers are 16 digits)
    if (cardNumber.length() != 16) return false;

    // Ensure all characters are digits
    for (char c : cardNumber) {
        if (!isdigit(c)) return false;
    }

    return true;
}

string getCreditCardNumber() {
    string cardNumber;
    do {
        cout << "Enter your 16-digit credit card number: ";
        cin >> cardNumber;

        if (!isValidCreditCard(cardNumber)) {
            cout << "Invalid card number. Please try again." << endl;
        }
    } while (!isValidCreditCard(cardNumber));

    return cardNumber;
}



int processPayment() {
    displayLogo();
    
   
    // Calculate total amount (sum of consultation and treatment prices)
    double totalAmount = bookingDetails.consultationPrice + bookingDetails.treatmentPrice;
    cout << "Total Amount: RM" << fixed << setprecision(2) << totalAmount << endl;

    int paymentOption;
    cout << "Proceed to Payment:" << endl;
    cout << setw(15) << setfill('-') << endl;
    cout << setfill(' ');
    cout << "1: Credit/Debit Card" << endl;
    cout << "2: eWallet" << endl;
    cout << "3: Online Banking" << endl;
    cout << setfill(' ') << endl;
    cout << "Enter payment option (1/2/3): ";
    cin >> paymentOption;

    if (paymentOption == 1) {
        string cardNumber = getCreditCardNumber();
        cout << setfill(' ') << endl;
        cout << "Processing payment with card ending in " << cardNumber.substr(12, 4) << "..." << endl;
        cout << setfill(' ') << endl;
        cout << "Payment successful." << endl;
    }
    else if (paymentOption == 2 || paymentOption == 3) {
        cout << "Payment processing... Please wait." << endl;
        cout << "Payment successful." << endl;
    }
    else {
        cout << "Invalid payment option selected. Payment failed." << endl;
    }

    return paymentOption;
}


void confirmBookingDetails() {
    system("cls");
    displayLogo();
    cout << "Confirming Booking Details:" << endl;
    cout << setw(15) << setfill('-') << endl;
    cout << setfill(' ');

    cout << "Booking Number: " << bookingDetails.bookingNo << endl;
    cout << "Customer Name: " << customer.name << endl;
    cout << "Contact Number: " << customer.contactNumber << endl;
    cout << "Customer ID:" << customer.id << endl;

    cout << setw(15) << setfill('-') << endl;

    // Show consultation details if booked
    if (!bookingDetails.consultationService.empty()) {
        cout << "Consultation Service: " << bookingDetails.consultationService << endl;
        cout << "Consultation Date: " << bookingDetails.consultationDate << endl;
        cout << "Consultation Expert: " << bookingDetails.consultationExpert << endl;
        cout << "Preferred Time: " << bookingDetails.consultationTime << endl;
    }

    // Show treatment details if booked
    if (!bookingDetails.service.empty() && !bookingDetails.consultationService.empty()) {
        cout << "Treatment Service: " << bookingDetails.service << endl;
        cout << "Treatment Date: " << customer.preferredDate << endl;
        cout << "Treatment Expert: " << bookingDetails.expert << endl;
        cout << "Preferred Time: " << bookingDetails.treatmentTime << endl;
    }
    else if (!bookingDetails.service.empty()) {
        cout << "Treatment Service: " << bookingDetails.service << endl;
        cout << "Treatment Date: " << customer.preferredDate << endl;
        cout << "Treatment Expert: " << bookingDetails.expert << endl;
        cout << "Preferred Time: " << bookingDetails.treatmentTime << endl;
    }

    cout << setw(15) << setfill('-') << endl;

    system("pause");

}

void updateBookingSchedule(const string& preferredDate, const string& preferredTime, const string& expert) {
    int day = stoi(preferredDate.substr(0, 2));
    int month = stoi(preferredDate.substr(3, 2));
    int year = stoi(preferredDate.substr(6, 4));

    if (isValidDate(year, month, day)) {
        // Proceed only if the date is valid
        system("cls");
        displayLogo();
        cout << "Updating Booking Schedule:" << endl;
        cout << setfill('-') << setw(116) << "-" << endl;

        // Check if the booking is for a treatment
        if (!bookingDetails.service.empty() && bookingDetails.sessionType == "Treatment") {
            // Match the expert with the treatment schedule and fill the slot
            if (expert == "Ms. Eunice") {
                cout << "E1: Eunice's Treatment Schedule Updated for " << preferredDate << " at " << preferredTime << endl;
                // Fill the relevant schedule slot for treatment
            }
            else if (expert == "Ms. Candy") {
                cout << "E2: Candy's Treatment Schedule Updated for " << preferredDate << " at " << preferredTime << endl;
                // Fill the relevant schedule slot for treatment
            }
            else if (expert == "Ms. Sophia") {
                cout << "E3: Sophia's Treatment Schedule Updated for " << preferredDate << " at " << preferredTime << endl;
                // Fill the relevant schedule slot for treatment
            }
            else {
                cout << "Treatment Expert not found." << endl;
            }
        }

        // Check if the booking is for a consultation
        if (!bookingDetails.consultationService.empty()) {
            // Update the consultation schedule
            updateConsultationSchedule(bookingDetails.consultationDate, preferredTime, bookingDetails.consultationExpert);
        }

    }
    else {
        cout << "Invalid preferred date entered by the customer." << endl;
    }

    cout << setfill('-') << setw(116) << "-" << endl;
    cout << "Press Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("cls");
}

void updateConsultationSchedule(const string& preferredDate, const string& preferredTime, const string& expert) {
    // Convert customer preferred date into day, month, and year
    int day = stoi(preferredDate.substr(0, 2));
    int month = stoi(preferredDate.substr(3, 2));
    int year = stoi(preferredDate.substr(6, 4));

    if (isValidDate(year, month, day)) {
        // Proceed only if the date is valid
        system("cls");
        displayLogo();
        cout << "Updating Consultation Schedule:" << endl;
        cout << setfill('-') << setw(116) << "-" << endl;

        // Match the expert with the consultation schedule and fill the slot
        if (expert == "Ms. Eunice") {
            cout << "E1: Eunice's Consultation Schedule Updated for " << preferredDate << " at " << preferredTime << endl;
            // Fill the relevant schedule slot for consultation
        }
        else if (expert == "Ms. Candy") {
            cout << "E2: Candy's Consultation Schedule Updated for " << preferredDate << " at " << preferredTime << endl;
            // Fill the relevant schedule slot for consultation
        }
        else if (expert == "Ms. Sophia") {
            cout << "E3: Sophia's Consultation Schedule Updated for " << preferredDate << " at " << preferredTime << endl;
            // Fill the relevant schedule slot for consultation
        }
        else {
            cout << "Consultation Expert not found." << endl;
        }
    }
    else {
        cout << "Invalid consultation date entered by the customer." << endl;
    }
}


class BeautySpaSales {
    static const int numServices = 3;

    string serviceNames[numServices] = { "Manicure", "Hair Extensions", "Facial" };
    double servicePrices[numServices] = { 330.0, 350.0, 360.0 };
    int serviceSessions[numServices] = { 0 };
    double totalSales;
    const double taxRate = 0.08;

public:
    BeautySpaSales() : totalSales(0.0) {}

    void performTransaction() {
        system("cls");
        displayLogo();
        int sessions[numServices];
        cout << "Enter the number of sessions for each service:\n";

        for (int i = 0; i < numServices; i++) {
            cout << serviceNames[i] << " ($" << servicePrices[i] << " per session): ";
            cin >> sessions[i];

            serviceSessions[i] += sessions[i];
            totalSales += sessions[i] * servicePrices[i];
        }

        cout << "Transaction completed successfully.\n";
        system("cls");
        displayLogo();
    }

    void displaySales() {
        system("cls");
        displayLogo();
        double taxAmount = totalSales * taxRate;
        double netSales = totalSales + taxAmount;

        cout << "\nDaily Sales Report\n";
        cout << setfill('-') << setw(78) << "" << endl;

        cout << left << setw(20) << setfill(' ') << "Service"
            << right << setw(13) << "Sessions"
            << setw(27) << "Price per Session"
            << setw(18) << "Total Sales" << endl;

        cout << setfill('-') << setw(78) << "" << endl;

        for (int i = 0; i < numServices; i++) {
            cout << left << setw(20) << setfill(' ') << serviceNames[i]
                << right << setw(10) << serviceSessions[i]
                << setw(20) << fixed << setprecision(2) << "$" << servicePrices[i]
                << setw(15) << "$" << serviceSessions[i] * servicePrices[i] << endl;
        }

        cout << setfill('-') << setw(78) << "" << endl;

        cout << right << setw(60) << setfill(' ') << "Gross Sales: "
            << setw(10) << "$" << fixed << setprecision(2) << totalSales << endl;

        cout << right << setw(60) << "Tax (8%): "
            << setw(10) << "$" << fixed << setprecision(2) << taxAmount << endl;

        cout << setfill('-') << setw(78) << "" << endl;

        cout << right << setw(60) << setfill(' ') << "Net Sales: "
            << setw(10) << "$" << fixed << setprecision(2) << netSales << endl;

        cout << endl;
        cout << "Press Enter to return to the menu..." << endl;
        cin.ignore();
        cin.get();
        system("cls");
        displayLogo();
    }
};

string getTimeSlot(const string& time) {
    // Extract hours from the input time string (e.g., "10:00")
    int hour = stoi(time.substr(0, 2));

    if (hour >= 8 && hour < 12) {
        return "am";  // Morning slot
    }
    else if (hour >= 12 && hour < 17) {
        return "pm";  // Afternoon slot
    }
    else if (hour >= 17 && hour <= 20) {
        return "ev";  // Evening slot
    }
    else {
        return "";  // Invalid time for booking
    }
}

void displayBookingSchedule() {
    int year, month, day;
    bool valid = false;

    // Input and validation (same as consultation)
    do {
        cout << "Enter the year (e.g., 2024): ";
        cin >> year;
        if (year < 1900 || year > 2100) {
            cerr << "Error: Invalid year. Please enter a value between 1900 and 2100." << endl;
            continue;
        }

        cout << "Enter the month (1-12): ";
        cin >> month;
        if (month < 1 || month > 12) {
            cerr << "Error: Invalid month. Please enter a value between 1 and 12." << endl;
            continue;
        }

        cout << "Enter the day (1-31): ";
        cin >> day;
        if (day < 1 || day > 31) {
            cerr << "Error: Invalid day. Please enter a value between 1 and 31." << endl;
            continue;
        }

        if (isValidDate(year, month, day)) {
            valid = true;
        }
        else {
            cerr << "Error: Invalid date. Please enter a valid date for the given month and year." << endl;
        }
    } while (!valid);

    tm start_day = getInputDate(year, month, day);
    tm first_day_of_week = start_day;

    // Adjust to Monday if necessary
    while (first_day_of_week.tm_wday != 1) {
        first_day_of_week.tm_mday--;
        mktime(&first_day_of_week);
    }

    system("cls");
    displayLogo();

    cout << setfill('-') << setw(116) << "-" << endl;
    cout << setfill(' ') << setw(44) << "" << "Treatment Booking Schedule" << setw(35) << "" << endl;
    cout << setfill('-') << setw(116) << "-" << endl;

    const string weekdays[] = { "Mon", "Tue", "Wed", "Thu", "Fri" };

    // Print the days and dates
    cout << setfill(' ') << setw(10) << "Day" << "|";
    for (int i = 0; i < 5; ++i) {
        tm current_day = first_day_of_week;
        current_day.tm_mday += i;
        mktime(&current_day);

        cout << setw(8) << weekdays[i] << " "
            << setfill(' ') << setw(2) << current_day.tm_mday << "/"
            << setw(2) << (current_day.tm_mon + 1) << setfill(' ') << setw(7) << "|";
    }
    cout << endl;

    cout << setfill('-') << setw(116) << "-" << endl;

    // Session headers with swapped 'pm' and 'ev'
    cout << setfill(' ') << setw(10) << "Treat" << "|";
    for (int i = 0; i < 5; ++i) {
        cout << setfill(' ') << setw(6) << "T1" << "|";  // am
        cout << setw(6) << "T2" << "|";  // ev -> pm
        cout << setw(6) << "T3" << "|";  // pm -> ev
    }
    cout << endl;

    cout << setfill(' ') << setw(10) << "Session" << "|";
    for (int i = 0; i < 5; ++i) {
        cout << setw(6) << "am" << "|";
        cout << setw(6) << "ev" << "|";  // Changed 'pm' to 'ev'
        cout << setw(6) << "pm" << "|";  // Changed 'ev' to 'pm'
    }
    cout << endl;

    cout << setfill('-') << setw(116) << "-" << endl;

    // Display schedule for each expert
    for (const string& expertName : { "E1: Eunice", "E2: Candy", "E3: Sophia" }) {
        cout << setfill(' ') << setw(10) << expertName << "|";

        for (int i = 0; i < 5; ++i) {
            tm current_day = first_day_of_week;
            current_day.tm_mday += i;
            mktime(&current_day);

            string formattedDate = to_string(current_day.tm_mday) + "/" + to_string(current_day.tm_mon + 1) + "/" + to_string(current_day.tm_year + 1900);
            string preferredSlot = getTimeSlot(customer.preferredTime); // Get the time slot (am, pm, ev)

            // Check if customer booked a treatment with the current expert
            if ((expertName == "E1: Eunice" && bookingDetails.expert == "Ms. Eunice") ||
                (expertName == "E2: Candy" && bookingDetails.expert == "Ms. Candy") ||
                (expertName == "E3: Sophia" && bookingDetails.expert == "Ms. Sophia")) {

                if (customer.preferredDate == formattedDate && preferredSlot == "am") {
                    cout << setw(6) << "/" << "|";  // Mark 'am' session
                }
                else {
                    cout << setw(6) << " " << "|";
                }

                if (customer.preferredDate == formattedDate && preferredSlot == "ev") {
                    cout << setw(6) << "/" << "|";  // Mark 'ev' session
                }
                else {
                    cout << setw(6) << " " << "|";
                }

                if (customer.preferredDate == formattedDate && preferredSlot == "pm") {
                    cout << setw(6) << "/" << "|";  // Mark 'pm' session
                }
                else {
                    cout << setw(6) << " " << "|";
                }
            }
            else {
                cout << setw(6) << " " << "|";  // 'am' empty
                cout << setw(6) << " " << "|";  // 'ev' empty
                cout << setw(6) << " " << "|";  // 'pm' empty
            }
        }
        cout << endl;
    }

    cout << setfill('-') << setw(116) << "-" << endl;

    cout << "Press Enter to return to the menu..." << endl;
    cin.ignore();
    cin.get();
    system("cls");
    displayLogo();
}


bool isValidDate(int year, int month, int day) {
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;


    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }
    else if (month == 2) {
        bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (day > (isLeap ? 29 : 28)) return false;
    }

    return true;
}

tm getInputDate(int year, int month, int day) {
    tm inputDate = {};
    inputDate.tm_year = year - 1900;
    inputDate.tm_mon = month - 1;
    inputDate.tm_mday = day;
    mktime(&inputDate);
    return inputDate;
}

void displayConsultationSchedule() {
    int year, month, day;
    bool valid = false;

    // Input and validation
    do {
        cout << "Enter the year (e.g., 2024): ";
        cin >> year;
        if (year < 1900 || year > 2100) {
            cerr << "Error: Invalid year. Please enter a value between 1900 and 2100." << endl;
            continue;
        }

        cout << "Enter the month (1-12): ";
        cin >> month;
        if (month < 1 || month > 12) {
            cerr << "Error: Invalid month. Please enter a value between 1 and 12." << endl;
            continue;
        }

        cout << "Enter the day (1-31): ";
        cin >> day;
        if (day < 1 || day > 31) {
            cerr << "Error: Invalid day. Please enter a value between 1 and 31." << endl;
            continue;
        }

        if (isValidDate(year, month, day)) {
            valid = true;
        }
        else {
            cerr << "Error: Invalid date. Please enter a valid date for the given month and year." << endl;
        }
    } while (!valid);

    tm start_day = getInputDate(year, month, day);
    tm first_day_of_week = start_day;

    // Adjust to Monday if necessary
    while (first_day_of_week.tm_wday != 1) {
        first_day_of_week.tm_mday--;
        mktime(&first_day_of_week);
    }

    system("cls");
    displayLogo();

    cout << setfill('-') << setw(116) << "-" << endl;
    cout << setfill(' ') << setw(44) << "" << "Consultation Schedule" << setw(35) << "" << endl;
    cout << setfill('-') << setw(116) << "-" << endl;

    const string weekdays[] = { "Mon", "Tue", "Wed", "Thu", "Fri" };

    // Print the days and dates
    cout << setfill(' ') << setw(10) << "Day" << "|";
    for (int i = 0; i < 5; ++i) {
        tm current_day = first_day_of_week;
        current_day.tm_mday += i;
        mktime(&current_day);

        cout << setw(8) << weekdays[i] << " "
            << setfill(' ') << setw(2) << current_day.tm_mday << "/"
            << setw(2) << (current_day.tm_mon + 1) << setfill(' ') << setw(7) << "|";
    }
    cout << endl;

    cout << setfill('-') << setw(116) << "-" << endl;

    // Session headers with swapped 'pm' and 'ev'
    cout << setfill(' ') << setw(10) << "Consult" << "|";
    for (int i = 0; i < 5; ++i) {
        cout << setfill(' ') << setw(6) << "T1" << "|";  // am
        cout << setw(6) << "T2" << "|";  // swapped pm and ev
        cout << setw(6) << "T3" << "|";
    }
    cout << endl;

    cout << setfill(' ') << setw(10) << "Session" << "|";
    for (int i = 0; i < 5; ++i) {
        cout << setw(6) << "am" << "|";
        cout << setw(6) << "ev" << "|";  // pm -> ev
        cout << setw(6) << "pm" << "|";  // ev -> pm
    }
    cout << endl;

    cout << setfill('-') << setw(116) << "-" << endl;

    // Display schedule for each expert
    for (const string& expertName : { "E1: Eunice", "E2: Candy", "E3: Sophia" }) {
        cout << setfill(' ') << setw(10) << expertName << "|";

        for (int i = 0; i < 5; ++i) {
            tm current_day = first_day_of_week;
            current_day.tm_mday += i;
            mktime(&current_day);

            string formattedDate = to_string(current_day.tm_mday) + "/" + to_string(current_day.tm_mon + 1) + "/" + to_string(current_day.tm_year + 1900);
            string preferredSlot = getTimeSlot(customer.preferredTime); // Get the time slot (am, pm, ev)

            // Check if the booking matches the current expert, date, and time slot
            if (bookingDetails.expert == expertName.substr(4) && customer.preferredDate == formattedDate) {
                // Debug print to check values:
                cout << "Booking Details: " << bookingDetails.expert << ", Date: " << customer.preferredDate << ", Slot: " << preferredSlot << endl;

                // Mark the consultation sessions with '/'
                if (preferredSlot == "am") {
                    cout << setw(6) << "/" << "|";  // Mark 'am' session
                }
                else {
                    cout << setw(6) << " " << "|";
                }

                if (preferredSlot == "ev") {
                    cout << setw(6) << "/" << "|";  // Mark 'ev' session
                }
                else {
                    cout << setw(6) << " " << "|";
                }

                if (preferredSlot == "pm") {
                    cout << setw(6) << "/" << "|";  // Mark 'pm' session
                }
                else {
                    cout << setw(6) << " " << "|";
                }
            }
            else {
                // If there's no booking, leave the slots empty
                cout << setw(6) << " " << "|";  // 'am' empty
                cout << setw(6) << " " << "|";  // 'ev' empty
                cout << setw(6) << " " << "|";  // 'pm' empty
            }
        }
        cout << endl;
    }

    cout << setfill('-') << setw(116) << "-" << endl;

    cout << "Press Enter to return to the menu..." << endl;
    cin.ignore();
    cin.get();
    system("cls");
    displayLogo();
}


void displayCustomerDetails() {
    if (registeredCustomers.empty()) {
        cout << "No customers registered yet." << endl;
    }
    else {
        cout << setw(43) << right << "Registered Customer Details" << endl;
        cout << setfill('-') << setw(70) << "" << endl;
        cout << setfill(' ');

        for (const auto& entry : registeredCustomers) {
            const Customer& customer = entry.second;
            cout << left << setw(25) << "Name" << ": " << customer.name << endl;
            cout << left << setw(25) << "IC / Passport Numbers" << ": " << customer.id << endl;
            cout << left << setw(25) << "Contact Number" << ": " << customer.contactNumber << endl;
            cout << setfill('-') << setw(70) << "" << endl;
        }
    }
    cout << "Press Enter to return to the menu...";
    cin.ignore();
    cin.get();
}


void showAdminMenu() {
    int choice;
    bool isLoggedIn = true;

    while (isLoggedIn) {
        system("cls");
        displayLogo();

        cout << "\n--- Admin Menu List---" << endl;
        cout << "1. View treatment schedule" << endl;
        cout << "2. View consultation schedule" << endl;
        cout << "3. View customer details" << endl;
        cout << "4. View sales report" << endl;
        cout << "5. Log out" << endl;
        cout << "Please enter your choice (1-5): ";
        cin >> choice;

        handleAdminChoice(choice, isLoggedIn);
    }

}void handleAdminChoice(int choice, bool& isLoggedIn) {
    static BeautySpaSales spaSales;

    switch (choice) {
    case 1: {
        system("cls");
        displayLogo();
        cout << "Viewing treatment schedule" << endl;
        displayBookingSchedule();
        break;
    }

    case 2: {
        system("cls");
        displayLogo();
        cout << "Viewing consultation schedule" << endl;
        displayConsultationSchedule();
        break;
    }

    case 3:
        system("cls");
        displayLogo();
        cout << "Viewing customer details" << endl;
        displayCustomerDetails();
        cout << "Press Enter to return to the menu...";
        cin.ignore();
        cin.get();
        system("cls");
        break;

    case 4: {
        system("cls");
        displayLogo();
        cout << "Viewing sales report" << endl;

        int salesChoice;
        while (true) {
            cout << "\nBeauty & Spa Sales Menu\n";
            cout << "1. Perform Transaction\n";
            cout << "2. Display Sales Report\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> salesChoice;

            switch (salesChoice) {
            case 1:
                displayLogo();
                spaSales.performTransaction();
                break;
            case 2:
                displayLogo();
                spaSales.displaySales();
                break;
            case 3:
                system("cls");
                displayLogo();
                cout << "Exiting to Admin Menu.\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }

        break;
    }

    case 5:
        system("cls");
        displayLogo();
        cout << "You have successfully logged out." << endl;
        isLoggedIn = false;
        break;

    default:
        cout << "Invalid choice entered." << endl;
    }

    cout << "Press Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("cls");
}

int main() {
    const string validusername1 = "0001", validpassword1 = "1234";
    const string validusername2 = "0002", validpassword2 = "4321";
    const string validusername3 = "0003", validpassword3 = "2143";
    string inputusername, inputpassword;
    bool isValid = false;
    int mainChoice = 0;

    while (mainChoice != 4) {
        system("cls");
        displayLogo();
        cout << "1. About Us" << endl;
        cout << "2. Customer" << endl;
        cout << "3. Staff" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> mainChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        system("cls");

        switch (mainChoice) {
        case 1:
            displayLogo();
            cout << "About Us" << endl;
            cout << string(15, '-') << endl;
            cout << "Welcome to Blissful Beauty Spa!" << endl;
            cout << "At Blissful Beauty Spa, we believe in the power of relaxation and rejuvenation." << endl;
            cout << endl;
            cout << "Our Story:" << endl;
            cout << "Blissful Beauty Spa was founded in 2010 by a group of wellness enthusiasts with a passion for beauty and self-care." << endl;
            cout << endl;
            break;

        case 2:
            displayCustomerOptions();  // This will now return to the main menu after 'M' is pressed
            break;

        case 3:
            displayLogo();
            cout << "Please log in to continue:\n";
            cout << "Username: ";
            cin >> inputusername;
            cout << "Password: ";
            cin >> inputpassword;

            if ((inputusername == validusername1 && inputpassword == validpassword1) ||
                (inputusername == validusername2 && inputpassword == validpassword2) ||
                (inputusername == validusername3 && inputpassword == validpassword3)) {
                isValid = true;
                cout << "Login successful!\n";
            }
            else {
                cout << "Invalid username or password. Please try again.\n";
                cin.ignore();
                cin.get();
            }
            cout << "Staff" << endl;
            showAdminMenu();
            break;

        case 4:
            displayLogo();
            cout << "Exiting..." << endl;
            break;

        default:
            displayLogo();
            cout << "Invalid main menu choice entered." << endl;
            break;
        }
        cout << endl;
        system("pause");
        system("cls");
    }

    return 0;
}