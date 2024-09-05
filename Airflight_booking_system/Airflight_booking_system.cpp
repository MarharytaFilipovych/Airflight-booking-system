#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Ticket
{

    string passenger_name;
    char number_of_seat;
    bool book_status;
    Ticket(string name, char number, bool status) :
        passenger_name(name), number_of_seat(number), book_status(status)
    {
    }
};
class Airplane
{
    int number_of_seats_per_row;
    vector<char> seat_numbers;
    string date;
    string flight_number;

    void getSeatNumbers()
    {
        seat_numbers.clear();
        for (int i = 0; i < number_of_seats_per_row; i++)
        {
            seat_numbers.push_back('A' + i);
        }
    }
    

    void PrintPrices()
    {
        for (auto i = prices.begin(); i != prices.end(); i++)
        {
            cout << "rows: " << i->first.front() << "-" << i->first.back();
            cout << " -> " << i->second << "$" << endl;
        }
    }

public:
    map<vector<int>, int> prices;
       
    Airplane(string day, string flight, int number_seats) : number_of_seats_per_row(number_seats), date(day), flight_number(flight)
    {
        getSeatNumbers();
        cout << date << " " << flight_number << " " << number_of_seats_per_row << endl;


    }
    void displayAirplane()
    {
        cout << "Date: " << date << ";\n"
            << "Flight number: " << flight_number << ";\n"
        << "Flight number: " << flight_number << ";\n"
        << "Number of seats per row: " << number_of_seats_per_row << ";\n"
        << "Prices for rows: " ;
        PrintPrices();

    }

};

class FileReader
{
    string fileName = "C:\\Margo\\Uni\\Airflight_booking_system\\Airflight_booking_system\\flights.txt";
    
    vector<Airplane> airplanes;

    void  MakeTableOfPrices(vector<string> range_and_prices, map<vector<int>,int> &prices)
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

};
class ProgramEngine
{
    FileReader fileReader;
};

int main()
{
    
    cout<<"Helllo"<<endl;
}
