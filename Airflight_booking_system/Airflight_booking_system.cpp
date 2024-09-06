#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Ticket
{

    const string number_of_seat;
    bool booked;

public:
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
    const string date;
    const string flight_number;
    vector<Ticket> tickets;

    void getSeatNumbers()  
    {
        seat_numbers.clear();
        for (int i = 0; i < number_of_seats_per_row; i++)
        {
            seat_numbers.push_back('A' + i);
        }
    }
    void generateTickets() 
    {
        for (int i = 0; i < number_of_seats_per_row; i++)
        {
            for (int j = 0; j < seat_numbers.size(); j++)
            {
                string ticket_number = to_string(i) + seat_numbers[j];
                Ticket ticket(ticket_number, false);
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
    map<vector<int>, int> prices;
       
    Airplane(string day, string flight, int number_seats) : number_of_seats_per_row(number_seats), date(day), flight_number(flight)
    {
        getSeatNumbers();
        generateTickets();
    }

    void displayAirplane() const
    {
        cout << "Info about airplane " << flight_number << ":\n"
         << "* date: " << date << "\n"
        << "* number of seats per row: " << number_of_seats_per_row << "\n"
        << "* prices for rows: " ;
        PrintPrices();

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

    void MakeTableOfPrices(vector<string> range_and_prices, map<vector<int>,int> &prices) 
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
            MakeTableOfPrices(range_and_prices, airplane.prices);
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

class UserInput
{

};
class Commands
{
    string command;

public:
    void ChooseCommand() const
    {
        if (command == "check")
        {
            
        }
        else if (command == "view")
        {

        }
        else if (command == "book")
        {

        }
        else if (command == "return")
        {

        }
        else if (command == "passenger")
        {

        }
        else if (command == "flight")
        {

        }
        else if (command == "help")
        {

        }
        else
        {
            cout << "Unfortunately, we do not have this command in our arcenal! Try 'help' for more info:)";
            return;
        }
    }
};
class ProgramEngine
{
    const FileReader fileReader;
    void function() const
    {
        vector<Airplane> airplanes = fileReader.GetAirplanes();
        for (int i = 0; i < airplanes.size(); i++)
        {
            airplanes[i].displayAirplane();
        }
    }
public:
    ProgramEngine()
    {
        function();
    }
};

int main()
{
    const ProgramEngine engine;

    cout<<"Helllo"<<endl;
}
