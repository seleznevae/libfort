#include <stdio.h>
#include "fort.h"




int main()
{
    FTABLE *table = ft_create_table();

    int n = ft_hdr_printf(table, "%d , %c|| %s|%f", 3, 'c', "234", 3.14);

    fprintf(stderr, "n = %d\n", n);

    fprintf(stderr, "result: %s\n", ft_to_string(table));

//    printf("Hello, world!\n");
//    char buffer[3] = {'a', 'b', 'c'};
//    int ret = snprintf(buffer, 3, "so");
//    printf("result is %s = lent = %d\n", buffer, ret);
//    fprintf(stderr, "asd %d|", 3);
//    fprintf(stderr, "%d\n", 3.14);
    ft_destroy_table(table);
}
