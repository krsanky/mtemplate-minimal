#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <unistd.h>

#include "mtemplate.h"
#include "mobject.h"

int
main(void)
{
	char		*template;
	char		buf[8192];
	size_t		tlen;
	int		len, ch, tfd;

	printf("testout template stuff...\n");

	if ((tfd = open("t1.tpl", O_RDONLY)) == -1)
		err(1, "open(\"%s\", O_RDONLY)", "t1.tpl");

	template = NULL;
	tlen = 0;
	for (;;) {
		if ((len = read(tfd, buf, sizeof(buf))) == -1) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			err(1, "read");
		}
		if (len == 0)
			break;
		if (tlen + len + 1 < tlen)
			errx(1, "template length exceeds SIZE_T_MAX");
		if ((template = realloc(template, tlen + len + 1)) == NULL)
			errx(1, "realloc(template, %zu) failed",
			    tlen + len + 1);
		memcpy(template + tlen, buf, len);
		*(template + tlen + len) = '\0';
		tlen += len;
	}

	return EXIT_SUCCESS;
}
