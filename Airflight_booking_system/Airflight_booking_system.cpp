#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
using namespace std;

class Ticket
{


public:
    bool booked;
    const string number_of_seat;


    Ticket(string number, bool status) :
        number_of_seat(number), booked(status)
    {
    }
    void displayTicket() const
    {
        if (booked)
        {
            cout << number_of_seat << ": booked" << endl;
        }
        else
        {
            cout << number_of_seat << ": free" << endl;

        }
    }
};
class Airplane
{
    const int number_of_seats_per_row;
    vector<char> seat_numbers;
    map<vector<Ticket>, int> prices_for_tickets;

    void getSeatNumbers()
    {
        seat_numbers.clear();
        for (int i = 0; i < number_of_seats_per_row; i++)
        {
            seat_numbers.push_back('A' + i);
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
    map<vector<int>, int> prices;
    const string date;
    const string flight_number;
    vector<Ticket> tickets;

    int number_of_rows;

    Airplane(string day, string flight, int number_seats) : number_of_seats_per_row(number_seats), date(day), flight_number(flight)
    {
        getSeatNumbers();
    }
    void generateTickets()
    {
        for (int i = 1; i < number_of_rows; i++)
        {
            for (int j = 0; j < seat_numbers.size(); j++)
            {
                string ticket_number = to_string(i) + seat_numbers[j];
                Ticket ticket(ticket_number, false);
                tickets.push_back(ticket);

            }

        }
    }
    void displayAirplane() const
    {
        cout << "Info about airplane " << flight_number << ":\n"
            << "* date: " << date << "\n"
            << "* number of seats per row: " << number_of_seats_per_row << "\n"
            << "* prices for rows: ";
        PrintPrices();

    }
    string findTicketPrice(const Ticket& ticket) const
    {
        const int tikcet_row = stoi(ticket.number_of_seat.substr(0, ticket.number_of_seat.length() - 1));

        for (const auto& price_itself : prices)
        {
            if (tikcet_row >= price_itself.first.front() && tikcet_row <= price_itself.first.back())
            {
                return to_string(price_itself.second) + "$";
            }

        }
    }

    void findPriceAndListOfFreeTickets()
    {
        for (int j = 0; j < tickets.size(); j++)
        {
            if (!tickets[j].booked)
            {
                string price = findTicketPrice(tickets[j]);
                cout << tickets[j].number_of_seat << ":" << price << " ";

            }
        }

    }


};

class Passenger
{
    const string name;
    vector<string> tickets;
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
    void ReadFile()
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
            string date, flight_number;
            int seats_per_row;

            this_line >> date >> flight_number >> seats_per_row;

            Airplane airplane(date, flight_number, seats_per_row);

            vector<string> range_and_prices;
            string range_and_price;
            while (this_line >> range_and_price)
            {
                range_and_prices.push_back(range_and_price);
            }
            MakeTableOfPrices(range_and_prices, airplane.prices, airplane.number_of_rows);
            airplane.generateTickets();
            airplanes.push_back(airplane);

            number_of_records--;
        }
        cout << "The file was read successfully!" << endl;
        file.close();
    }
public:

    FileReader()
    {
        ReadFile();
    }
    const vector<Airplane>& GetAirplanes() const
    {
        return airplanes;
    }

};
class Commands
{
    const FileReader fileReader;
    //string command;   

public:
    void Check(string date, string flight_number)
    {
        vector<Airplane> airplanes = fileReader.GetAirplanes();
        for (int i = 0; i < airplanes.size(); i++)
        {
            if (airplanes[i].flight_number == flight_number && airplanes[i].date == date)
            {
                airplanes[i].findPriceAndListOfFreeTickets();
            }
        }
        cout << endl;
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

    void validateReturnView(vector<string>& data)
    {
        if (data.size() != 1 || !isDigit(data[0]))
        {
            cout << "Invalid input!" << endl;
            return;
        }
    }
    void validateFlightNumber(const string& number)
    {
        if (number.length() != 5 || !isalpha(number[0]) || !isalpha(number[1]) || !isupper(number[0]) || !isupper(number[1]) || !isdigit(number[2]) || !isdigit(number[3]) || !isdigit(number[4])) {
            cout << "The number of your flight doesn't look correct:(" << endl;
            return;
        }
    }
    void validateDate(string date)
    {
        if (date.length() != 10 || date[2] != '.' || date[5] != '.')
        {
            cout << "The format of the data is incorrect!" << endl;
            return;
        }
        vector<string> parts = split(date, '.');
        if (!isDigit(parts[0]) || !isDigit(parts[1]) || !isDigit(parts[2]))
        {
            cout << "The format of the data is incorrect!" << endl;
            return;
        }
        int day = stoi(parts[0]);
        int month = stoi(parts[1]);
        int year = stoi(parts[2]);
        if (month > 12 || month < 1 || year < 2000)
        {
            cout << "Incorrect date numbers!" << endl;
            return;
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
            return;
        }

    }
    void validatePlace(string& place)
    {
        if (!isdigit(place[0] || !isalpha(place[1]) || !isupper(place[1])))
        {
            cout << "There is no such place in the airplane :(" << endl;
            return;
        }
    }
    void validateBook(vector<string>& data)
    {
        if (data.size() < 4)
        {
            cout << "Incorrrect data for book command :(" << endl;
            return;
        }
        validateDate(data[0]);
        validateFlightNumber(data[1]);
        validatePlace(data[2]);
        for (int i = 3; i < data.size(); i++) {
            if (!isLetters(data[i])) {
                cout << "You name should only contain letters, shouldn't it?" << endl;
                return;
            }
        }

    }
    void validateViewFlightOrCheck(vector<string>& data)
    {
        if (data.size() != 2)
        {
            cout << "Incorrrect data for book command :(" << endl;
            return;
        }
        validateDate(data[0]);
        validateFlightNumber(data[1]);
    }
    void validateUsername()
    {
        if (!isLetters(userInput))
        {
            cout << "You name should only contain letters, shouldn't it?" << endl;
            return;
        }
    }
    void validateCommand(string& command)
    {
        for (const auto& cmd : commands)
        {
            if (userInput.rfind(cmd, 0) == 0)
            {
                command = cmd;
                if (userInput.length() > cmd.length() + 1)
                {
                    userInput.erase(0, cmd.length() + 1);

                }
                else
                {
                    cout << "Bro, enter some parameters!" << endl;
                    return;
                }
                break;
            }
            else
            {
                cout << "Unfortunately, we do not have this command in our arcenal! Try 'help' for more info:)";
                return;
            }
        }
    }
    Commands command_from_class;
public:

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
            exit;
        }
        else if (command == "view username")
        {
            validateUsername();
            const string username = userInput;
        }
        else {
            vector<string> data = split(userInput, ' ');
            if (command == "check")
            {
                validateViewFlightOrCheck(data);
                const string date = data[0];
                const string flight_number = data[1];
                command_from_class.Check(date, flight_number);

            }
            else if (command == "view")
            {
                validateReturnView(data);
                int id;

            }
            else if (command == "book")
            {
                validateBook(data);

            }
            else if (command == "return")
            {
                validateReturnView(data);
                int id;

            }
            else if (command == "view flight")
            {
                validateViewFlightOrCheck(data);
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
        userInput.TakeUserInput();
    }
};

int main()
{
    const ProgramEngine engine;

}
