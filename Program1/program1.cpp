/*
Name: Aidan Lam
Description: Reads a line of foobars from a file, determines each foobar's strength based on its type and position, and writes the results to an output file.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Represents an ordinary Foobar and stores its name and position.
class Foobar {
private:
    std::string name;
    int position;

protected:
    int getPosition() const {
        return position;
    }

public:
    Foobar(const std::string& name) : name(name), position(0) {}

    virtual ~Foobar() = default;
    
    void setPosition(int newPosition) {
        position = newPosition;
    }

    virtual int getStrength() const {
        return getPosition();
    }

    const std::string& getName() const {
        return name;
    }
};

// Represents a Foo, whose strength is three times its position.
class Foo : public Foobar {
public:
    Foo(const std::string& name) : Foobar(name) {} 

    int getStrength() const override {
        return 3 * getPosition();
    }
};

// Represents a Bar, whose strength is 15 greater than its position.
class Bar : public Foobar {
public:
    Bar(const std::string& name) : Foobar(name) {}

    int getStrength() const override {
        return getPosition() + 15;
    }
};

// Reads the input file and creates Foobar objects based on each line.
void readInputFile(const std::string& filename, std::vector<Foobar*>& foobars) {
    std::ifstream inputFile(filename);
    std::string line;

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string type, name;
        ss >> type >> name;

        if (type == "foo") {
            foobars.push_back(new Foo(name));
        } else if (type == "bar") {
            foobars.push_back(new Bar(name));
        } else {
            foobars.push_back(new Foobar(name));
        }
    }  
}

// Assigns positions to the Foobar objects in reverse order.
void assignPositions(std::vector<Foobar*>& foobars) {
    for (std::size_t i = foobars.size(); i-- > 0;) {
        foobars[i]->setPosition(static_cast<int>(foobars.size() - i));
    }
}

// Writes each Foobar's name and strength to the output file.
void writeOutputFile(const std::string& filename, const std::vector<Foobar*>& foobars) {
    std::ofstream outputFile(filename);

    for (const Foobar* foobar : foobars) {
        outputFile << foobar->getName() << " " << foobar->getStrength() << "\n";
    }
}

// Gets filenames, reads inputs, assigns positions, and writes outputs.
int main() {
    std::string inputFilename;
    std::string outputFilename;
    std::vector<Foobar*> foobars;

    std::cout << "Enter the name of the input file: ";
    std::cin >> inputFilename;

    std::cout << "Enter the name of the output file: ";
    std::cin >> outputFilename;

    readInputFile(inputFilename, foobars);
    assignPositions(foobars);
    writeOutputFile(outputFilename, foobars);

    for (Foobar* foobar : foobars) {
        delete foobar;
    }
    
    return 0;
}