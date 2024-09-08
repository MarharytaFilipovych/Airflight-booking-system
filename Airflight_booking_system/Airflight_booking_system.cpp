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
    bool booked = false;
    string place = "";
    string owner = "";
    int price = 0;
    string flight = "";
    string date = "";

    Ticket() = default;

    Ticket(string number, bool status, int price_for_ticket, string date_of_flight, string flight_number)
        : place(number), booked(status), price(price_for_ticket), flight(flight_number), date(date_of_flight)
    {
    }

    Ticket& operator=(const Ticket& other) {
        if (this == &other) {
           return *this;
        }
        this->price = other.price;
        this->place = other.place;
        this->booked = other.booked;
        this->owner = other.owner;
        this->flight = other.flight;
        this->date = other.date;
        return *this;
    }

    void displayTicket() const
    {
        if (booked)
        {
            cout << place << ": booked, price: " << price << endl;
        }
        else
        {
            cout << place << ": free, price: " << price << endl;
        }
    }
};


class Airplane
{
    const int number_of_seats_per_row;
    vector<char> seats;
    const map<vector<int>, int> prices;
    const int number_of_rows;
    void getSeatNumbers()
    {
        seats.clear();
        for (int i = 0; i < number_of_seats_per_row; i++)
        {
            seats.push_back('A' + i);
        }
    }
    int findTicketPrice(const string& ticket_number) const
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
            for (int j = 0; j < seats.size(); j++)
            {
                string ticket_number = to_string(i) + seats[j];
                int price = findTicketPrice(ticket_number);
                Ticket ticket(ticket_number, false, price, flight_number, date);
                tickets.push_back(ticket);
            }
        }
    }

    void displayTickets() const
    {
        for (int i = 0; i < tickets.size(); i++)
        {
            tickets[i].displayTicket();

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
    vector<Ticket> tickets;

    Airplane(string day, string flight, int number_seats, map<vector<int>, int> prices_for_rows, int rows) : number_of_seats_per_row(number_seats), date(day), flight_number(flight), prices(prices_for_rows), number_of_rows(rows) {
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
};

class Passenger
{
    const string name;
    unordered_set<int> id_tickets;  
    
public:
    Passenger(string username): name(username)
    {
    }
    void addTicket(const int& id_ticket)
    {
        id_tickets.insert(id_ticket);
    }
    bool operator==(const Passenger& other) const
    {
        return  name == other.name;
    }
    bool operator<(const Passenger& other) const {
        return name < other.name;
    }
    
};


class FileReader
{
    const string fileName = "C:\\Margo\\Uni\\Airflight_booking_system\\Airflight_booking_system\\flights.txt";

    vector<Airplane> airplanes;

    void MakeTableOfPrices(vector<string> range_and_prices, map<vector<int>, int>& prices, int& number_of_rows)
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
        airplanes.push_back(airplane);
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
    const vector<Airplane>& GetAirplanes() const
    {
        return airplanes;
    }

};
class ID
{
    unordered_set<int> IDs;
    const int N = 10000;
public:

    int generateID() {
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
    vector<Airplane> airplanes = fileReader.GetAirplanes();
    set<Passenger> passengers;  
    unordered_map<int, Ticket> bought_tickets;
    ID id_generator;
    void addPassengers(const Passenger& passenger)
    {
        passengers.insert(passenger); 
    }  
public:
    void Check(const string& date, const string& flight_number) const
    {
        for (const Airplane& airplane : airplanes)
        {
            if (airplane.flight_number == flight_number && airplane.date == date)
            {
                for (const Ticket& ticket : airplane.tickets)
                {
                    if (!ticket.booked)
                    {
                        cout << ticket.place << ":" << to_string(ticket.price) + "$" << " ";
                    }
                }
            }
        }
        cout << endl;
    }

    void Book(const string& date, const string& flight_number, const string& place, const string& username)
    {
        Passenger passenger(username);
        addPassengers(passenger);
        for (const Airplane& airplane : airplanes)
        {
            if (airplane.flight_number == flight_number && airplane.date == date)
            {
                for ( Ticket ticket : airplane.tickets)
                {
                    if (ticket.place == place)
                    {
                        if (ticket.booked)
                        {
                            cout << "Sorry, this place is already booked!" << endl;
                            break;
                        }
                        else
                        {
                            int id = id_generator.generateID();
                            passenger.addTicket(id);
                            cout << "Confirmed with ID " << id << endl;
                            ticket.booked = true;
                            ticket.owner = username;
                            bought_tickets[id] = ticket;
                            break;
                        }
                    }
                }
            }
        }
    }

    void Return(int id)
    {
        auto it = bought_tickets.find(id);
        if (it != bought_tickets.end())
        {
            Ticket& ticket = it->second;
            
            cout << "Confirmed " << ticket.price << "$ refund for " << ticket.owner << endl;
            ticket.booked = false;
            ticket.owner = ""; 
            bought_tickets.erase(it);
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

};
class UserInput
{
    string userInput;
    const list<string> commands = { "check", "view", "book", "return", "help", "exit", "view username", "view flight" };
    vector<string> split(string& data, char delimiter)
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

    bool validateReturnView(vector<string>& data)
    {
        if (data.size() != 1 || !isDigit(data[0]))
        {
            cout << "Invalid input!" << endl;
            return false;
        }
        return true;

    }
    bool validateFlightNumber(const string& number)
    {
        if (number.length() != 5 || !isalpha(number[0]) || !isalpha(number[1]) || !isupper(number[0]) || !isupper(number[1]) || !isdigit(number[2]) || !isdigit(number[3]) || !isdigit(number[4])) {
            cout << "The number of your flight doesn't look correct:(" << endl;
            return false;
        }
        return true;
    }
    bool validateDate(string date)
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
    bool validatePlace(string& place)
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
    bool validateBook(vector<string>& data)
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
    bool validateViewFlightOrCheck(vector<string>& data)
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
    bool validateUsername()
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
    Commands command_from_class;
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
            const string username = userInput;
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
                command_from_class.Check(date, flight_number);

            }
            else if (command == "view")
            {
                if (!validateReturnView(data))
                {
                    return;
                }
                const int id = stoi(data[0]);
                command_from_class.View(id);
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
                    username = username + " " + data[i];
                }
                command_from_class.Book(date, flight_number, place, username);


            }
            else if (command == "return")
            {
                if (!validateReturnView(data))
                {
                    return;
                }
                int id = stoi(data[0]);
                command_from_class.Return(id);
            }
            else if (command == "view flight")
            {
                if (!validateViewFlightOrCheck(data))
                {
                    return;
                }
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