#include <iostream>

using namespace std;
class box{
    public:
    double length;
    double breadth;
    double height;
};

double calcalculateVolume(box b){
    return b.length * b.breadth * b.height;
}

int main() {
    box b1;
    b1.length = 10;
    b1.breadth = 5;
    b1.height = 2;
    double volume = calcalculateVolume(b1);
    cout << "Volume of the box is: " << volume << endl;
    return 0;
}