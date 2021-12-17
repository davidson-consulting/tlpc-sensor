import java.io.IOException;
import java.net.*;

/**
 * @author Benjamin DANGLOT
 * benjamin.danglot@davidson.fr
 * on 15/12/2021
 */
public class LWPC {

    private DatagramSocket socket;
    private InetAddress address;

    public LWPC() throws SocketException, UnknownHostException {
        socket = new DatagramSocket();
        address = InetAddress.getByName("localhost");
    }

    public void start() throws IOException {
        System.out.println("start " + ProcessHandle.current().pid());
        final byte[] buffer = ("start " + ProcessHandle.current().pid()).getBytes();
        DatagramPacket packet = new DatagramPacket(buffer, buffer.length, this.address, 2000);
        this.socket.send(packet);
        packet = new DatagramPacket(buffer, buffer.length);
        socket.receive(packet);
    }

    public void stop() throws IOException {
        final byte[] buffer = "stop".getBytes();
        DatagramPacket packet = new DatagramPacket(buffer, buffer.length, this.address, 2000);
        this.socket.send(packet);
        packet = new DatagramPacket(buffer, buffer.length);
        socket.receive(packet);
    }

    public static void main(String[] args) throws IOException {
        final LWPC lwpc = new LWPC();
        lwpc.start();
        for (int i = 0 ; i < 100 ; i++) {
            System.out.print(i + " ");
        }
        lwpc.stop();
    }

}
