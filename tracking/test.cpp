#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

int main(int argc, char** argv)
  {
  mat A = randu<mat>(5,5);

//A.save("A1.mat");  // default save format is arma_binary
A.save("A2.mat", arma_ascii);

//mat B;
// automatically detect format type
//B.load("A1.mat");

mat C;
// force loading in the arma_ascii format
C.load("A2.mat", arma_ascii);


// example of saving/loading using a stream
//std::stringstream s;
//A.save(s);

//mat D;
//D.load(s);


// example of testing for success
mat E;
bool status = E.load("A2.mat");

if(status == true)
  {
  cout << "loaded okay" << endl;
  }
else
  {
  cout << "problem with loading" << endl;
  }
  system("pause");
  return 0;
  }