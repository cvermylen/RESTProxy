#ifndef WORKFLOW_HANDLER_H
#define WORKFLOW_HANDLER_H

#define MAX_WORKFLOWS	10

typedef struct {
	int num_lines;
	char **lines;
}workflow_instruction_t;

typedef struct workflow_rule workflow_rule_t;

struct workflow_rule {
	workflow_instruction_t *instruction;
	workflow_rule_t *next;
};

typedef struct {
	workflow_rule_t *first;
}workflow_t;

void (*init_workflow(const char *file_name))();

#endif
