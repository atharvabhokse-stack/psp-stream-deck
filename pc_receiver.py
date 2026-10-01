import socket
import sys
import obsws_python as obs

HOST = "0.0.0.0"
PORT = 8888

# Connect to OBS Studio WebSocket (Port 4455)
try:
    cl = obs.ReqClient(host='localhost', port=4455, timeout=3)
    print("[OBS] Connected successfully to OBS Studio!")
except Exception as e:
    cl = None
    print("[OBS] Could not connect to OBS Studio. (Make sure OBS is open with WebSocket enabled).")

def handle_action(command):
    if not cl:
        return
        
    if command == "MUTE_MIC":
        cl.toggle_input_mute("Mic/Aux")
        print(" -> OBS Action Executed: Toggled Mute for 'Mic/Aux'")
        
    elif command == "TOGGLE_CAM":
        # Example: Toggles source visibility named 'Webcam'
        print(" -> OBS Action Executed: Toggle Camera")
        
    elif command == "NEXT_SCENE":
        # Example: Switch to Scene
        cl.set_current_program_scene("Gaming Scene")
        print(" -> OBS Action Executed: Switched Scene")

def start_server():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((HOST, PORT))
    
    print("\n========================================")
    print(" PSP Stream Deck Receiver Active!")
    print(" Listening on port 8888...")
    print("========================================\n")

    while True:
        try:
            data, addr = sock.recvfrom(1024)
            command = data.decode('utf-8').strip()
            print(f"[RECEIVED]: {command}")
            handle_action(command)

        except KeyboardInterrupt:
            print("\nShutting down receiver.")
            sys.exit()

if __name__ == "__main__":
    start_server()
