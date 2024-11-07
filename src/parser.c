#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>

#define yaml_parse(p, e) 		if (!yaml_parser_parse(&parser, &event)) {  \
									printf("YAML Parser error %d\n", parser.error); \
									exit(EXIT_FAILURE); }
static int map = 0;
static int seq = 0;
static int pkgnum = 0;
static int depnum = 0;
char** tmp_deplist = NULL;
struct pkg * tmp_pkglist = NULL;
struct pkg { 
	unsigned int major;
	unsigned int minor;
	unsigned int subver;
	unsigned int patch;
	unsigned int depnum;
	char* name;
	char* filename;
	char* author;
	char* source;
	char* script;
	char** deplist;

};

struct pkg * pkglist;

void parse_pkglist(char* filename);

int main(int argc, char** argv){ 

	pkglist = calloc(1, sizeof(struct pkg));

	if (argc < 2){ 
		printf("No package list was provided\n");
	}
	
	parse_pkglist(argv[1]);



	printf("\nPackage Info:\n");
	for(int i = 0; i < pkgnum; i++){ 
		printf("Name:\t\t\t%s\n", pkglist[i].name);
		printf("Filename:\t\t%s\n", pkglist[i].filename);
		printf("Version:\t\t%d.%d.%d-%d\n", pkglist[i].major, pkglist[i].minor, pkglist[i].subver, pkglist[i].patch);
		printf("Build script:\t\t%s\n", pkglist[i].script);
		printf("URL:\t\t\t%s\n", pkglist[i].source);
		printf("Num of Dependencies:\t%u\n", pkglist[i].depnum);
		for(int j = 0; j < pkglist[i].depnum; j++){ 
			printf("Dependency %d:\t\t%s\n", j, pkglist[i].deplist[j]);
		}
		printf("\n\n");
	}


cleanup:
	for(int i = 0; i < pkgnum; i++){ 
		for(int j = 0; j < pkglist[i].depnum; j++){ 
			free(pkglist[i].deplist[j]);
		}
		free(pkglist[i].deplist);
		free(pkglist[i].source);
		free(pkglist[i].script);
		free(pkglist[i].filename);
		free(pkglist[i].name);
	}

	free(pkglist);
	return 0;
}

void parse_pkglist(char* filename){ 

	yaml_parser_t parser;
	yaml_event_t event;

	char* repo_src;
	char* repo_name;

	FILE *fpkglist = fopen(filename, "r");
	if (fpkglist == NULL){ 
		printf("Could not open package list file: %s\n", filename);
	}

	if (!yaml_parser_initialize(&parser)){ 
		printf("Failed to initalize yaml parser\n");
	}
	
	yaml_parser_set_input_file(&parser, fpkglist);

	do { 
		if (!yaml_parser_parse(&parser, &event)) { 
			printf("YAML Parser error %d\n", parser.error);				
			exit(EXIT_FAILURE);
		}

		switch(event.type){ 
			case YAML_NO_EVENT:
				break;
			case YAML_STREAM_START_EVENT:
				
				break;
			case YAML_STREAM_END_EVENT:
				break;
			/* Block delimiters */
			case YAML_DOCUMENT_START_EVENT:
				break;
			case YAML_DOCUMENT_END_EVENT:
				break;
			case YAML_SEQUENCE_START_EVENT:
					
				seq++;
				
				break;
			case YAML_SEQUENCE_END_EVENT:
				
				if (map == 2 && seq == 2){ 
					depnum = 0;
				}

				seq--;
				break;
			case YAML_MAPPING_START_EVENT:
				map++;

				if ((pkgnum > 0) && (map == 2 && seq == 1)){ 
					tmp_pkglist = reallocarray(pkglist, pkgnum+1, sizeof(struct pkg));
					if (tmp_pkglist == NULL){ 
						printf("Error changing size of package list\n");
						exit(1); 
					}
					else{ 
						pkglist = tmp_pkglist;
						tmp_pkglist = NULL;
					}

				}

				break;
			case YAML_MAPPING_END_EVENT:
				if (map == 2 && seq == 1){ 
					pkglist[pkgnum].source = calloc(strlen(repo_src) , sizeof(char));
					memcpy(pkglist[pkgnum].source, repo_src, strlen(repo_src));
					printf("Copy repo src: %s\n", pkglist[pkgnum].source);
					pkgnum++;
				}

				map--;
				break;
			case YAML_ALIAS_EVENT:
				break;
			case YAML_SCALAR_EVENT:

				if (map == 1 && seq == 0){ 
					if (strncmp(event.data.scalar.value, "source", event.data.scalar.length) == 0){ 
						yaml_parse(&parser, &event)

						printf("Repository source: %s \n", event.data.scalar.value);
						repo_src = calloc(event.data.scalar.length + 1, sizeof(char));
						memcpy(repo_src, event.data.scalar.value, event.data.scalar.length);
					}
				} else if  (map == 2 && seq == 1) { 
					if  (strncmp(event.data.scalar.value, "name", event.data.scalar.length) == 0) { 
						yaml_parse(&parser, &event)
						pkglist[pkgnum].name = calloc(event.data.scalar.length + 1, sizeof(char));
						memcpy(pkglist[pkgnum].name, event.data.scalar.value, event.data.scalar.length);
					}	
					
					else if (strncmp(event.data.scalar.value, "major", event.data.scalar.length) == 0){ 
						yaml_parse(&parser, &event)
						pkglist[pkgnum].major = atoi(event.data.scalar.value);
					}	
					else if (strncmp(event.data.scalar.value, "minor", event.data.scalar.length) == 0){ 
						yaml_parse(&parser, &event)
						pkglist[pkgnum].minor = atoi(event.data.scalar.value);
					}	
					else if (strncmp(event.data.scalar.value, "subver", event.data.scalar.length) == 0){ 
						yaml_parse(&parser, &event)
						pkglist[pkgnum].subver = atoi(event.data.scalar.value);
					}	
					else if (strncmp(event.data.scalar.value, "patch", event.data.scalar.length) == 0){ 
						yaml_parse(&parser, &event)
						pkglist[pkgnum].patch = atoi(event.data.scalar.value);
					}	
					else if (strncmp(event.data.scalar.value, "script", event.data.scalar.length) == 0){ 
						yaml_parse(&parser, &event)
						pkglist[pkgnum].script = calloc(event.data.scalar.length + 1, sizeof(char));
						memcpy(pkglist[pkgnum].script, event.data.scalar.value, event.data.scalar.length);
					}	
					else if (strncmp(event.data.scalar.value, "filename", event.data.scalar.length) == 0){ 
						yaml_parse(&parser, &event)
						pkglist[pkgnum].filename = calloc(event.data.scalar.length + 1, sizeof(char));
						memcpy(pkglist[pkgnum].filename, event.data.scalar.value, event.data.scalar.length);
					}
					
				} else if (map == 2 && seq == 2) {
					if (depnum == 0) {
						pkglist[pkgnum].deplist = malloc(sizeof(char*));
					} else {
						tmp_deplist = reallocarray(pkglist[pkgnum].deplist, depnum+1, sizeof(char *));
						if (tmp_deplist == NULL) {
							printf("Error changing size of package dependency list\n");
							exit(1); 
						} else {
							pkglist[pkgnum].deplist = tmp_deplist;
							tmp_deplist = NULL;
						}
					}

					pkglist[pkgnum].deplist[depnum] = calloc(1, event.data.scalar.length + 1);
					if (pkglist[pkgnum].deplist[depnum] == NULL) {
						printf("Could not allocate memory for dependency string\n");
						exit(1);
					}
					memcpy(pkglist[pkgnum].deplist[depnum], event.data.scalar.value,
                            event.data.scalar.length + 1);				
					depnum++;
					pkglist[pkgnum].depnum = depnum;
				}

				break;
		}
		

		if (event.type != YAML_STREAM_END_EVENT) {
			yaml_event_delete(&event);
		}
	
	} while (event.type != YAML_STREAM_END_EVENT);

	yaml_event_delete(&event);

	/* Cleanup parser */
	yaml_parser_delete(&parser);
	fclose(fpkglist);
}
