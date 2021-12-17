import socket
import os 

def start():
    pid = os.getpid()
    print('start', pid)
    UDP_IP, UDP_PORT = 'localhost', 2000
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(bytes('start ' + str(pid), encoding='utf-8'), (UDP_IP, UDP_PORT))
    data, addr = sock.recvfrom(1024)
    print(data, addr)
    sock.close()


def stop():
    UDP_IP, UDP_PORT = 'localhost', 2000
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(b'stop', (UDP_IP, UDP_PORT))
    data, addr = sock.recvfrom(1024)
    print(data, addr)
    sock.close()


if __name__ == '__main__':
    start()
    for i in range(0, 100):
        print(i)
    stop()
