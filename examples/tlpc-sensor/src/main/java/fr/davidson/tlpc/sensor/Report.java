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
        builder.append(OPEN_CURLY_BRACKET)
                .append(NEW_LINE)
                .append(TABULATION)
                .append(DOUBLE_QUOTE)
                .append("test_measures")
                .append(DOUBLE_QUOTE)
                .append(DOUBLE_DOT)
                .append(WHITE_SPACE)
                .append(OPEN_BRACKET)
                .append(NEW_LINE)
                .append(buildArrayMeasures(indicatorsPerIdentifier))
                .append(NEW_LINE)
                .append(TABULATION)
                .append(CLOSE_BRACKET)
                .append(NEW_LINE)
                .append(CLOSE_CURLY_BRACKET);
        try (final FileWriter writer = new FileWriter(pathname)) {
            writer.write(builder.toString());
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    private static String buildArrayMeasures(IndicatorsPerIdentifier indicatorsPerIdentifier) {
        return indicatorsPerIdentifier.keySet().stream().map(
                testIdentifier ->
                        new StringBuilder()
                                .append(TABULATION).append(TABULATION).append(OPEN_CURLY_BRACKET).append(NEW_LINE)
                                .append(TABULATION).append(TABULATION).append(TABULATION)
                                .append(DOUBLE_QUOTE).append("test_identifier").append(DOUBLE_QUOTE).append(DOUBLE_DOT)
                                .append(WHITE_SPACE).append(DOUBLE_QUOTE).append(testIdentifier).append(DOUBLE_QUOTE).append(COMMA).append(NEW_LINE)
                                .append(TABULATION).append(TABULATION).append(TABULATION)
                                .append(DOUBLE_QUOTE).append("measures").append(DOUBLE_QUOTE).append(DOUBLE_DOT)
                                .append(WHITE_SPACE).append(OPEN_BRACKET).append(NEW_LINE)
                                .append(buildArrayMeasure(indicatorsPerIdentifier.get(testIdentifier)))
                                .append(NEW_LINE)
                                .append(TABULATION).append(TABULATION).append(TABULATION)
                                .append(CLOSE_BRACKET).append(NEW_LINE)
                                .append(TABULATION).append(TABULATION).append(CLOSE_CURLY_BRACKET)
        ).collect(Collectors.joining(COMMA + NEW_LINE));
    }

    private static String buildArrayMeasure(IndicatorPerLabel indicatorPerLabel) {
        return new StringBuilder().append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION)
                .append(OPEN_BRACKET).append(NEW_LINE)
                .append(
                        indicatorPerLabel.keySet().stream().map(
                                indicator ->
                                        new StringBuilder()
                                                .append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION).append(OPEN_CURLY_BRACKET).append(NEW_LINE)
                                                .append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION)
                                                .append(DOUBLE_QUOTE).append("indicator").append(DOUBLE_QUOTE).append(DOUBLE_DOT)
                                                .append(WHITE_SPACE).append(DOUBLE_QUOTE).append(indicator).append(DOUBLE_QUOTE).append(COMMA).append(NEW_LINE)
                                                .append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION)
                                                .append(DOUBLE_QUOTE).append("value").append(DOUBLE_QUOTE).append(DOUBLE_DOT).append(WHITE_SPACE).append(indicatorPerLabel.get(indicator)).append(NEW_LINE)
                                                .append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION).append(CLOSE_CURLY_BRACKET)
                        ).collect(Collectors.joining(COMMA + NEW_LINE))
                ).append(NEW_LINE)
                .append(TABULATION).append(TABULATION).append(TABULATION).append(TABULATION)
                .append(CLOSE_BRACKET).toString();
    }

    private static final String NEW_LINE = System.getProperty("line.separator");

    private static final String TABULATION = "\t";

    private static final String OPEN_CURLY_BRACKET = "{";

    private static final String CLOSE_CURLY_BRACKET = "}";

    private static final String OPEN_BRACKET = "[";

    private static final String CLOSE_BRACKET = "]";

    private static final String DOUBLE_QUOTE = "\"";

    private static final String DOUBLE_DOT = ":";

    private static final String WHITE_SPACE = " ";

    private static final String COMMA = ",";

}
