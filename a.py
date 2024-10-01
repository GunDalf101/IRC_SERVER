import socket
import sys
import threading

# Server address and port
server_address = 'localhost'
server_port = 9999

# chareb webserve f FT_IRC, f7al Science Math fach kay9lebha PC
# 

# Initial lines to send
initial_lines = [
    'pass 9999\r\n',
    'nick testtest\r\n',
    'nick testtestt\r\n',
    # 'nick testtesttt\r\n',
    'user r r r r\r\n',
    'A'*10000 + '\r\n',
    # 'join #c\r\n'
]
def receive_from_server(sock):
    while True:
        try:
            data = sock.recv(4096)
            if data:
                sys.stdout.write(data.decode('utf-8'))
                sys.stdout.flush()
            else:
                print('Connection closed by server.')
                break
        except Exception as e:
            print(f'Error receiving data: {e}')
            break

def send_to_server(sock):
    while True:
        try:
            line = input()
            if line:
                line += '\r\n'  # Ensure the line ends with \r\n
                sock.sendall(line.encode('utf-8'))
        except KeyboardInterrupt:
            print("\nDisconnected.")
            break
        except Exception as e:
            print(f'Error sending data: {e}')
            break

def main():
    if len(sys.argv) < 2:
        print("Usage: python script.py <number_of_channels>")
        sys.exit(1)

    num_channels = int(sys.argv[1])

    try:
        # Create a socket object
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            # Connect to the server
            sock.connect((server_address, server_port))
            print(f'Connected to {server_address}:{server_port}')
            
            # Send initial lines
            for line in initial_lines:
                sock.sendall(line.encode('utf-8'))
                print(f'Sent: {line.strip()}')
            
            # Join multiple channels dynamically
            for i in range(1, num_channels + 1):
                join_cmd = f'join :#c{i}\r\n'
                sock.sendall(join_cmd.encode('utf-8'))
                print(f'Sent: {join_cmd.strip()}')

            # Start threads for sending and receiving
            receive_thread = threading.Thread(target=receive_from_server, args=(sock,))
            send_thread = threading.Thread(target=send_to_server, args=(sock,))
            
            receive_thread.start()
            send_thread.start()
            
            send_thread.join()  # Wait for the send_thread to finish
            sock.close()        # Close the socket when done
            receive_thread.join()  # Ensure receive_thread finishes before exiting

    except Exception as e:
        print(f'An error occurred: {e}')

if __name__ == '__main__':
    main()