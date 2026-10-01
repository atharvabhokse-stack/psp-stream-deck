import socket
import sys

# Listen on all local network interfaces on port 8888
HOST = "0.0.0.0"
PORT = 8888

def start_server():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((HOST, PORT))
    
    print("=" * 40)
    print(f" PSP Stream Deck Receiver Active!")
    print(f" Listening for signals on port {PORT}...")
    print(" Press Ctrl + C in this terminal to stop.")
    print("=" * 40 + "\n")

    while True:
        try:
            data, addr = sock.recvfrom(1024)
            command = data.decode('utf-8').strip()
            
            print(f"[RECEIVED from {addr[0]}]: {command}")

            # Map incoming commands to actions
            if command == "MUTE_MIC":
                print(" -> Action: Muting/Unmuting Microphone")
                # Trigger action (e.g. via OBS WebSocket or keyboard shortcut)
            elif command == "TOGGLE_CAM":
                print(" -> Action: Toggling Camera")
            elif command == "NEXT_SCENE":
                print(" -> Action: Switching Scene")

        except KeyboardInterrupt:
            print("\nShutting down receiver server.")
            sys.exit()
        except Exception as e:
            print(f"Error: {e}")

if __name__ == "__main__":
    start_server()
