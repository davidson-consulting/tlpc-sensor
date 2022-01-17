package fr.davidson.j.tlpc.sensor;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.util.Arrays;

public class JNIClient {

    public native void start();

    public native void stop();

    private static final String DEFAULT_BASE_DIR = System.getProperty("java.io.tmpdir") + "/libperf";

    private static void extractLibPerf() {
        final String libperfDotSO = "/libperf.so";
        final File fileLib = new File(DEFAULT_BASE_DIR + libperfDotSO);
        final File directory = new File(DEFAULT_BASE_DIR);
        if (fileLib.exists()) {
            fileLib.delete();
            directory.delete();
        }
        directory.mkdir();
        final String extractFilePath = DEFAULT_BASE_DIR + libperfDotSO;
        try (final InputStream resourceAsStream = JNIClient.class.getResourceAsStream(libperfDotSO)) {
            try (final FileOutputStream writer = new FileOutputStream(extractFilePath)) {
                byte[] buffer = new byte[1024];
                int bytesRead = 0;
                while ((bytesRead = resourceAsStream.read(buffer)) != -1) {
                    writer.write(buffer, 0, bytesRead);
                }
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    private static void addDirectoryToLoadedLibraries() {
        try {
            Field field = ClassLoader.class.getDeclaredField("usr_paths");
            field.setAccessible(true);
            String[] paths = (String[])field.get(null);
            if (Arrays.asList(paths).contains(DEFAULT_BASE_DIR)) {
                return;
            }
            String[] tmp = new String[paths.length+1];
            System.arraycopy(paths,0,tmp,0,paths.length);
            tmp[paths.length] = DEFAULT_BASE_DIR;
            field.set(null,tmp);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    static {
        extractLibPerf();
        addDirectoryToLoadedLibraries();
        System.loadLibrary("perf");
    }

    public static void main(String[] args) {
        final JNIClient sensor = new JNIClient();
        sensor.start();
        for (int i = 0 ; i < 10000 ; i++) {
            System.out.println(i);
        }
        sensor.stop();
    }

}