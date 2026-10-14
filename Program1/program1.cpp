/*
Name: Aidan Lam
Description: 
*/

#include <fstream>
#include <vector>
#include <string>

class Foobar {
private:
    std::string name;
    int position;

protected:
    int getPosition() const {
        return position;
    }

public:
    Foobar(const std::string& n) : name(n), position(0) {}
    
    void setPosition(int p) {
        position = p;
    }

    virtual int getStrength() const {
        return getPosition();
    }

    std::string getName() const {
        return name;
    }
};

class Foo : public Foobar {
public:
    Foo(const std::string& n) : Foobar(n) {} 

    int getStrength() const override {
        return 3 * getPosition();
    }
};

class Bar : public Foobar {
public:
    Bar(const std::string& n) : Foobar(n) {}

    int getStrength() const override {
        return 15 + getPosition();
    }
};
