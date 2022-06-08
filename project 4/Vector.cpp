#include "Vector.h"
#include <iostream>
#include <cstddef>
#include <stdexcept>
using namespace std;
using std::size_t;

    // Object Mgt.
void Vector::grow() {                           //Grow function, creates new larger array and deletes old array
    capacity *= 1.6;
    int* temp_ptr = new int[capacity];
    for(int i = 0; i < n_elems; i++)
        temp_ptr[i] = data_ptr[i];
    Vector::~Vector();
    data_ptr = temp_ptr;
}

Vector::Vector() {
    data_ptr = new int[CHUNK];
    capacity = CHUNK;
    n_elems = 0;
}

Vector::Vector(const Vector& v){                // Copy constructor
    data_ptr = new int[this->capacity];
    capacity = v.capacity;
    n_elems = v.n_elems;
    for(int i = 0; i < v.n_elems; i++)
        this->data_ptr[i] = v.at(i);
}

Vector& Vector::operator=(const Vector& v) {    // Copy assignment operator
    this->capacity = v.capacity;
    this->data_ptr = new int[this->capacity];
    this->n_elems = v.n_elems;
    for(int i = 0; i<v.n_elems; i++)
        this->data_ptr[i] = v.at(i);
    return (*this);
}

Vector::~Vector() {
    delete[] data_ptr;
}

    // Accessors

int Vector::front() const {                     // Return the int in position 0, if any
    if(this->n_elems > 0)
        return data_ptr[0];
    throw range_error("Range-error: No items to return");
}

int Vector::back() const {                      // Return last element (position n_elems-1)
    if(this->n_elems > 0)
        return data_ptr[n_elems - 1];
    throw range_error("Range-error: No items to return");
}

int Vector::at(size_t pos) const {              // Return element in position "pos" (0-based)
    if(pos >= 0 && pos < n_elems)
        return data_ptr[pos];
    throw range_error("Pos not in range of vector");
}

size_t Vector::size() const {                   // Return n_elems
    return (size_t)n_elems;
}

bool Vector::empty() const {                    // Return n_elems == 0
    return n_elems == 0;
}

    // Mutators

int& Vector:: operator[](size_t pos) {          // Same as at but no bounds checking
    return this->data_ptr[pos];
}

void Vector::push_back(int item) {              // Append a new element at the end of the array
    if(capacity == n_elems) {
        grow();
        data_ptr[n_elems] = item;
        n_elems++;
    } else
        data_ptr[n_elems++] = item;
}

void Vector::pop_back() {                       // --n_elems (nothing else to do; returns nothing)
    if(n_elems > 0)
        --n_elems;
    throw range_error("No items");
}

void Vector::erase(size_t pos) {                // Remove item in position pos and shuffles following items left
    if(pos >= 0 && pos < n_elems){
        for(int i = pos+1; i < n_elems; i++)
            data_ptr[i-1] = data_ptr[i];
        n_elems--;
    } else
        throw range_error("Pos not in range of array");
}

void Vector::insert(size_t pos, int item) {     // Shuffle items right to make room for a new element
    if(capacity==n_elems)
        grow();
    for(int i = n_elems; i > pos; i--)
        data_ptr[i] = data_ptr[i - 1];
    data_ptr[pos] = item;
    n_elems++;
}

void Vector::clear() {                          // n_elems = 0 (nothing else to do; keep the current capacity)
    n_elems = 0;
}

    // Iterators

int* Vector::begin() {                          // Return a pointer to 1st element, or nullptr if n_elems == 0
    if(n_elems == 0)
        return nullptr;
    return &data_ptr[0];
}

int* Vector::end() {                            // Return a pointer to 1 past last element, or nullptr if n_elems == 0
    if(n_elems == 0)
        return NULL;
    return &data_ptr[n_elems];
}

    // Comparators

bool Vector::operator==(const Vector& v) const {
    if(this->capacity != v.capacity)
        return false;
    if(this->n_elems != v.n_elems)
        return false;
    for(int i=0; i < n_elems; i++){
        if(this->data_ptr[i] != v.data_ptr[i])
            return false;
    }
    return true;
}

bool Vector::operator!=(const Vector& v) const {
    return !(operator==(v));
}