#define DZM_ELEVATED
#define DZM_DEVEL

#include "src/dzm.hpp"

#include "src/lang/lex/lex_test.hpp"

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char** argv)
{
	SET_SYNTAX(test_read);
	SET_SYNTAX_PAIR(test_read_pair);

    test_init(argc, argv);
    
    return(0);
}
