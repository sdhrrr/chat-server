import socket
import subprocess

try: 
    def get_local_ip() -> str:
        try:
            # Create a socket object
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            
            # Connect to a dummy address
            s.connect(("8.8.8.8", 80))
            
            # Get the local IP address
            local_ip = s.getsockname()[0]
            
            # Close the socket
            s.close()
            
            return local_ip
        except socket.error:
            return None

    print(str(get_local_ip()))
    # subprocess.call("mingw32-make") //uncomment if you want to make again
    subprocess.call(f"./main {get_local_ip()}")

except Exception:
    print("Program coudn't run.");