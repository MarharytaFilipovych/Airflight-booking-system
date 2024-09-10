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


    Ticket& operator=(const Ticket& other) {
        if (this == &other) {
           return *this;
        }
        this->price = other.price;
        this->place = other.place;
        this->owner = other.owner;
        this->flight = other.flight;
        this->date = other.date;
        return *this;
    }
    bool operator<(const Ticket& other) const {
        if (price == other.price) {
            return place < other.place;  
        }
        return price < other.price;  
    }
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
                Ticket ticket(ticket_number, price, flight_number, date);
                tickets.insert(ticket);
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
public:

    const string date;
    const string flight_number;
    set<Ticket> tickets;
    set<Ticket> booked_tickets;

    Airplane() = default;

    Airplane(const string& day, const string& flight, const int number_seats, const map<vector<int>, int>& prices_for_rows, const int rows) : number_of_seats_per_row(number_seats), date(day), flight_number(flight), prices(prices_for_rows), number_of_rows(rows) {
        getSeatNumbers();
        generateTickets();
    }

    void displayAirplane() const
    {
        cout << "Info about airplane " << flight_number << ":\n"
            << "* date: " << date << "\n"
            << "* number of seats per row: " << number_of_seats_per_row << "\n"
            << "* prices for rows: ";
        PrintPrices();
    }  
    bool operator==(const Airplane& airplane) const
    {
        return date == airplane.date &&
            flight_number == airplane.flight_number &&
            number_of_seats_per_row == airplane.number_of_seats_per_row &&
            number_of_rows == airplane.number_of_rows &&
            prices == airplane.prices;
    }


    struct HashFunction
    {
        size_t operator()(const Airplane& airplane) const
        {

            size_t hash_value = 17;
            hash_value = hash_value * 31 + std::hash<std::string>()(airplane.date);
            hash_value = hash_value * 31 + std::hash<std::string>()(airplane.flight_number);
            hash_value = hash_value * 31 + std::hash<int>()(airplane.number_of_seats_per_row);
            hash_value = hash_value * 31 + std::hash<int>()(airplane.number_of_rows);

            return hash_value;

        }
    };
    Airplane(const Airplane& other)
        : number_of_seats_per_row(other.number_of_seats_per_row),
        seats(other.seats),
        prices(other.prices),
        number_of_rows(other.number_of_rows),
        date(other.date),
        flight_number(other.flight_number),
        tickets(other.tickets),
        booked_tickets(other.booked_tickets)
    {
    }

    Airplane& operator=(const Airplane& other)
    {
        if (this != &other)
        {
            seats = other.seats;
            prices = other.prices;
            tickets = other.tickets;
            booked_tickets = other.booked_tickets;
        }
        return *this;
    }

};



class FileReader
{
    const string fileName = "C:\\Margo\\Uni\\Airflight_booking_system\\Airflight_booking_system\\flights.txt";

    unordered_map<string,Airplane> airplanes;

    void MakeTableOfPrices(vector<string>& range_and_prices, map<vector<int>, int>& prices, int& number_of_rows) const
    {
        for (int i = 0; i < range_and_prices.size(); i += 2)
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
    void readFile()
    {
        ifstream file(fileName);
        if (!file.is_open())
        {
            cout << "Your file " << fileName << " could not be opened!" << endl;
            return;
        }
        int number_of_records;
        file >> number_of_records;
        file.ignore();
        string line;
        while (number_of_records > 0)
        {
            if (!getline(file, line) || line.empty())
            {
                continue;
            }
            istringstream this_line(line);

            createPlanes(this_line);
            number_of_records--;
        }
        file.close();
    }


public:

    FileReader()
    {
        readFile();
    }
    const unordered_map<string,Airplane>& GetAirplanes() const
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
    unordered_map<string,Airplane> airplanes = fileReader.GetAirplanes();
    unordered_map<int, Ticket> bought_tickets;
    ID id_generator;
    unordered_map<string, unordered_set<int>> passengers;
    
public:
    void checkOrViewFlight(const string& date, const string& flight_number, bool check) const
    {
        string key = date + flight_number;
        auto it = airplanes.find(key);
        if (it != airplanes.end())
        {
            const Airplane& airplane = it->second;
            if (check)
            {
                for (const Ticket& ticket : airplane.tickets)
                {
                    cout << ticket.place << ":" << to_string(ticket.price) + "$" << " ";
                }
            }
            else
            {
                for (const Ticket& ticket : airplane.booked_tickets)
                {
                    cout << ticket.place << ", " << ticket.owner << to_string(ticket.price) + "$" << " ";
                }
            }           
            cout << endl;  
        }
        else
        {
            cout << "Flight not found!" << endl;
        }
    }

    void Book(const string& date, const string& flight_number, const string& place, const string& username)
    {
        bool found_place = false;
        string key = date + flight_number;
        auto airplane_it = airplanes.find(key);
        if (airplane_it != airplanes.end())
        {
            Airplane& airplane = airplane_it->second; 
            auto it = airplane.tickets.begin();
            while (it != airplane.tickets.end())
            {
                if (it->place == place)
                {
                    found_place = true;
                    int id = id_generator.generateID();
                    passengers[username].insert(id);
                    cout << "Confirmed with ID " << id << endl;
                    Ticket ticket = *it;
                    ticket.owner = username;
                    bought_tickets[id] = ticket;
                    airplane.booked_tickets.insert(ticket);
                    airplane.tickets.erase(it);
                    return;
                }
                else
                {
                    it++;
                }
            }
            if (!found_place)
            {
                cout << "Sorry, this place is already booked!" << endl;
                return;
            }
        }
        else
        {
            cout << "Flight not found!" << endl;
        }
    }

    void Return(int id)
    {
        auto it = bought_tickets.find(id);
        if (it != bought_tickets.end())
        {
            Ticket& ticket = it->second;
            string key = ticket.date + ticket.flight;

            cout << "Confirmed " << ticket.price << "$ refund for " << ticket.owner << endl;

            auto it_user = passengers.find(ticket.owner);
            if (it_user != passengers.end())
            {
                unordered_set<int>& ids = it_user->second;
                ids.erase(id);
                if (ids.empty())
                {
                    passengers.erase(it_user);
                }
            }

            ticket.owner = "";
            bought_tickets.erase(it);

            auto airplane_it = airplanes.find(key);
            if (airplane_it != airplanes.end())
            {
                Airplane& airplane = airplane_it->second; 
                airplane.booked_tickets.erase(ticket);
                airplane.tickets.insert(ticket);
            }
        }
        else
        {
            cout << "Hmm... There is no id " << id << ". Therefore, no money back))))" << endl;
        }
    }

    void View(int id)
    {
        auto it = bought_tickets.find(id);
        if (it != bought_tickets.end())
        {
           Ticket& ticket = it->second;
           cout << "Flight " << ticket.flight << ", " << ticket.date << ", " << ticket.place << ", " << ticket.owner << endl;        
        }
        else
        {
            cout << "The ticket with id " << id << " was not booked." << endl;
        }
    }

    void viewUsername(const string& username)
    {
        auto it = passengers.find(username);
        if (it != passengers.end())
        {
            const unordered_set<int>& ids = it->second;
            int i = 1;
            for (const int id : ids)
            {
                cout << i << ". ";
                View(id);
            }
        }
        else
        {
            cout << "Passenger not found." << endl;
        }
    }

    
};
class UserInput
{
    string userInput;
    const unordered_set<string> commands = { "check", "view", "book", "return", "help", "exit", "view username", "view flight" };
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
    bool validateDate(string& date) const
    {
        if (date.length() != 10 || date[2] != '.' || date[5] != '.')
        {
            cout << "The format of the data is incorrect!" << endl;
            return false;
        }
        vector<string> parts = split(date, '.');
        if (!isDigit(parts[0]) || !isDigit(parts[1]) || !isDigit(parts[2]))
        {
            cout << "The format of the data is incorrect!" << endl;
            return false;
        }
        int day = stoi(parts[0]);
        int month = stoi(parts[1]);
        int year = stoi(parts[2]);
        if (month > 12 || month < 1 || year < 2000)
        {
            cout << "Incorrect date numbers!" << endl;
            return false;
        }
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
    bool validateBook( vector<string>& data) const
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
    
        for (int i = 3; i < data.size(); i++) {
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
        if (!validateDate(data[0]) || !validateFlightNumber(data[1]) )
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
    void validateCommand(string& command) 
    {
        bool commandFound = false;
        list<string> type_view = { "username", "flight" };
        for (const auto& cmd : commands)
        {
            if (userInput.rfind(cmd, 0) == 0)
            {
                command = cmd;
                if (userInput.length() > cmd.length())
                {            
                    userInput.erase(0, cmd.length() + 1);
                    if (command == "view")
                    {
                        for(const auto& view_t: type_view)
                        {
                            if (userInput.rfind(view_t, 0) == 0)
                            {
                                command = command + " " + view_t;
                                userInput.erase(0, view_t.length() + 1);
                            }                            
                        }                       
                    }
                }
                else
                {
                    if (command != " help" && command != "exit")
                    {
                        cout << "Bro, enter some parameters!" << endl;
                    }
                }
                commandFound = true;
                break;
            }
        }
        if (!commandFound)
        {
            cout << "Unfortunately, we do not have this command in our arsenal! Try 'help' for more info :)" << endl;
            return;
        }
    }
    Commands existing_commands;
public:
    bool exit = false;

    void TakeUserInput()
    {
        string command;
        cout << "Enter a command with proper info: " << endl;
        getline(cin, userInput);
        validateCommand(command);
        if (command == "help")
        {
            cout << endl;
            return;
        }
        else if (command == "exit")
        {
            cout << "Thank you for using our service! Have a good life!" << endl;
            exit = true;
        }
        else if (command == "view username")
        {
            if (!validateUsername())
            {
                return;
            }
            removeSpaces(userInput);
            const string username = userInput;
            existing_commands.viewUsername(username);
        }
        else {
            vector<string> data = split(userInput, ' ');
            if (command == "check")
            {
                if (!validateViewFlightOrCheck(data))
                {
                    return;
                }
                const string date = data[0];
                const string flight_number = data[1];
                existing_commands.checkOrViewFlight(date, flight_number, true);

            }
            else if (command == "view")
            {
                if (!validateReturnView(data))
                {
                    return;
                }
                const int id = stoi(data[0]);
                existing_commands.View(id);
            }
            else if (command == "book")
            {
                if (!validateBook(data))                   
                {
                    return;
                }
                const string date = data[0];
                const string flight_number = data[1];
                const string place = data[2];
                string username = "";
                for (int i = 3; i < data.size(); i++)
                {
                    username += data[i];
                }
                existing_commands.Book(date, flight_number, place, username);


            }
            else if (command == "return")
            {
                if (!validateReturnView(data))
                {
                    return;
                }
                int id = stoi(data[0]);
                existing_commands.Return(id);
            }
            else if (command == "view flight")
            {
                if (!validateViewFlightOrCheck(data))
                {
                    return;
                }
                const string date = data[0];
                const string flight_number = data[1];
                existing_commands.checkOrViewFlight(date, flight_number, false);

            }
        }
        userInput.clear();
    }
};

class ProgramEngine
{
    UserInput userInput;

public:
    ProgramEngine()
    {
        while (!userInput.exit)
        {
            userInput.TakeUserInput();
        }
    }
};

int main()
{
    const ProgramEngine engine;
}