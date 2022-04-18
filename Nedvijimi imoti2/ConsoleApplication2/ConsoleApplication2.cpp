// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <iterator> 
#include <Windows.h>
#include <list> 
#include <algorithm>
#include <iostream>

using namespace std;

enum Communication { Wather, Electricity, NearRoad, Pluming };
enum BrokerType { Helpful, Dishonest, Simple };
enum EstateType { FlatType, HouseType, PlotType };

class Estate {
private:
    EstateType type;
    string address;
    string owner;
    double price;
    int area;
public:

    Estate(EstateType type, string address, string owner, double price, int area) {
        this->type = type;
        this->address = address;
        this->owner = owner;
        this->price = price;
        this->area = area;
    }

    void setAddress(string address) { this->address = address; }
    void setOwner(string owner) { this->owner = owner; }
    void setPrice(double price) { this->price = price; }
    void setPrice(int area) { this->area = area; }
    void setType(EstateType type) { this->type = type; }

    string getAddress() { return address; }
    string getOwner() { return owner; }
    double getPrice() { return price; }
    int getArea() { return area; }
    EstateType getType() { return type; }

    virtual void Print() {
        cout << "Addres: " << this->getAddress() << " Owner: " << this->getOwner() << " Price: " << this->getPrice();
        cout << " Area: ";
        cout << this->getArea() << endl;
    }
};

class Flat : public Estate {
private:
    int rooms;
    int floor;
public:
    Flat(string address, string owner, double price, int area, int rooms, int floor) :Estate(FlatType, address, owner, price, area) {
        this->rooms = rooms;
        this->floor = floor;
    }
    virtual void Print() {
        cout << "FLAT - Addres: " << this->getAddress() << " Owner: " << this->getOwner() << " Price: " << this->getPrice();
        cout << " Area: " << this->getArea() << " rooms: " << rooms << " floor: " << floor << endl;
    }
    void setRooms(int newRooms) { rooms = newRooms; }
    void setFloor(int newFloor) { floor = newFloor; }

    int getRooms() { return rooms; }
    int getFloor() { return floor; }
};

class House : public Estate {
private:
    int rooms;
    int floors;
    int backYeardArea;
public:
    House(string address, string owner, double price, int area, int rooms, int floors, int backYeardArea) :Estate(HouseType, address, owner, price, area) {
        this->rooms = rooms;
        this->floors = floors;
        this->backYeardArea = backYeardArea;
    }
    virtual void Print() {
        cout << "HOUSE - Addres: " << this->getAddress() << " Owner: " << this->getOwner() << " Price: " << this->getPrice();
        cout << " Area: " << this->getArea() << " rooms: " << rooms << " floors: " << floors << " backYeardArea: " << backYeardArea << endl;
    }
    void setRooms(int newRooms) { rooms = newRooms; }
    void setFloors(int newFloors) { floors = newFloors; }
    void setbackYeardArea(int newBackYeardArea) { backYeardArea = newBackYeardArea; }

    int getRooms() { return rooms; }
    int getFloors() { return floors; }
    int getbackYeardArea() { return backYeardArea; }

};

class Plot : public Estate {
private:
    list<Communication> communications;
public:
    Plot(string address, string owner, double price, int area, list<Communication> communications) :Estate(PlotType, address, owner, price, area) {
        this->communications = communications;
    }
    list<Communication> getCommunications() {
        return communications;
    }
    void setCommunications(list<Communication> comm) {
        communications = comm;
    }
};

class RealEstates {
public:
    list<unique_ptr<Estate>> estates;
    void printEstates() {
        cout << "Full list of estates: " << endl;
        for (const auto& estate : estates)
        {
            estate->Print();
        }
    }
};


class Broker {
private:
    string name;
    double percentage;
    BrokerType Type;
public:
    RealEstates estates;
    Broker(RealEstates& estates, BrokerType Type, string name, double percentage) {
        this->estates = move(estates);
        this->Type = Type;
        this->name = name;
        this->percentage = percentage;
    }
    virtual void findEstate(int typeSearch) = 0;
    void setType(BrokerType type) {
        this->Type = type;
    }
    static void printResult(list<unique_ptr<Estate>>& result) {
        cout << "Results from search:" << endl;
        for (const auto& temp : result)
        {
            temp->Print();
        }
    }
};


class SimpleBroker : public Broker {
public:
    SimpleBroker(RealEstates& estates, string name, double percentage) : Broker(estates, Simple, name, percentage) {
    }
    void findEstate(int typeSearch) {
        if (typeSearch == 1) {
            this->findByAdddress();
        }
        if (typeSearch == 2) {
            this->findByPrice();
        }
    }

    void findByAdddress() {
        cin.clear();
        string address;
        cout << "Enter address:" << endl;
        getline(cin, address);

        list<unique_ptr<Estate>> temp;
        copy_if(make_move_iterator(this->estates.estates.begin()), make_move_iterator(this->estates.estates.end()), back_inserter(temp),
            [&](const auto& p) { return (p->getAddress() == address); });
        Broker::printResult(temp);
    }
    void findByPrice() {
        int priceFrom, priceTo;
        cout << "With price from:" << endl;
        cin >> priceFrom;
        cout << "With price to:" << endl;
        cin >> priceTo;

        list<unique_ptr<Estate>> temp;
        copy_if(make_move_iterator(this->estates.estates.begin()), make_move_iterator(this->estates.estates.end()), back_inserter(temp),
            [&](const auto& p) { return (p->getPrice() > priceFrom && p->getPrice() < priceTo); });
        Broker::printResult(temp);
    }
};

class HelpfulBroker : public Broker {
private:
    list<unique_ptr<Estate>> vip;
public:
    HelpfulBroker(RealEstates& estates, string name, double percentage) : Broker(estates, Helpful, name, percentage) {
    }
    void findEstate(int typeSearch) {
    }
};

class DishonestBroker : public Broker {
private:
    list<unique_ptr<Estate>> vip;
public:
    DishonestBroker(RealEstates& estates, string name, double percentage) : Broker(estates, Dishonest, name, percentage) {
    }
    void findEstate(int typeSearch) {
    }
};

int main()
{
    RealEstates realEstates;
    realEstates.estates.push_back(make_unique<Flat>("Varna", "Ivan Ivanov", 5678.56, 47, 4, 7));
    realEstates.estates.push_back(make_unique<House>("Varna", "Victor Ivanov", 78463.99, 47, 7, 2, 45));
    realEstates.estates.push_back(make_unique<Flat>("Sofia", "Petyr Petrov", 1356.55, 23, 3, 3));

    realEstates.printEstates();
    cout << "______________________________________________" << endl;

    SimpleBroker broker(realEstates, "Mimi", 10.2);


    cout << "1 - Search for estate by address." << endl;
    cout << "2 - Search for estate with price between." << endl;

    int option = 0;
    cout << "Enter option: ";
    cin >> option;
    cin.ignore();
    switch (option) {
    case 1:
        broker.findEstate(1);
        break;
    case 2:
        broker.findEstate(2);
        break;
    }

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
