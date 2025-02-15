#include <iostream>
#include <string.h>
using namespace std;

struct studentrecord{
  char name[22];
  int ID;
  char dept[22];
  char gender;
};

int main() {
  studentrecord student1;
  strcpy(student1.name, "Mohan");
  student1.ID = 100;
  strcpy(student1.dept, "Comp");
  student1.gender = 'M';
  cout<<"The student is"<<endl;
  switch (student1.gender) {
    case 'M':
      cout<<"Mr.";break;
    case 'F':
      cout<<"Mrs.";break;
  }
  cout<<student1.name<<endl;
  return 0;
}