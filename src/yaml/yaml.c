#include "yaml.h"
#include <yaml.h>
#include <string.h>

injectable injected_vars[10];

void inject(const char *name, const char *type, const char *value)
{
	switch(string_type_2_enum(type)){
	case CHAR_YAML:
		break;
	case STRING_YAML:
		break;
	case INT_YAML:
		break;
	case UNSIGNED_INT_YAML:
		break;
	case LONG_YAML:
		break;
	case UNSIGNED_LONG_YAML:
		break;
	case FLOAT_YAML:
		break;
	default:
		break;
	}
}

int string_type_2_enum(const char *type)
{
	if(strcmp("char *", type) == 0)
		return STRING_YAML;
	if(strcmp("char []", type) == 0)
		return STRING_YAML;
	if(strcmp("char", type) == 0)
		return CHAR_YAML;
	if(strcmp("int", type) == 0)
		return INT_YAML;
	if(strcmp("unsigned int", type) == 0)
		return UNSIGNED_INT_YAML;
	if(strcmp("long", type) == 0)
		return LONG_YAML;
	if(strcmp("unsigned long", type) == 0)
		return UNSIGNED_LONG_YAML;
	if(strcmp("float", type) == 0)
		return FLOAT_YAML;
	
	return -1;
}
