package fr.davidson.tlpc.sensor;

import org.junit.jupiter.api.Test;

/**
 * @author Benjamin DANGLOT
 * benjamin.danglot@davidson.fr
 * on 16/02/2022
 */
public class TLPCSensorTest {

    @Test
    void test() {
        TLPCSensor.start("fr.davidson.tlpc.sensor.TLPCSensorTest#test");
        System.out.println("Measuring the consumption of this print!");
        TLPCSensor.stop("fr.davidson.tlpc.sensor.TLPCSensorTest#test");
        TLPCSensor.start("fr.davidson.tlpc.sensor.TLPCSensorTest#test2");
        System.out.println("Measuring the consumption of this print!");
        TLPCSensor.stop("fr.davidson.tlpc.sensor.TLPCSensorTest#test2");
        TLPCSensor.start("fr.davidson.tlpc.sensor.TLPCSensorTest#test3");
        System.out.println("Measuring the consumption of this print!");
        TLPCSensor.stop("fr.davidson.tlpc.sensor.TLPCSensorTest#test3");
        TLPCSensor.report("target/test.json");
    }

    @Test
    void test2() {
        TLPCSensor.start("fr.davidson.tlpc.sensor.TLPCSensorTest#test2");
        System.out.println("Measuring the consumption of this print!");
        TLPCSensor.stop("fr.davidson.tlpc.sensor.TLPCSensorTest#test2");
        TLPCSensor.report("target/test.json");
    }

    @Test
    void test3() {
        TLPCSensor.start("fr.davidson.tlpc.sensor.TLPCSensorTest#test3");
        System.out.println("Measuring the consumption of this print!");
        TLPCSensor.stop("fr.davidson.tlpc.sensor.TLPCSensorTest#test3");
        TLPCSensor.report("target/test.json");
    }


}
