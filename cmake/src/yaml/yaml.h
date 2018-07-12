#ifndef YAML_H
#define YAML_H

#define CHAR_YAML		1
#define STRING_YAML		2
#define INT_YAML		3
#define UNSIGNED_INT_YAML	4
#define LONG_YAML		5
#define UNSIGNED_LONG_YAML	6
#define FLOAT_YAML		7

typedef struct _injectable {
	char * property_name;
	int type;
	union ptr {
		char * c;
		int * i;
		unsigned int * ui;
		long * l;
		unsigned long * ul;
		float * f;
	} value;
} injectable;

void inject(const char *name, const char *type, const char *value);

int string_type_2_enum(const char *type);

#endif
