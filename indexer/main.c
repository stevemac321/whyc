#include "tree.h"

const size_t BOOKS = 66;

struct book {
	const char *name;
	struct tree verses;
        unsigned int count;
};

void parse_semi(struct book bks[BOOKS], const int chapter, const int paragraph,
		const int footnote, const char *token);
void parse_hash(struct book bks[BOOKS], const int chapter, const int paragraph,
		const char *token);

void parse_file(struct book bks[BOOKS], const char *filename);
void parse_line(struct book bks[BOOKS], const int chapter, const int paragraph,
		const char *filename);

int capture_num(char *line, const char *delim);
char *trim_leading(char *line);
char *trim_verse_end(char *line);
char *remove_comma_ws(char *line);

int main()
{
	const char *books[] = {"Genesis",
			       "Exodus",
			       "Leviticus",
			       "Numbers",
			       "Deuteronomy",
			       "Joshua",
			       "Judges",
			       "Ruth",
			       "1 Samuel",
			       "2 Samuel",
			       "1 Kings",
			       "2 Kings",
			       "1 Chronicles",
			       "2 Chronicles",
			       "Ezra",
			       "Nehemiah",
			       "Esther",
			       "Job",
			       "Psalm",
			       "Proverbs",
			       "Ecclesiastes",
			       "Song of Solomon",
			       "Isaiah",
			       "Jeremiah",
			       "Lamentations",
			       "Ezekiel",
			       "Daniel",
			       "Hosea",
			       "Joel",
			       "Amos",
			       "Obadiah",
			       "Jonah",
			       "Micah",
			       "Nahum",
			       "Habakkuk",
			       "Zephaniah",
			       "Haggai",
			       "Zechariah",
			       "Malachi",
			       "Matthew",
			       "Mark",
			       "Luke",
			       "John",
			       "Acts",
			       "Romans",
			       "1 Corinthians",
			       "2 Corinthians",
			       "Galatians",
			       "Ephesians",
			       "Philippians",
			       "Colossians",
			       "1 Thessalonians",
			       "2 Thessalonians",
			       "1 Timothy",
			       "2 Timothy",
			       "Titus",
			       "Philemon",
			       "Hebrews",
			       "James",
			       "1 Peter",
			       "2 Peter",
			       "1 John",
			       "2 John",
			       "3 John",
			       "Jude",
			       "Revelation"};

        clock_t start = clock();

	struct book bks[BOOKS];
	for (size_t i = 0; i < BOOKS; i++) {
		bks[i].name = books[i];
                bks[i].count = 0;
		tree_init(&bks[i].verses);
	}

	parse_file(bks, "lbc.txt");

	for (size_t i = 0; i < BOOKS; i++) {
		if (bks[i].verses.root != NULL) {
			fprintf(stdout, "%s\n", bks[i].name);
			tree_print(&bks[i].verses);
			tree_delete(&bks[i].verses);
		}
	}
        
        clock_t stop = clock();
        printf("Ticks: %ld\n", stop - start);
        printf("Seconds: %.4lf\n", (stop - start) / ((double) CLOCKS_PER_SEC));

#ifdef _DUMP
        for(size_t i=0; i < BOOKS; i++) {
                if(bks[i].count > 0)
                        printf("Book: %s Count: %d\n", bks[i].name, bks[i].count);
        }

#endif
}
void parse_file(struct book bks[BOOKS], const char *filename)
{
	FILE *stream;
	char *line = NULL;
	char *chapter = NULL;
	char *para = NULL;

	int chapnum = 0;
	int paranum = 0;
	size_t len = 0;
	ssize_t nread;

	stream = fopen(filename, "r");
	assert(stream);

	while ((nread = getline(&line, &len, stream)) != -1) {
		if ((chapter = strstr(line, "Chapter"))) {
			chapnum = capture_num(chapter, ":");
		} else if ((para = strstr(line, "Paragraph"))) {
			paranum = capture_num(para, ".");
			parse_line(bks, chapnum, paranum, line);
		}
	}

	free(line);
	fclose(stream);
}
void parse_line(struct book bks[BOOKS], const int chapter, const int paragraph,
		const char *line)
{
	char *begin = NULL;
	char *end = NULL;
	while ((begin = strstr(line, "(")) && (end = strstr(line, ")"))) {
		if (begin && end) {
                        *end = '\0';
			if (strstr(begin, ":")) {
                                begin++;
                                break;
			} else {
                                end++;
                                line = end;
                        }
		}
	}

	if (begin == NULL || end == NULL)
		return;

	struct queue q;
	queue_init(&q);

	char *tok = strtok(begin, "#");
	while (tok) {
		queue_enqueue(&q, tok);
		tok = strtok(NULL, "#");
	}
	const char *out = NULL;
	while (!queue_is_empty(&q)) {
		out = queue_dequeue(&q, out);
		parse_hash(bks, chapter, paragraph, out);
	}
	queue_free(&q);
}
void parse_hash(struct book bks[BOOKS], const int chapter, const int paragraph,
		const char *token)
{
	char *footnote = (char *)token;
	char *end = footnote;

	while (isdigit(*end))
		++end;

	*end = '\0';
	token = end + 1;

	int ftnote = atoi(footnote);

	char *dup = strdup(token);

	struct queue q;
	queue_init(&q);

	char *tok = strtok(dup, ";");
	while (tok) {
		queue_enqueue(&q, tok);
		tok = strtok(NULL, ";");
	}
	const char *out = NULL;
	while (!queue_is_empty(&q)) {
		out = queue_dequeue(&q, out);
		parse_semi(bks, chapter, paragraph, ftnote, out);
	}
	queue_free(&q);
	free(dup);
}
void parse_semi(struct book bks[BOOKS], const int chapter, const int paragraph,
		const int footnote, const char *token)
{
	char *trimmed = trim_leading((char *)token);
	trimmed = trim_verse_end(trimmed);

	char *p = strstr(trimmed, ":");
	if (p == NULL)
		return;
	--p;

	while (isdigit(*p) && p > trimmed)
		--p;
	*p = '\0';
	++p;
	while (isspace(*p) && *p != '\0')
		++p;

	char *verses = remove_comma_ws(p);

	assert(isdigit(*verses));

	for (size_t i = 0; i < BOOKS; i++) {
		if (strcmp(trimmed, bks[i].name) == 0) {
                        bks[i].count++;
			struct entry *e = malloc(sizeof(struct entry));
			sprintf(e->ref, "LBC %d.%d.%d", chapter, paragraph,
				footnote);
#ifdef _DUMP
                sprintf(e->verse, "%s %s", trimmed, verses);
                fprintf(stdout, "%-*s %*s\n", 14, e->ref, 14, e->verse);
                free(e);
#else
			strcpy(e->verse, verses);
			tree_insert(&bks[i].verses, e);
#endif
			break;
		}
	}
}

int capture_num(char *line, const char *delim)
{
	char *dup = strdup(line);
	char *find_delim = strstr(dup, delim);

	*find_delim = '\0';

	--find_delim;
	while (isdigit(*find_delim) && find_delim > dup)
		--find_delim;

	while (!isdigit(*find_delim) && find_delim != '\0')
		++find_delim;

	int ret = atoi(find_delim);
	free(dup);
	return ret;
}
char *trim_leading(char *line)
{
	char *p = line;

	while (isspace(*p) && *p != '\0')
		++p;

	line = p;
	return line;
}
char *remove_comma_ws(char *line)
{
	char *p = line;

	while (*p != '\0') {
		if (isspace(*p)) {
			char *end = p;
			while (*(end + 1) != '\0') {
				*end = *(end + 1);
				++end;
			}
			*end = '\0';
		}
		++p;
	}
	return (char *)line;
}
char *trim_verse_end(char *line)
{
	char *end = line;
	while (*end != '\0')
		++end;

	while (isspace(*end) && end > line) {
		*end = '\0';
		--end;
	}
	return line;
}
