# ft_irc

A custom IRC (Internet Relay Chat) server implemented in C++98, enabling real-time text communication between multiple clients. Tested with [LimeChat](https://limechat.net/mac/) as the reference client for compatibility and stability.

## Features

### Core Functionality
- **Authentication**: Requires a password for client connections.
- **User Commands**:
  - `NICK` and `USER`: Set nickname and username for identity on the server.
  - `JOIN`: Join or create a channel.
  - `PRIVMSG`: Send private messages to users or channels.
- **Channel Communication**: Messages sent in a channel are broadcasted to all members.

### Operator Commands
- **Channel Management**:
  - `KICK`: Remove a user from the channel.
  - `INVITE`: Invite users to join a channel.
  - `TOPIC`: Set or view the topic of the channel.
  - `MODE`: Adjust channel modes:
    - `i`: Invite-only access.
    - `t`: Only operators can set the topic.
    - `k`: Set a password for the channel.
    - `o`: Grant or revoke operator privileges.
    - `l`: Set a user limit for the channel.

### Bonus Features
- **File Transfer Support**: Users can send files to each other.
- **Bot**: Adds a bot to interact with users or respond in channels.
  
## Implementation Details
This IRC server was built using:
- **Non-blocking I/O and Polling**: The server employs non-blocking sockets, using `poll()` to handle multiple clients through a single event loop. This approach ensures the server remains responsive and can manage numerous simultaneous connections without blocking.
- **TCP/IP Communication**: All client-server interactions are handled through the TCP/IP protocol, ensuring stable, real-time messaging.
- **Robust Error Handling**: The server is designed to prevent crashes and manage unexpected input gracefully, ensuring a consistent user experience even under heavy load.
- **C++98 Compliance**: The project adheres strictly to C++98 standards, avoiding any libraries beyond those natively available in C++98. This adds a layer of complexity but ensures compatibility across older systems.


## Getting Started

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/ft_irc.git
   cd ft_irc
 
2. Build and Run the Server:
  ```
  make && ./ircserv <port> <server_password>
  ```
```<port>```: The port number the server will listen on for incoming connections.

```<server_password>```: Password required by clients to connect.
## Compatibility
Compatible with Unix-like systems, including macOS (with ```fcntl``` for non-blocking I/O compatibility).
