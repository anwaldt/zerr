#include <iostream>

#include "wavequant.h"

using namespace std;


int main(int argc, char *argv[])
{

  std::string zerrCfgFile;
  std::string spkrCfgFile;

  // Display each command-line argument.
  cout << "\nCommand-line arguments:\n";

  for(int count = 1; count < (argc-1); count++ )
  {
    if( std::string(argv[count]) == "-z" )
    {
      // cout << argv[count] << std::endl;
      zerrCfgFile = argv[count+1];
    }

    if( std::string(argv[count]) == "-s" )
    {
      // cout << argv[count] << std::endl;
      spkrCfgFile = argv[count+1];
    }
  }

// cout << "  argv[" << count << "]   " << argv[count] << "\n";

    WaveQuant *z = new WaveQuant(zerrCfgFile, spkrCfgFile);

    return 0;
}
