#include "gm_error.h"

static char* curr_file=NULL;
static char* curr_proc=NULL;
static bool need_print = true;
static bool parse_error = false;

void GM_set_parse_error(bool b) {parse_error = b;}
bool GM_is_parse_error() {return parse_error;}

void gm_set_current_filename(char* fname)  {curr_file = fname;}
char* gm_get_current_filename() {return curr_file;}
void gm_set_curr_procname(char* pname) {curr_proc=pname; need_print=true;}

void gm_print_error_header()
{
    if (need_print == false) return;
    if (curr_file!=NULL) printf("%s:", curr_file);
    if (curr_proc == NULL) printf("At top level:\n");
    else printf("In procedure %s:\n", curr_proc);
    need_print = false;
}


void gm_type_error(int errno, ast_id* id, const char* str1, const char* str2)
{
    gm_print_error_header();
    if (curr_file!=NULL) printf("%s:", curr_file);
    printf("%d: %d: error: ", id->get_line(), id->get_col());
    switch(errno) {
        case GM_ERROR_UNDEFINED:
            printf("%s is not defined.\n", id->get_orgname()); break;
        case GM_ERROR_UNDEFINED_FIELD:
            printf("Property name %s is not defined.\n", id->get_orgname()); break;
        //case GM_ERROR_MULTIPLE_TARGET:
        //    printf("%s is bound to multiple graphs.\n", id->get_orgname()); break;
        case GM_ERROR_PROPERTY_ARGUMENT:
            printf("Property %s cannot be defined as an output parameter.\n", id->get_orgname()); break;
        case GM_ERROR_WRONG_PROPERTY:
            printf("Property %s is not defined as %s\n", id->get_orgname(), str1); break;
        case GM_ERROR_NONGRAPH_FIELD:
            printf("%s is neither node, edge, nor graph.\n", id->get_orgname()); break;
        case GM_ERROR_READONLY:
            printf("It is not allowed to write into %s \n", id->get_orgname()); break;
        case GM_ERROR_NEED_ORDER:
            printf("Need ordered set for reverse-order iteration (%s is not)\n", id->get_orgname()); break;
        //case GM_ERROR_INVALID_ITERATOR:
        //    printf("Iterator not valid for the source %s \n", id->get_orgname()); break;

        case GM_ERROR_NEED_NODE_ITERATION:
            printf("Iteration should start from a node instead of %s\n", id->get_orgname()); break;
        case GM_ERROR_NEED_BFS_ITERATION:
            printf("Leveled Iteration should start from a bfs iterator instead of %s \n", id->get_orgname()); break;

        case GM_ERROR_NEED_ITERATOR:
            printf("%s is not a iterator name\n", id->get_orgname()); break;

        case GM_ERROR_UNKNOWN:
        default:
            printf("Unknown error 1\n"); break;
    }
}
void gm_type_error(int errno, int l, int c, const char* str1, const char* str2)
{
    gm_print_error_header();

    if (curr_file!=NULL) printf("%s:", curr_file);
    printf("%d: %d: error: ", l, c);
    switch(errno) {
        case GM_ERROR_INVALID_BUILTIN:
            printf("Invalid built-in:%s\n", str1);
            break;
        case GM_ERROR_INVALID_BUILTIN_ARG_COUNT:
            printf("Argument number mismatch for built-in:%s\n", str1);
            break;
        case GM_ERROR_INVALID_BUILTIN_ARG_TYPE:
            printf("type mismatch for built-in:%s, arg_no:%s \n", str1,str2);
            break;
        case GM_ERROR_OPERATOR_MISMATCH:
            printf("Operator %s applied to an unsupported type (%s)\n", str1, str2);break;
        case GM_ERROR_TYPE_CONVERSION:
            printf("Type conversion can be only applied to primitive types\n");break;
        case GM_ERROR_TYPE_CONVERSION_BOOL_NUM:
            printf("Type conversion cannot be  applied between numeric and boolean types\n");break;
        case GM_ERROR_TARGET_MISMATCH:
            printf("Assignment to different Graphs\n");break;
        case GM_ERROR_ASSIGN_TYPE_MISMATCH:
            printf("Typemismatch in Assignment. LHS:%s, RHS:%s \n", str1, str2 );break;
        case GM_ERROR_COMPARE_MISMATCH:
            printf("Typemismatch in Comparison. LHS:%s, RHS:%s \n", str1, str2 );break;
        case GM_ERROR_NEED_BOOLEAN:
            printf("Need boolean expression.\n"); break;
        case GM_ERROR_UNBOUND_REDUCE:
            printf("Reduce(Defer) assignment must be performed inside a foreach statement\n"); break;

        case GM_ERROR_DOUBLE_BOUND_ITOR:
            printf("Reduce(Defer) Target already bound to a different iterator: %s\n", str1); break;
        case GM_ERROR_DOUBLE_BOUND_OP:
            printf("Reduce(Defer) Target already bound to a different operator: %s\n", str1); break;
        case GM_ERROR_GRAPH_REDUCE:
            printf("Can not do reduce (defer) assignment to graph variable :%s\n", str1); break;
        case GM_ERROR_GROUP_MISMATCH:
            printf("Group assignment error (assigning node_prop into edge_prop or vice versa)\n");break;
        case GM_ERROR_RETURN_FOR_VOID:
            printf("Cannot have return value for void procedure\n"); break;
        case GM_ERROR_NO_VOID_RETURN:
            printf("Need Return Value\n"); break;
        case GM_ERROR_RETURN_MISMATCH:
            printf("Return type mismatch: required(%s), found(%s)\n",str1,str2 ); break;

        default:
            printf("Unknown error 2\n"); break;
    }
}

void gm_type_error(int errno, ast_id* id1, ast_id* id2)
{
    gm_print_error_header();
    if (curr_file!=NULL) printf("%s:", curr_file);
    printf("%d: %d: error: ", id1->get_line(),id1->get_col());
    switch(errno) {
        case GM_ERROR_NONGRAPH_TARGET:
            printf("%s is not a graph type object\n", 
                    id1->get_orgname()); break;
        case GM_ERROR_NONSET_TARGET:
            printf("%s is not a set  type object\n", 
                    id1->get_orgname()); break;
        case GM_ERROR_UNDEFINED_FIELD_GRAPH:
            printf("Property %s is not defined for graph %s.\n", id1->get_orgname(), id2->get_orgname()); break;
        case GM_ERROR_DUPLICATE:
            printf("%s is defined more than one time. (First defined in line %d : %d)\n",
                    id1->get_orgname(), id2->get_line(), id2->get_col());
            break;
        case GM_ERROR_UNKNOWN:
        default:
            assert(false);
            printf("Unknown error 3\n"); break;
    }
}

void  gm_conf_error(int errno, gm_symtab_entry* target, ast_id* ev1, ast_id* ev2, bool is_warning)
{
    gm_print_error_header();
    if (curr_file!=NULL) printf("%s:", curr_file);

    if (is_warning)
        printf("%d: %d: warn: ", ev1->get_line(), ev1->get_col());
    else
        printf("%d: %d: error: ", ev1->get_line(), ev1->get_col());

    ast_id* target_id = target->getId();
    char* name = target_id->get_orgname();

    switch(errno) {
        case GM_ERROR_READ_WRITE_CONFLICT:
            printf("Property %s may have read-write conflict: read at line:%d, write at line:%d\n", 
                   name,ev1->get_line(), ev2->get_line());
            break;
        case GM_ERROR_WRITE_WRITE_CONFLICT:
            printf("Property %s may have write-write conflict: write at line:%d, write at line:%d\n", 
                   name,ev1->get_line(), ev2->get_line());
            break;
        case GM_ERROR_READ_REDUCE_CONFLICT:
            printf("Property %s may have read-reduce conflict: read at line:%d, reduce at line:%d\n", 
                   name,ev1->get_line(), ev2->get_line());
            break;
        case GM_ERROR_WRITE_REDUCE_CONFLICT:
            printf("Property %s may have write-reduce conflict: write at line:%d, reduce at line:%d\n", 
                   name,ev1->get_line(), ev2->get_line());
            break;
        case GM_ERROR_UNKNOWN:
        default:
            assert(false);
            printf("Unknown error 3\n"); break;
    }
}

void gm_backend_error(int errno, const char* str1, const char* str2)
{
    switch(errno) {
        case GM_ERROR_FILEWRITE_ERROR:
            printf("Error: cannot open file %s for write\n", str1);
            break;
        default:
            assert(false);
            printf("Unknown error 3\n"); break;
    }
}
