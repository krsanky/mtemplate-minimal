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

static void
define(struct mobject *namespace, const char *kv)
{
	const char *cp;
	char kbuf[256], ebuf[512];
	struct mobject *v;

	if ((cp = strchr(kv, '=')) == NULL || cp == kv || *(cp + 1) == '\0') {
		warnx("Invalid define");
		/*usage();*/
		exit(1);
	}
	if ((size_t)(cp - kv) >= sizeof(kbuf))
		errx(1, "Define key too long");
	memcpy(kbuf, kv, cp - kv);
	kbuf[cp - kv] = '\0';
	if ((v = mstring_new(cp + 1)) == NULL)
		errx(1, "mstring_new failed");

	if (mnamespace_set(namespace, kbuf, v, ebuf, sizeof(ebuf)) != 0)
		errx(1, "mnamespace_set: %s", ebuf);
}

int
main(void)
{
	char		*template;
	char		buf[8192];
	size_t		tlen;
	int		tfd;
	int		len;

	struct mtemplate *t;
	struct mobject *namespace = NULL;

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
	close(tfd);

	if ((t = mtemplate_parse(template, buf, sizeof(buf))) == NULL)
		errx(1, "mtemplate_parse: %s", buf);

	if ((namespace = mdict_new()) == NULL)
		errx(1, "mdict_new failed");

	define(namespace, "v1=asdasd");
	define(namespace, "a.b=QWE.123");
	
	if (mtemplate_run_stdio(t, namespace, stdout, buf, sizeof(buf)) == -1)
		errx(1, "mtemplate_run: %s", buf);

	return EXIT_SUCCESS;
}
