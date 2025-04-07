#include <iostream>
using namespace std;

const int MAX_LENGTH = 100; // Define MAX_LENGTH

template<typename ItemType>
class GList {
public:
    GList() : length(0) {} // Constructor

    bool IsEmpty() const { return length == 0; }
    bool IsFull() const { return length >= MAX_LENGTH; }
    int Length() const { return length; }
    
    void Insert(ItemType item) {
        if (!IsFull()) {
            data[length] = item;
            length++;
        } else {
            cout << "List is full!" << endl;
        }
    }
    
    void Delete(ItemType item) {
        for (int i = 0; i < length; i++) {
            if (data[i] == item) {
                for (int j = i; j < length - 1; j++) {
                    data[j] = data[j + 1];
                }
                length--;
                return;
            }
        }
        cout << "Item not found!" << endl;
    }
    
    bool IsPresent(ItemType item) const {
        for (int i = 0; i < length; i++) {
            if (data[i] == item) return true;
        }
        return false;
    }
    
    void Print() const {
        for (int i = 0; i < length; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

private:
    int length;
    ItemType data[MAX_LENGTH];
};

int main() {
    GList<int> intList;
    intList.Insert(1);
    intList.Insert(2);
    intList.Insert(3);
    intList.Print(); // Output: 1 2 3
    cout << "Length: " << intList.Length() << endl; // Output: 3
    intList.Delete(2);
    intList.Print(); // Output: 1 3
    return 0;
}