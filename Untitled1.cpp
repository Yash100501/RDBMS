#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main () {
  string line;
  ifstream myfile ("STD.txt");
  if (myfile.is_open())
  {
    while (! myfile.eof() )
    {
      getline (myfile,line);
      for(int i=0;i<line.size();i++)
      {
          if(line[i]=='#')
          {
              cout<<" ";
              continue;
          }
          cout<<line[i];
      }
      cout<<"\n";

    }
    myfile.close();
  }

  else
    cout << "Unable to open file";

  return 0;
}
