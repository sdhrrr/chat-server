import os


ip_address = str(input())
# port = int(input()) 
#change the port number if you want to
port = 8000
os.system("g++ client/client.cpp -o client -lws2_32")
os.system(f".\client {ip_address} {port}")

