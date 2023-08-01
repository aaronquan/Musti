#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <stdexcept>

#include "../Geometry/Util.h";

using namespace std;
using namespace Eigen;

template <class T> void safeRelease(T** ppT) {
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

int add(int a, int b);

void outputDebugLine(Array2f arr);
void outputDebugLine(Vector2f v);
void outputDebugLine(Array2i arr);
void outputDebugLine(Vector2i v);

void outputDebugLine(char c);
void outputDebugLine(string s);

void outputDebugLine(wstring s);

void outputDebugLine(vector<unsigned int> v);

template <typename T>
class VectorStream {
private:
    vector<T> vec;
    unsigned int curr;
public:
    VectorStream() : curr(0){};
    void setVector(vector<T> v){
        vec = v;
        curr = 0;
    };
    T value() const{
        if (curr >= vec.size() || curr < 0) {
            throw out_of_range("Index out of vector range: VectorStream");
        }
        return vec[curr];
    };
    bool next(){
        curr++;
        return (curr < vec.size());
    };
    bool prev() {
        curr--;
        return (curr >= 0);
    }
    void first(){
        curr = 0;
    };
    void last() {
        curr = vec.size();
    };
};