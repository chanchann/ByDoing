#ifndef SHAPE_H
#define SHAPE_H

#include <stdexcept>  // std::logic_error
#include <iostream>
enum class shape_type {
    circle,
    triangle,
    rectangle,
};

class shape {
public:
    virtual ~shape() {}
    virtual void print() {
        std::cout << "I am shape" << std::endl;
    }
};

class circle : public shape {
public:
    circle() { std::cout << "circle()" << std::endl; }
    ~circle() { std::cout << "~circle()" << std::endl; }
    void print() {
        std::cout << "I am circle" << std::endl;
    }
};

class triangle : public shape {
public:
    triangle() { std::cout << "triangle()" << std::endl; }
    ~triangle() { std::cout << "~triangle()" << std::endl; }
    void print() {
        std::cout << "I am triangle" << std::endl;
    }
};

class rectangle : public shape {
public:
    rectangle() { std::cout << "rectangle()" << std::endl; }
    ~rectangle() { std::cout << "~rectangle()" << std::endl; }
    void print() {
        std::cout << "I am rectangle" << std::endl;
    }
};

inline shape* create_shape(shape_type type) {
    switch (type) {
    case shape_type::circle:
        return new circle();
    case shape_type::triangle:
        return new triangle();
    case shape_type::rectangle:
        return new rectangle();
    }
    throw std::logic_error("shape type is invalid");
}

#endif // SHAPE_H