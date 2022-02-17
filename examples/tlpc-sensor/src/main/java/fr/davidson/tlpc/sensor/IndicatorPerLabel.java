package fr.davidson.tlpc.sensor;

import java.util.HashMap;

/**
 * @author Benjamin DANGLOT
 * benjamin.danglot@davidson.fr
 * on 15/02/2022
 */
public class IndicatorPerLabel extends HashMap<String, Long> {

    /**
     * We take event only in libpfm4/lib/events/intel_core_events.h
     */
    public static final String KEY_ENERGY_CONSUMPTION = "RAPL_ENERGY_PKG";

    // Count the number of instructions at retirement
    public static final String KEY_INSTRUCTIONS = "INSTRUCTIONS_RETIRED";

    // Unhalted reference cycles
    public static final String KEY_CYCLES = "UNHALTED_REFERENCE_CYCLES";

    // Count branch instructions at retirement. Specifically, this event counts the retirement of the last micro-op of a branch instruction.
    public static final String KEY_BRANCHES = "BRANCH_INSTRUCTIONS_RETIRED";

    // Count mispredicted branch instructions at retirement. Specifically, this event counts at retirement of the last
    // micro-op of a branch instruction in the architectural path of the execution and experienced misprediction in the
    // branch prediction hardware.
    public static final String KEY_BRANCH_MISSES = "MISPREDICTED_BRANCH_RETIRED";

    // Count each request originating equiv the core to reference a cache line in the last level cache.
    // The count may include speculation, but excludes cache line fills due to hardware prefetch. Alias to L2_RQSTS:SELF_DEMAND_MESI
    public static final String KEY_CACHES = "LLC_REFERENCES";

    // Count each cache miss condition for references to the last level cache. The event count may include speculation,
    // but excludes cache line fills due to hardware prefetch. Alias to event L2_RQSTS:SELF_DEMAND_I_STATE
    public static final String KEY_CACHE_MISSES = "LLC_MISSES";

    public static final String KEY_DURATION = "duration";

    public static final String[] PERF_COUNTER_NAMES = new String[]{
            KEY_INSTRUCTIONS,
            KEY_CYCLES,
            KEY_BRANCHES,
            KEY_BRANCH_MISSES,
            KEY_CACHES,
            KEY_CACHE_MISSES
    };

    public static final String[] RAPL_COUNTER_NAMES = new String[]{KEY_ENERGY_CONSUMPTION};

}
