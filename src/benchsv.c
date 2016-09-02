#include <stdio.h>
#include <string.h>
#include <fml32.h>
#include <atmi.h>
#include <userlog.h>

/*
 * Benchmark service
 */
void BENCHSVC(TPSVCINFO *data)
{
	FBFR32 *ubf;

	ubf = (FBFR32 *)data->data;

	tpreturn(TPSUCCESS, 0,(char *)ubf, 0L, 0);
}

/**
 * Server init
 */
int tpsvrinit(int argc, char **argv)
{
    int ret = 0;

#ifdef NDRX_VERSION
    /* for some reason tuxedo this does not like ...
     * build server will do the advertise anyway... */
    if (0!=tpadvertise("BENCHSVC", BENCHSVC))
    {
	fprintf(stderr, "BENCHSVC advertise failed: %s\n", tpstrerror(tperrno)); 
	ret = -1;
	goto out;
    }
#endif

out:
	return ret;
}

/**
 * Do de-initialization
 */
void tpsvrdone(void)
{
	fprintf(stderr, "BSERVER finished\n");
}

