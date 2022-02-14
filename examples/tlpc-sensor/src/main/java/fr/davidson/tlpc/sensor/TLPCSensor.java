package fr.davidson.tlpc.sensor;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.util.Arrays;

public class TLPCSensor {

    public native void start(String identifier);

    public native void stop(String identifier);

    public native void report(String pathname);

    private static final String DEFAULT_BASE_DIR = System.getProperty("java.io.tmpdir") + "/libperf";

    private static void extractLibPerf() {
        final String libperfDotSO = "/libperf.so";
        final File fileLib = new File(DEFAULT_BASE_DIR + libperfDotSO);
        final File directory = new File(DEFAULT_BASE_DIR);

        try {
            if (fileLib.exists()) {
                fileLib.delete();
                if (directory.exists()) {
                    directory.delete();
                }
            }
        } catch (Exception ignored) {
            System.err.println("Something went wrong when preparing the directories that contains share library.");
            System.err.println("It might result in an unstable run. Please check it out.");
            System.err.println(directory.getAbsolutePath());
            System.err.println(fileLib.getAbsolutePath());
        }
        try {
            directory.mkdir();
        } catch (Exception ignored) {

        }

        final String extractFilePath = DEFAULT_BASE_DIR + libperfDotSO;
        try (final InputStream resourceAsStream = TLPCSensor.class.getResourceAsStream(libperfDotSO)) {
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

    public static void test() {
        /**
         *  This code is made avalaible to test the API
         */
        final TLPCSensor tlpcSensor = new TLPCSensor();
        tlpcSensor.start("main");
        tlpcSensor.start("loop1");
        for (int i = 0; i < 100000 ; i++) {
            System.out.println(i);
        }
        tlpcSensor.stop("loop1");
        tlpcSensor.start("loop2");
        for (int i = 0; i < 100000 ; i++) {
            System.out.println(i);
        }
        tlpcSensor.stop("loop2");
        tlpcSensor.stop("main");
        tlpcSensor.report("target/report_java.json");
    }
}