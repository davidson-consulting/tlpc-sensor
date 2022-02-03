import socket
import os 

def start(identifier):
    pid = os.getpid()
    print('start', pid, identifier)
    UDP_IP, UDP_PORT = 'localhost', 2000
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(bytes('start ' + str(pid) + ' ' + identifier, encoding='utf-8'), (UDP_IP, UDP_PORT))
    data, addr = sock.recvfrom(1024)
    print(data, addr)
    sock.close()


def stop(identifier):
    print('stop', identifier)
    UDP_IP, UDP_PORT = 'localhost', 2000
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(bytes('stop ' + identifier, encoding='utf-8'), (UDP_IP, UDP_PORT))
    data, addr = sock.recvfrom(1024)
    print(data, addr)
    sock.close()

def loop(iterations):
    for i in range(0, iterations):
        print(i)

def record_loop(iterations, identifier):
    start(identifier)
    loop(iterations)
    stop(identifier)

def report(path):
    print('report', path)
    UDP_IP, UDP_PORT = 'localhost', 2000
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(bytes('report ' + path, encoding='utf-8'), (UDP_IP, UDP_PORT))
    data, addr = sock.recvfrom(1024)
    print(data, addr)
    sock.close()

if __name__ == '__main__':
    start("main")
    record_loop(10000, "loop1")
    loop(10000)
    record_loop(10000, "loop2")
    stop("main")
    report("report_py.json")
