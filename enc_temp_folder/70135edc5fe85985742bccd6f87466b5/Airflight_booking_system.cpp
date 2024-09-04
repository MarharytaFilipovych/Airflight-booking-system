#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


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
        map<int, int>::iterator it = prices.begin();
        while (it != prices.end())
        {
            cout << it->first << " " << it->second << endl;
        }
    }

public:

    map<int, int> prices;

    Airplane(string day, string flight, int number_seats) : number_of_seats_per_row(number_seats), date(day), flight_number(flight)
    {
        getSeatNumbers();
        cout << date << " " << flight_number << " " << number_of_seats_per_row << endl;


    }

};

class FileReader
{
    string fileName;
    void MakeTableOfPrices(vector<string> range_and_prices, map<int, int> prices)
    {
        for (int i = 0; i < range_and_prices.size(); i += 2)
        {
            string range = range_and_prices[i];

            int dash = range.find('-');
            if (dash == string::npos) {
                cout << "Invalid data format! " << endl;
                return;
            }
            int start = stoi(range.substr(0, dash));
            int end = stoi(range.substr(dash + 1));

            string price_with_dollar = range_and_prices[i + 1];
            int dollar = price_with_dollar.find('$');
            if (dollar == string::npos) {
                cout << "Invalid data format! " << endl;
                return;
            }
            int price = stoi(price_with_dollar.substr(0, dollar));
            for (int i = start; i <= end; i++)
            {
                prices[i] = price;
            }

        }
    }

public:
    vector<Airplane> airplanes;

    FileReader(string file) : fileName(file)
    {

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

            if (range_and_prices.size() % 2 != 0)
            {
                cout << "Invalid data format for range and prices!" << endl;
                continue;
            }

            MakeTableOfPrices(range_and_prices, airplane.prices);
            airplanes.push_back(airplane);

            number_of_records--;
        }

        cout << "The file was read successfully!" << endl;
        file.close();
    }


};


/*
class Ticket
{

    string passenger_name;
    char number_of_seat;
    bool book_status;
    Ticket(string name, char number, bool status, string day, string flight, int price):
        passenger_name(name), number_of_seat(number), book_status(status), date(day), flight_number(flight), price_of_ticket(price)
        {
        }



};*/
class UserInput
{

};
class Commands
{

};
int main()
{
    string fileName = "C:\\Margo\\Uni\\Airflight_booking_system\\Airflight_booking_system\\flights.txt";
    FileReader fileReader(fileName);
    fileReader.ReadFile();
    cout << "Helllo" << endl;
}
