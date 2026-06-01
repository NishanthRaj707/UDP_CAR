from pynput import keyboard
import socket

# --- Network Configuration ---
# REPLACE THIS with the IP address printed on your Arduino IDE Serial Monitor
ESP32_IP = "10.93.209.51" 
UDP_PORT = 1234

# Create the UDP Socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# State tracker to prevent network spamming when a key is held down
current_key = None

def send_command(cmd_char):
    """Encodes the character to bytes and fires the UDP packet."""
    try:
        sock.sendto(cmd_char.encode(), (ESP32_IP, UDP_PORT))
    except Exception as e:
        print(f"Network Error: {e}")

def on_press(key):
    global current_key
    
    # If the key is already being held down, do nothing
    if key == current_key:
        return 
        
    current_key = key
    
    # Map Arrow Keys to Motor Commands
    if key == keyboard.Key.up:
        send_command('F')
        print("Action: Forward [F]")
    elif key == keyboard.Key.down:
        send_command('B')
        print("Action: Backward [B]")
    elif key == keyboard.Key.left:
        send_command('L')
        print("Action: Turn Left [L]")
    elif key == keyboard.Key.right:
        send_command('R')
        print("Action: Turn Right [R]")

def on_release(key):
    global current_key
    
    # Reset the state tracker for this key
    if key == current_key:
        current_key = None
        
    # If a movement key was released, instantly fire the Stop command
    if key== keyboard.Key.down:
        send_command('S')
        print("Action: Stop [S]")

    # Press ESC to cleanly shut down the script
    if key == keyboard.Key.esc:
        print("\nShutting down Teleoperation Master Node...")
        return False

# --- Main Execution ---
print("=========================================")
print(" UDP TELEOPERATION ACTIVE")
print(f" Targeting ESP32 at {ESP32_IP}:{UDP_PORT}")
print("=========================================")
print(" Controls: Arrow Keys to drive.")
print(" Press ESC to quit.\n")

# Start listening to the keyboard
with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()
