#include "SAT.hpp"
bool read_file(char file_name[], CNF &cnf)
{
    FILE *fp = fopen(file_name, "r");
    if (!fp)
    {
        printf("文件打开失败: %s\n", file_name);
        return 0;
    }
    cnf = new cnf_node;
    char ch;
    while ((ch = getc(fp)) == 'c')
    {
        while ((ch = getc(fp)) != '\n')
            continue;
    }
    getc(fp);
    getc(fp);
    getc(fp);
    getc(fp);
    fscanf(fp, "%d%d", &cnf->bool_count, &cnf->clause_count);
    if (cnf->bool_count <= 0 || cnf->clause_count <= 0)
    {
        printf("CNF header 未找到或无效\n");
        fclose(fp);
        free(cnf);
        return 0;
    }
    cnf->root = NULL;

    clause_list last_clause = NULL;
    for (int ci = 0; ci < cnf->clause_count; ++ci)
    {
        clause_list cl = new clause_node;
        cl->head = NULL;
        cl->next = NULL;

        literal_list last_lit = NULL;
        int lit;
        while (fscanf(fp, "%d", &lit) == 1)
        {
            if (lit == 0)
                break;

            literal_list ln = new literal_node;
            ln->literal = lit;
            ln->next = NULL;

            if (last_lit == NULL)
                cl->head = ln;
            else
                last_lit->next = ln;
            last_lit = ln;
        }
        if (cnf->root == NULL)
        {
            cnf->root = cl;
            last_clause = cl;
        }
        else
        {
            last_clause->next = cl;
            last_clause = cl;
        }
    }

    fclose(fp);
    printf("读取成功！\n");
    return 1;
}
void destroy_cnf(CNF &cnf)
{
    while (cnf->root)
    {
        clause_list temp = cnf->root;
        literal_list delete_node = temp->head;
        while (delete_node)
        {
            literal_list p = delete_node;
            delete_node = delete_node->next;
            delete p;
        }
        cnf->root = cnf->root->next;
        delete temp;
    }
    cnf->root = NULL;
    return;
}
void print_cnf(CNF cnf)
{
    clause_list p = cnf->root;
    if (p == NULL) // 如果没有子句
    {
        printf(" 没有句子\n");
        return;
    }
    printf("  The CNF is:\n");
    printf("  boolCount:%d\n", cnf->bool_count);
    printf("  clauseCount:%d\n", cnf->clause_count);
    while (p)
    {
        literal_list q = p->head;
        printf("  ");
        while (q)
        {
            printf("%-5d", q->literal);
            q = q->next;
        }
        printf("0\n");
        p = p->next;
    }
    return;
}