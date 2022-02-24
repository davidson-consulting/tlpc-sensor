package fr.davidson.tlpc.sensor;

import java.io.FileWriter;
import java.util.stream.Collectors;

/**
 * @author Benjamin DANGLOT
 * benjamin.danglot@davidson.fr
 * on 24/02/2022
 */
public class Report {


    static void report(String pathname, IndicatorsPerIdentifier indicatorsPerIdentifier) {
        final StringBuilder builder = new StringBuilder();
        builder.append(OPEN_CURLY_BRACKET).append(NEW_LINE);
        builder.append(report(indicatorsPerIdentifier));
        builder.append(NEW_LINE).append(CLOSE_CURLY_BRACKET);
        try (final FileWriter writer = new FileWriter(pathname)) {
            writer.write(builder.toString());
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    private static String report(IndicatorsPerIdentifier indicatorsPerIdentifier) {
        return indicatorsPerIdentifier.keySet().stream().map(
                identifier ->
                        new StringBuilder()
                                .append(TABULATION)
                                .append(DOUBLE_QUOTE)
                                .append(identifier)
                                .append(DOUBLE_QUOTE)
                                .append(DOUBLE_DOT)
                                .append(WHITE_SPACE)
                                .append(OPEN_CURLY_BRACKET)
                                .append(NEW_LINE)
                                .append(report(indicatorsPerIdentifier, identifier))
                                .append(NEW_LINE)
                                .append(TABULATION)
                                .append(CLOSE_CURLY_BRACKET)
        ).collect(Collectors.joining(COMMA + NEW_LINE));
    }

    private static String report(IndicatorsPerIdentifier indicatorsPerIdentifier, String identifier) {
        return indicatorsPerIdentifier.get(identifier)
                .keySet()
                .stream()
                .map(label ->
                        new StringBuilder()
                                .append(TABULATION)
                                .append(TABULATION)
                                .append(DOUBLE_QUOTE)
                                .append(label)
                                .append(DOUBLE_QUOTE)
                                .append(DOUBLE_DOT)
                                .append(WHITE_SPACE)
                                .append(indicatorsPerIdentifier.get(identifier).get(label))
                ).collect(Collectors.joining(COMMA + NEW_LINE));
    }

    private static final String NEW_LINE = System.getProperty("line.separator");

    private static final String TABULATION = "\t";

    private static final String OPEN_CURLY_BRACKET = "{";

    private static final String CLOSE_CURLY_BRACKET = "}";

    private static final String DOUBLE_QUOTE = "\"";

    private static final String DOUBLE_DOT = ":";

    private static final String WHITE_SPACE = " ";

    private static final String COMMA = ",";

}
