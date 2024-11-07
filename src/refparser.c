#include <stdio.h>
#include <stdlib.h>
#include <yaml.h>



int main( int argc, char** argv) {
	yaml_parser_t parser;
	yaml_event_t event;

	int done = 0;

	if (argc < 2) {
		printf( "No package list was provided\n");
	}
	
	FILE *fpkglist = fopen( argv[1], "r");
	if (fpkglist == NULL) {
		printf( "Could not open package list file: %s\n", argv[1]);
	}

	if (!yaml_parser_initialize( &parser)) {
		printf( "Failed to initalize yaml parser\n");
	}
	
	yaml_parser_set_input_file(&parser, fpkglist);


	do  {
		if (!yaml_parser_parse(&parser, &event) )  {
			printf( "YAML Parser error %d\n", parser.error);				
			exit( EXIT_FAILURE);
		}

		switch(event.type) {
			case YAML_NO_EVENT:
				printf( "No event\n");
				break;
			case YAML_STREAM_START_EVENT:
				printf( "Stream Start\n");
				break;
			case YAML_STREAM_END_EVENT:
				printf( "Stream End\n");
				break;
			/* Block delimiters */
			case YAML_DOCUMENT_START_EVENT:
				printf( "\tStart document\n");
				break;
			case YAML_DOCUMENT_END_EVENT:
				printf( "\tEnd document\n");
				break;
			case YAML_SEQUENCE_START_EVENT:
				printf( "\t\tStart Sequence\n");
				break;
			case YAML_SEQUENCE_END_EVENT:
				printf( "\t\tEnd Sequence\n");
				break;
			case YAML_MAPPING_START_EVENT:
				printf( "\t\tMapping start\n");
				break;
			case YAML_MAPPING_END_EVENT:
				printf( "\t\tMapping end\n");
				break;
			case YAML_ALIAS_EVENT:
				printf( "\t\t\tGot alias ( anchor %s)\n", event.data.alias.anchor);
				break;
			case YAML_SCALAR_EVENT:
				printf( "\t\t\tGot scalar ( value %s)\n", event.data.scalar.value);
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
	return 0;


}
