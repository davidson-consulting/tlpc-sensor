#include <stdio.h>
 
int main() {
    FILE *fptr = fopen("./report.json","w");
    fprintf(fptr, "{\n");
    fprintf(fptr, "\t\"%s\": %d\n", "key", 10);
    fprintf(fptr, "}");
    fclose(fptr);
}