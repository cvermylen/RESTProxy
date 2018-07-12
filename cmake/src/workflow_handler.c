#include "workflow_handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stack.h>

workflow_t* workflows;

void workflow_step()
{
}

char* dup_str(char* str)
{
	char* res = (char*)malloc(sizeof(char)*(strlen(str) +1));
	strcpy(res, str);
	res[strlen(str)] = '\0';
	return res;
}

workflow_rule_t* read_rule(FILE* fd, char *buffer)
{
	workflow_rule_t* res = NULL;
	buffer = fgets(buffer, 4096, fd);
	int i, depth = 0;
	stack_head_t* stack;
	if(strcmp(buffer, "{") == 0){
		stack = stack_init();
		res = (workflow_rule_t*)malloc(sizeof(workflow_rule_t));
		buffer = fgets(buffer, 4096, fd);
		while(strcmp(buffer, "}") != 0) {
			stack_push(stack, dup_str(buffer));
		};
		workflow_instruction_t* instr = (workflow_instruction_t*)malloc(sizeof(workflow_instruction_t));
		res->instruction = instr;
		int depth = stack_depth(stack);
		res->instruction->num_lines = depth;
		res->instruction->lines = (char**)malloc(sizeof(char*)*depth);
		for(i = 0; i < depth; i++){
			res->instruction->lines[i] = stack_pop(stack);
		}
		stack_free(stack);
	}
	return res;
}

void (*init_workflow(const char* file_name))()
{
	workflows = (workflow_t*)malloc(sizeof (workflow_t) * MAX_WORKFLOWS);
	FILE* fd = fopen(file_name, "r");
	workflow_rule_t *rule = NULL;
	char * s = (char*)malloc(sizeof(char)*4096);
printf("workflow:%s\n", file_name);
if(fd==NULL)printf("FD is null\n");
	char * t;
	while((t = fgets(s, 4096, fd))){
printf("Here we go\n");
	}
	fclose(fd);
	free(s);
	return &workflow_step;
}

