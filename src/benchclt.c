#include <stdio.h>
#include <string.h>
#include <fml32.h>
#include <atmi.h>
#include <userlog.h>
#include <sys/time.h>
#include "benchflds.fd.h"

double timedifference(struct timeval t0, struct timeval t1)
{
    return ((t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec)/ 1000000.0f);
}


int append_text_file(char *file, char *line)
{
    int ret = 0;
    FILE *f = fopen(file, "a");
    
    if (NULL==f)
    {
        ret=-1;
	goto out;
    }
    
    fprintf(f, "%s", line);
    
out:
    if (NULL!=f)
    {
        fclose(f);
    }
    return ret;
}


int main(int argc, char** argv)
{
	FBFR32 *p_ub = (FBFR32 *)tpalloc("FML32", NULL, 1536);
	int i, j;
	int ret = 0;
	long rsplen;
        int calls = 100000;
        struct timeval stop, start;
        double sec;
        char buf[56*1024];
	
	if (NULL==p_ub)
	{
		fprintf(stderr, "Failed to alloc FML32: %s\n", tpstrerror(tperrno));
		ret = -1;
		goto out;
	}
        
	/* Have some header */
	append_text_file("results.txt", "\"Configuration\" \"MsgSize\" \"CallsPerSec\"\n");
		
	/* Do some stuff for charts... */
	for (j=1; j<56; j=(j==0?j=1:j+4))
	{
		int callsz = j*1024;
		char result[1024];
		p_ub = (FBFR32 *)tprealloc ((char *)p_ub, callsz+500);
		
		if (-1 == Fchg32(p_ub, B_CARRAY_FLD, 0, buf, callsz))
		{
			fprintf(stderr, "Failed to set B_CARRAY_FLD: %s\n", Fstrerror32(Ferror32));
			ret = -1;
			goto out;
		}
		
		
		gettimeofday(&start, NULL);
		for (i = 0; i<calls; i++)
		{
			if (-1 == tpcall("BENCHSVC", (char *)p_ub, 0L, (char **)&p_ub, &rsplen,0))
			{
				fprintf(stderr, "BENCHSVC failed: %s", tpstrerror(tperrno));
				ret = -1;
				goto out;
			}
		}
		gettimeofday(&stop, NULL);

		sec = timedifference(start, stop);
		fprintf(stderr, "took %lf secs\n", sec);
		fprintf(stderr, "calls per sec: %lf (size %d bytes)\n", ((double)calls) / sec, callsz);
		
#ifdef NDRX_VERSION
		sprintf(result, "\"Enduro/X 3.2 Linux 64bit\" %d %.0lf\n", callsz, (((double)calls) / sec));
#else
		sprintf(result, "\"Oracle Tuxedo 12.2.2.0.0 Linux 64bit\" %d %.0lf\n", callsz, (((double)calls) / sec));
#endif
		append_text_file("result.txt", result);
	}
	
	tpfree((char *)p_ub);
	
out:
	fprintf(stderr, "benchclt ends: %d\n", ret);
	return ret;
	
}
