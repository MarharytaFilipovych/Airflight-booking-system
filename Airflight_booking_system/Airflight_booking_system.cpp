#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <Windows.h>
using namespace std;

class Ticket
{
public:
    string place = "";
    string owner = "";
    int price = 0;
    string flight = "";
    string date = "";

    Ticket() = default;

    Ticket(const string& number, const int price_for_ticket, const string& date_of_flight, const string& flight_number)
        : place(number), price(price_for_ticket), flight(flight_number), date(date_of_flight) {}

};


class Airplane
{
    int number_of_seats_per_row = 0;
    unordered_set<char> seats;
    map<vector<int>, int> prices;
    int number_of_rows = 0;

    void getSeatNumbers()
    {
        seats.clear();
        for (int i = 0; i < number_of_seats_per_row; i++)
        {
            seats.insert('A' + i);
        }
    }
    const int findTicketPrice(const string& ticket_number) const
    {
        const int tikcet_row = stoi(ticket_number.substr(0, ticket_number.length() - 1));
        for (const auto& price_itself : prices)
        {
            if (tikcet_row >= price_itself.first.front() && tikcet_row <= price_itself.first.back())
            {
                int price = price_itself.second;
                return price;

            }
        }
        return 0;
    }
    void generateTickets()
    {
        for (int i = 1; i <= number_of_rows; i++)
        {
            for (const char seat : seats)
            {
                string ticket_number = to_string(i) + seat;
                int price = findTicketPrice(ticket_number);
                Ticket ticket(ticket_number, price, date, flight_number);
                tickets[ticket.place] = ticket;
            }
        }
    }

    void PrintPrices() const
    {
        for (auto i = prices.begin(); i != prices.end(); i++)
        {
            cout << i->first.front() << "-" << i->first.back();
            cout << " -> " << i->second << "$  ";
        }
        cout << endl;

    }
    void displayAirplane() const
    {
        cout << "Info about airplane " << flight_number << ":\n"
            << "* date: " << date << "\n"
            << "* number of seats per row: " << number_of_seats_per_row << "\n"
            << "* prices for rows: ";
        PrintPrices();
    }

public:

    const string date;
    const string flight_number;
    map<string, Ticket> tickets;
    map<string, Ticket> booked_tickets;

    Airplane() = default;

    Airplane(const string& day, const string& flight, const int number_seats, const map<vector<int>, int>& prices_for_rows, const int rows) : number_of_seats_per_row(number_seats), date(day), flight_number(flight), prices(prices_for_rows), number_of_rows(rows) {
        getSeatNumbers();
        generateTickets();
    }

    Airplane& operator=(const Airplane& other)
    {
        if (this != &other)
        {
            seats = other.seats;
            prices = other.prices;
            tickets = other.tickets;
            booked_tickets = other.booked_tickets;
            number_of_rows = other.number_of_rows;
            number_of_seats_per_row = other.number_of_seats_per_row;
        }
        return *this;
    }



    void displayAvailableTickets() const
    {
        for (auto i = tickets.begin(); i != tickets.end(); i++)
        {
            cout << i->second.place << ":" << to_string(i->second.price) + "$" << " ";
        }
    }
    void displayBookedTickets() const
    {
        for (auto i = booked_tickets.begin(); i != booked_tickets.end(); i++)
        {
            cout << i->second.place << ", " << i->second.owner << ", " << to_string(i->second.price) + "$" << " " << endl;
        }
    }

};

class FileReader
{
private:
    const string fileName = "flights.txt";
    HANDLE hFile;
    unordered_map<string, Airplane> airplanes;

    void MakeTableOfPrices(vector<string>& range_and_prices, map<vector<int>, int>& prices, int& number_of_rows) const
    {
        for (size_t i = 0; i < range_and_prices.size(); i += 2)
        {
            string range = range_and_prices[i];
            size_t dash = range.find('-');
            int start = stoi(range.substr(0, dash));
            int end = stoi(range.substr(dash + 1));
            string price_with_dollar = range_and_prices[i + 1];
            size_t dollar = price_with_dollar.find('$');
            int price = stoi(price_with_dollar.substr(0, dollar));
            vector<int> rows;
            for (int i = start; i <= end; i++)
            {
                rows.push_back(i);
            }
            prices[rows] = price;
            number_of_rows = end;
        }
    }

    void createPlanes(istringstream& this_line)
    {
        string date, flight_number;
        int seats_per_row;
        this_line >> date >> flight_number >> seats_per_row;
        vector<string> range_and_prices;
        string range_and_price;
        while (this_line >> range_and_price)
        {
            range_and_prices.push_back(range_and_price);
        }
        int number_of_rows;
        map<vector<int>, int> prices;
        MakeTableOfPrices(range_and_prices, prices, number_of_rows);
        Airplane airplane(date, flight_number, seats_per_row, prices, number_of_rows);
        string key = airplane.date + airplane.flight_number;
        airplanes[key] = airplane;
    }

    const string readFile()const
    {
        DWORD numberOfBytesRead;
        char buffer[1024];
        string fileContent;
        DWORD numberOfBytesToRead = sizeof(buffer) - 1;
        while (ReadFile(hFile, buffer, numberOfBytesToRead, &numberOfBytesRead, NULL) && numberOfBytesRead > 0) {
            buffer[numberOfBytesRead] = '\0';
            fileContent.append(buffer);
        }
        return fileContent;
    }

    void processContent()
    {
        const string content = readFile();
        istringstream stream(content);
        string line;
        int number_of_records = 0;
        if (getline(stream, line))
        {
            istringstream lineStream(line);
            lineStream >> number_of_records;
        }
        while (number_of_records > 0 && getline(stream, line))
        {
            if (line.empty())
            {
                continue;
            }
            istringstream this_line(line);
            createPlanes(this_line);
            number_of_records--;
        }
    }

public:

    FileReader() : hFile(INVALID_HANDLE_VALUE)
    {
        hFile = CreateFileA(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            cout << "Your file " << fileName << " could not be opened!" << endl;
            return;
        }
        processContent();
    }

    ~FileReader()
    {
        if (hFile != INVALID_HANDLE_VALUE)
        {
            CloseHandle(hFile);
        }
    }

    const unordered_map<string, Airplane>& GetAirplanes() const
    {
        return airplanes;
    }
};

class ID
{
    unordered_set<int> IDs;
    const int N = 10000;

public:
    ID()
    {
        srand(time(nullptr));
    }

    const int generateID() {
        int id;
        do {
            id = rand() % N;
        } while (IDs.find(id) != IDs.end());

        IDs.insert(id);
        return id;
    }
};

class Commands
{
    const FileReader fileReader;
    unordered_map<string, Ticket> bought_tickets;
    ID id_generator;
    unordered_map<string, unordered_set<int>> passengers;
    unordered_map<string, Airplane> airplanes;

    Airplane* findAirplane(const string& date, const string& flight_number)
    {
        string key = date + flight_number;
        auto airplane_it = airplanes.find(key);
        if (airplane_it != airplanes.end())
        {
            return &airplane_it->second;
        }
        cout << "Flight not found!" << endl;
        return nullptr;
    }

    Ticket* findTicket(unordered_map<string, Ticket>& tickets, const string& place)
    {
        auto ticket_it = tickets.find(place);
        if (ticket_it != tickets.end())
        {
            return &ticket_it->second;
        }
        return nullptr;
    }
    Ticket* findTicket(map<string, Ticket>& tickets, const string& place)
    {
        auto ticket_it = tickets.find(place);
        if (ticket_it != tickets.end())
        {
            return &ticket_it->second;
        }
        return nullptr;
    }

    void removeTicketFromPassenger(const string& username, int ticket_id)
    {
        unordered_set<int>* ids = findPassengerIDs(username);
        ids->erase(ticket_id);
        if (ids->empty())
        {
            passengers.erase(username);
        }
    }

    unordered_set<int>* findPassengerIDs(const string& username)
    {
        auto it = passengers.find(username);
        if (it != passengers.end())
        {
            return &it->second;
        }
        return nullptr;
    }

public:

    Commands() : airplanes(fileReader.GetAirplanes()) {}

    void checkOrViewFlight(const string& date, const string& flight_number, bool check)
    {
        Airplane* airplane = findAirplane(date, flight_number);
        if (airplane == nullptr) {
            return;
        }
        if (check)
        {
            airplane->displayAvailableTickets();
        }
        else
        {
            airplane->displayBookedTickets();
        }
        cout << endl;
    }

    void book(const string& date, const string& flight_number, const string& place, const string& username)
    {
        Airplane* airplane = findAirplane(date, flight_number);
        if (airplane == nullptr) {
            return;
        }
        Ticket* ticket = findTicket(airplane->tickets, place);
        if (ticket == nullptr) {
            cout << "Sorry, this place is already booked!" << endl;
            return;
        }
        int id = id_generator.generateID();
        passengers[username].insert(id);
        cout << "Confirmed with ID " << id << endl;
        ticket->owner = username;
        bought_tickets[to_string(id)] = *ticket;
        airplane->booked_tickets[place] = *ticket;
        airplane->tickets.erase(place);
    }

    void Return(string& id)
    {
        Ticket* ticket = findTicket(bought_tickets, id);
        if (ticket == nullptr) {
            cout << "Hmm... There is no id " << id << ". Therefore, no money back))))" << endl;
            return;
        }
        cout << "Confirmed " << ticket->price << "$ refund for " << ticket->owner << endl;
        unordered_set<int>* ids = findPassengerIDs(ticket->owner);
        removeTicketFromPassenger(ticket->owner, stoi(id));
        ticket->owner = "";
        Airplane* airplane = findAirplane(ticket->date, ticket->flight);
        if (airplane == nullptr) {
            return;
        }
        airplane->booked_tickets.erase(ticket->place);
        airplane->tickets[ticket->place] = *ticket;
        bought_tickets.erase(id);
    }

    void view(const string& id)
    {
        Ticket* ticket = findTicket(bought_tickets, id);
        if (ticket == nullptr) {
            cout << "The ticket with id " << id << " was not booked." << endl;
            return;
        }
        cout << "Flight " << ticket->flight << ", " << ticket->date << ", " << ticket->place << ", " << ticket->owner << endl;
    }

    void viewUsername(const string& username)
    {
        const unordered_set<int>* ids = findPassengerIDs(username);
        if (ids == nullptr)
        {
            cout << "Passenger not found." << endl;
            return;
        }
        if (ids != nullptr)
        {
            int i = 1;
            for (const int id : *ids)
            {
                cout << i << ". ";
                view(to_string(id));
                i++;
            }
        }
    }
};

class UserInput
{
    string userInput;
    const vector<string> commands = { "check", "view", "book", "return", "help", "exit", "view username", "view flight" };
    const vector<string> split(string& data, char delimiter) const
    {
        stringstream ss(data);
        string token;
        vector<string> tokens;
        while (getline(ss, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    bool isDigit(const string& data) const
    {
        return all_of(data.begin(), data.end(), ::isdigit);
    }

    bool isLetters(const string& data) const
    {
        return all_of(data.begin(), data.end(), ::isalpha);
    }

    bool validateReturnView(const vector<string>& data) const
    {
        if (data.size() != 1 || !isDigit(data[0]))
        {
            cout << "Invalid input!" << endl;
            return false;
        }
        return true;
    }

    bool validateFlightNumber(const string& number) const
    {
        if (number.length() != 5 || !isalpha(number[0]) || !isalpha(number[1]) || !isupper(number[0]) || !isupper(number[1]) || !isdigit(number[2]) || !isdigit(number[3]) || !isdigit(number[4])) {
            cout << "The number of your flight doesn't look correct:(" << endl;
            return false;
        }
        return true;
    }

    bool checkMonthAndYear(int month, int year)const
    {
        if (month > 12 || month < 1 || year < 2000)
        {
            cout << "Incorrect date numbers!" << endl;
            return false;
        }
        return true;
    }

    bool checkDay(int month, int day, int year)const
    {
        int daysInMonth = 31;
        if (month == 4 || month == 6 || month == 9 || month == 11) {
            daysInMonth = 30;
        }
        else if (month == 2) {
            bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            daysInMonth = isLeapYear ? 29 : 28;
        }
        if (day < 1 || day > daysInMonth) {
            cout << "The day is incorrect for the given month!" << endl;
            return false;
        }
        return true;
    }

    bool validateDate(string& date) const
    {
        if (date.length() != 10 || date[2] != '.' || date[5] != '.')
        {
            cout << "The format of the data is incorrect!" << endl;
            return false;
        }
        vector<string> parts = split(date, '.');
        if (parts.size() != 3 || !isDigit(parts[0]) || !isDigit(parts[1]) || !isDigit(parts[2]))
        {
            cout << "The format of the data is incorrect!" << endl;
            return false;
        }
        int day = stoi(parts[0]);
        int month = stoi(parts[1]);
        int year = stoi(parts[2]);

        if (!checkDay(month, day, year) || !checkMonthAndYear(month, year))
        {
            return false;
        }
        return true;
    }

    bool validatePlace(const string& place) const
    {
        for (size_t i = 0; i < place.length() - 1; i++)
        {
            if (!isdigit(place[i]))
            {
                cout << "There is no such place in the airplane :(" << endl;
                return false;
            }
        }
        char lastChar = place[place.length() - 1];

        if (!isalpha(lastChar) || !isupper(lastChar))
        {
            cout << "There is no such place in the airplane :(" << endl;
            return false;
        }

        return true;
    }

    bool validateBook(vector<string>& data) const
    {
        if (data.size() < 4)
        {
            cout << "Incorrrect data for book command :(" << endl;
            return false;
        }
        if (!validateDate(data[0]) || !validateFlightNumber(data[1]) || !validatePlace(data[2]))
        {
            return false;
        }
        for (size_t i = 3; i < data.size(); i++) {
            if (!isLetters(data[i])) {
                cout << "You name should only contain letters, shouldn't it?" << endl;
                return false;
            }
        }
        return true;
    }

    bool validateViewFlightOrCheck(vector<string>& data) const
    {
        if (data.size() != 2)
        {
            cout << "Incorrrect data for book command :(" << endl;
            return false;
        }
        if (!validateDate(data[0]) || !validateFlightNumber(data[1]))
        {
            return false;
        }
        return true;
    }

    void removeSpaces(string& username) const
    {
        username.erase(remove(username.begin(), username.end(), ' '), username.end());
    }

    bool validateUsername() const
    {
        if (!isLetters(userInput))
        {
            cout << "You name should only contain letters, shouldn't it?" << endl;
            return false;
        }
        return true;
    }

    void handleViewCommand(string& userInput, string& command) {
        list<string> type_view = { "username", "flight" };
        for (const string& view_t : type_view) {
            if (userInput.rfind(view_t, 0) == 0) {
                command += " " + view_t;
                userInput.erase(0, view_t.length() + 1);
                return;
            }
        }
    }

    void processCommand(string& userInput, string& command, const vector<string>& commands) {
        for (const string& cmd : commands) {
            if (userInput.rfind(cmd, 0) == 0) {
                command = cmd;
                userInput.erase(0, cmd.length() + 1);
                if (command == "view") {
                    handleViewCommand(userInput, command);
                }
                return;
            }
        }
        cout << "Unfortunately, we do not have this command in our arsenal! Try 'help' for more info :)" << endl;
    }

    void validateCommand(string& userInput, string& command) {
        if (userInput.empty()) {
            cout << "Input cannot be empty!" << endl;
            return;
        }
        processCommand(userInput, command, commands);
        if (!command.empty() && command != "help" && command != "exit") {
            if (userInput.empty()) {
                cout << "Bro, enter some parameters!" << endl;
            }
        }
    }

    void help() {
        cout << "There is the list of commands you should use (Caps Lock identifies parameters you are required to use):\n";
        cout << "* check DATE FLIGHT_NUMBER: check available places for a particular flight :)\n";
        cout << "* book DATE FLIGHT_NUMBER PLACE PASSENGER: book and buy a ticket\n";
        cout << "* return TICKET_ID: return a ticket and receive money back\n";
        cout << "* view username PASSENGER: see a list of tickets bought and booked by a particular person\n";
        cout << "* view TICKET_ID: see all information about the booked ticket\n";
        cout << "* view flight DATE FLIGHT_NUMBER: view all booked tickets for a particular flight\n";
    }

    void exit(bool& exitFlag) {
        cout << "Thank you for using our service! Have a good life!" << endl;
        exitFlag = true;
    }

    void viewUsername(string& userInput) {
        if (!validateUsername()) {
            return;
        }
        removeSpaces(userInput);
        const string username = userInput;
        existing_commands.viewUsername(username);
    }

    void check(vector<string>& data) {
        if (!validateViewFlightOrCheck(data)) {
            return;
        }
        const string date = data[0];
        const string flight_number = data[1];
        existing_commands.checkOrViewFlight(date, flight_number, true);
    }

    void view(vector<string>& data) {
        if (!validateReturnView(data)) {
            return;
        }
        string id = data[0];
        existing_commands.view(id);
    }

    void book(vector<string>& data) {
        if (!validateBook(data))
        {
            return;
        }
        const string date = data[0];
        const string flight_number = data[1];
        const string place = data[2];
        string username = "";
        for (size_t i = 3; i < data.size(); i++)
        {
            username += data[i];
        }
        existing_commands.book(date, flight_number, place, username);
    }

    void Return(const vector<string>& data) {
        if (!validateReturnView(data)) {
            return;
        }
        string id = data[0];
        existing_commands.Return(id);
    }

    void viewFlight(vector<string>& data) {
        if (!validateViewFlightOrCheck(data)) {
            return;
        }
        const string date = data[0];
        const string flight_number = data[1];
        existing_commands.checkOrViewFlight(date, flight_number, false);
    }

    Commands existing_commands;

public:

    bool exit_flag = false;

    void TakeUserInput()
    {
        string command;
        cout << "Enter a command with proper info: " << endl;
        getline(cin, userInput);
        validateCommand(userInput, command);
        if (command == "help")
        {
            help();
        }
        else if (command == "exit")
        {
            exit(exit_flag);
        }
        else if (command == "view username")
        {
            viewUsername(userInput);
        }
        else {
            vector<string> data = split(userInput, ' ');
            if (command == "check")
            {
                check(data);
            }
            else if (command == "view")
            {
                view(data);
            }
            else if (command == "book")
            {
                book(data);
            }
            else if (command == "return")
            {
                Return(data);
            }
            else if (command == "view flight")
            {
                viewFlight(data);
            }
        }
        userInput.clear();
    }

    UserInput()
    {
        cout << "Welcome to the best Airflight-booking system in the world! Enjoy:)))" << endl;
        help();
        while (!exit_flag)
        {
            TakeUserInput();
        }
    }
};

int main()
{
    UserInput userInput;
}