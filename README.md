# Custom VPN – Secure Client-Server Communication over TUN Interface

This project implements a basic VPN using C, OpenSSL, and the Linux TUN driver to enable encrypted communication between a client and a server over a virtual network interface.

## 🔒 Features

- AES-128 encrypted communication (CBC mode)
- TUN device for virtual networking
- Simple client-server architecture over TCP sockets
- Secure message exchange using OpenSSL

## 📦 Requirements

- Ubuntu Linux
- GCC Compiler
- OpenSSL Library
- Root privileges (for TUN device setup)

## 📁 Project Structure

- `client/`: Contains the VPN client code.
- `server/`: Contains the VPN server code.
- `tun_setup/`: Contains TUN device setup code.

## 🛠️ Setup & Compilation

### TUN Device Setup

```bash
sudo ip tuntap add dev tun0 mode tun
sudo ip addr add 10.0.0.1/24 dev tun0
sudo ip link set tun0 up
