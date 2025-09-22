#include "SAT.hpp"

int find_unit_clause(clause_list cl) // 找到单子句
{
    while (cl)
    {
        if (cl->head != NULL && cl->head->next == NULL)
            return cl->head->literal;
        cl = cl->next;
    }
    return 0;
}
void remove_clause(clause_list &cl) // 删除子句
{
    literal_list temp = cl->head;
    while (temp)
    {
        literal_list p = temp;
        temp = temp->next;
        delete (p);
    }
    delete (cl);
    cl = NULL;
    return;
}
void simplify(clause_list &cl, int lit) // 化简子句
{
    clause_list pre = NULL, p = cl;
    while (p)
    {
        bool is_deleted = 0;
        literal_list lpre = NULL, q = p->head;
        while (q)
        {
            if (q->literal == lit) // 文字等于 literal
            {
                if (!pre)
                {
                    cl = cl->next;
                    remove_clause(p);
                    p = cl;
                }
                else
                {
                    pre->next = p->next;
                    remove_clause(p);
                    p = pre->next;
                }
                is_deleted = 1;
                break;
            }
            else if (q->literal == -lit) // 文字等于 -literal
            {
                if (lpre)
                {
                    lpre->next = q->next;
                    delete (q);
                    q = lpre->next;
                }
                else
                {
                    p->head = q->next;
                    delete (q);
                    q = p->head;
                }
            }
            else // 啥都没有
            {
                lpre = q;
                q = q->next;
            }
        }
        if (!is_deleted)
        {
            pre = p;
            p = p->next;
        }
    }
}

clause_list copy_cnf(clause_list cl)
{
    clause_list new_cnf = new clause_node;
    clause_list cl_new = new_cnf, cl_old = cl;
    while (cl_old)
    {
        literal_list lit_new;
        literal_list lit_old = cl_old->head;
        if (lit_old)
        {
            lit_new = new literal_node;
            lit_new->literal = lit_old->literal;
            lit_new->next = NULL;
            cl_new->head = lit_new;
            lit_old = lit_old->next;
        }
        while (lit_old)
        {
            lit_new->next = new literal_node;
            lit_new = lit_new->next;
            lit_new->literal = lit_old->literal;
            lit_new->next = NULL;
            lit_old = lit_old->next;
        }
        if (cl_old->next)
        {
            cl_new->next = new clause_node;
            cl_new = cl_new->next;
            cl_new->next = NULL;
            cl_new->head = NULL;
        }
        cl_old = cl_old->next;
    }
    return new_cnf;
}
bool is_satisfy(clause_list cl) // 判断cnf是否可以满足
{
    if (!cl)
        return 1;
    return 0;
}
bool is_empty_clause(clause_list cl) // 判断有没有空子句
{
    while (cl)
    {
        if (!cl->head)
            return 1;
        cl = cl->next;
    }
    return 0;
}
int choose_literal_1(CNF cnf) // 没有优化的选择
{
    return cnf->root->head->literal;
}
int choose_literal_2(CNF cnf) // 找出现最多的字
{

    int *value = new int[cnf->bool_count * 2 + 1];
    for (int i = 0; i < cnf->bool_count * 2 + 1; i++)
        value[i] = 0;
    for (clause_list cl = cnf->root; cl != NULL; cl = cl->next)
    {
        for (literal_list lit = cl->head; lit != NULL; lit = lit->next)
        {
            if (lit->literal > 0)
                value[lit->literal]++;
            else
                value[cnf->bool_count - lit->literal]++;
        }
    }
    int max = 0, max_lit;
    for (int i = 0; i < cnf->bool_count; i++)
    {
        if (value[i] > max)
        {
            max = value[i];
            max_lit = i;
        }
    }
    if (max == 0)
    {
        for (int i = cnf->bool_count; i < cnf->bool_count * 2; i++)
        {
            if (value[i] > max)
            {
                max = value[i];
                max_lit = cnf->bool_count - i;
            }
        }
    }
    delete[] value;
    return max_lit;
}

int choose_literal_3(CNF cnf) // 在最小子句找出现次数最多的字
{

    clause_list max_cl;
    int *value = new int[cnf->bool_count * 2 + 1];
    for (int i = 0; i <= cnf->bool_count * 2; i++)
        value[i] = 0;
    int max = INT_MAX;
    for (clause_list cl = cnf->root; cl; cl = cl->next)
    {
        int count = 0;
        literal_list lit = cl->head;
        while (lit)
        {
            count++;
            lit = lit->next;
        }
        if (count < max)
        {
            max = count;
            max_cl = cl;
        }
    }
    for (literal_list lit = max_cl->head; lit != NULL; lit = lit->next)
    {
        value[lit->literal + cnf->bool_count]++;
    }
    max = 0;
    int max_lit;
    for (int i = 0; i < cnf->bool_count * 2 + 1; i++)
    {
        if (value[i] > max)
        {
            max = value[i];
            max_lit = i - cnf->bool_count;
        }
    }
    delete[] value;
    return max_lit;
}

bool save_file(int result, char file_name[], double time, bool value[], int bool_count, double time_)
{
    FILE *fp;
    char name[100];
    for (int i = 0; file_name[i] != '\0'; i++)
    {
        if (file_name[i] == '.' && file_name[i + 4] == '\0')
        {
            name[i] = '.';
            name[i + 1] = 'r';
            name[i + 2] = 'e';
            name[i + 3] = 's';
            name[i + 4] = '\0';
            break;
        }
        name[i] = file_name[i];
    }
    if (fopen_s(&fp, name, "w"))
    {
        printf("保存失败\n");
        return 0;
    }
    fprintf(fp, "s %d", result);
    if (result == 1)
    {
        fprintf(fp, "\nv\n");
        for (int i = 1; i <= bool_count; i++)
        {
            if (value[i] == 1)
                fprintf(fp, "%d ", i);
            else
                fprintf(fp, "%d ", -i);
        }
    }
    fprintf(fp, "\nt %lfms", time * 1000);
    if (time_ != 0)
    {
        fprintf(fp, "\nt %lfms(optimized)", time_ * 1000);        // 运行时间/毫秒
        double optimization_rate = ((time - time_) / time) * 100; // 优化率
        fprintf(fp, "\nOptimization Rate: %.2lf%%", optimization_rate);
    }
    fclose(fp);
    return 1;
}

int choose_literal_4(CNF cnf) // MOMS
{
    if (!cnf || !cnf->root)
        return 0;
    int V = cnf->bool_count;
    int m = INT_MAX;
    // 找最短子句长度
    for (clause_list c = cnf->root; c != NULL; c = c->next)
    {
        int len = 0;
        for (literal_list p = c->head; p != NULL; p = p->next)
            ++len;
        if (len > 0 && len < m)
            m = len;
    }
    if (m == INT_MAX)
        return 0;

    int *cnt_pos = new int[V + 1]();
    int *cnt_neg = new int[V + 1]();
    for (clause_list c = cnf->root; c != NULL; c = c->next)
    {
        int len = 0;
        for (literal_list p = c->head; p != NULL; p = p->next)
            ++len;
        if (len != m)
            continue;
        for (literal_list p = c->head; p != NULL; p = p->next)
        {
            int L = p->literal;
            if (L > 0)
                cnt_pos[L]++;
            else
                cnt_neg[-L]++;
        }
    }

    int best_v = 0, best_total = -1;
    for (int v = 1; v <= V; ++v)
    {
        int tot = cnt_pos[v] + cnt_neg[v];
        if (tot > best_total)
        {
            best_total = tot;
            best_v = v;
        }
    }

    int res = 0;
    if (best_total > 0)
        res = (cnt_pos[best_v] >= cnt_neg[best_v]) ? best_v : -best_v;
    delete[] cnt_pos;
    delete[] cnt_neg;
    return res;
}
// ==== 全局活动度数组 ====
double *g_activity = NULL;
int g_activity_size = 0;

// 初始化 VSIDS 分数：统计出现次数
void vsids_init_from_cnf(CNF cnf)
{
    int V = cnf->bool_count;
    if (g_activity)
        delete[] g_activity;
    g_activity = new double[V + 1];
    g_activity_size = V;
    for (int i = 0; i <= V; i++)
        g_activity[i] = 0.0;

    for (clause_list c = cnf->root; c != NULL; c = c->next)
    {
        for (literal_list p = c->head; p != NULL; p = p->next)
        {
            g_activity[abs(p->literal)] += 1.0;
        }
    }
}

// 用静态 VSIDS 分数选择 literal
int choose_literal_vsids_static(CNF cnf) // https://www.researchgate.net/publication/279633448_Understanding_VSIDS_Branching_Heuristics_in_Conflict-Driven_Clause-Learning_SAT_Solvers
{
    if (!cnf || !cnf->root)
        return 0;
    if (!g_activity)
        return 0;

    int best_v = 0;
    double best = -1.0;
    int V = cnf->bool_count;

    // 1. 找出现频率最高且仍在 CNF 中的变量
    for (int v = 1; v <= V; v++)
    {
        if (g_activity[v] <= 0.0)
            continue;

        bool appears = false;
        for (clause_list c = cnf->root; c != NULL && !appears; c = c->next)
        {
            for (literal_list p = c->head; p != NULL; p = p->next)
            {
                if (abs(p->literal) == v)
                {
                    appears = true;
                    break;
                }
            }
        }
        if (!appears)
            continue;

        if (g_activity[v] > best)
        {
            best = g_activity[v];
            best_v = v;
        }
    }

    if (best_v == 0)
        return 0; // 没有找到

    // 2. polarity 选择：统计正负出现次数
    int pos = 0, neg = 0;
    for (clause_list c = cnf->root; c != NULL; c = c->next)
    {
        for (literal_list p = c->head; p != NULL; p = p->next)
        {
            if (abs(p->literal) == best_v)
            {
                if (p->literal > 0)
                    pos++;
                else
                    neg++;
            }
        }
    }

    return (pos >= neg) ? best_v : -best_v;
}

bool DPLL(CNF cnf, bool value[], int flag) // 主要的东西
{
    int unit_lit = find_unit_clause(cnf->root);
    while (unit_lit != 0)
    {
        value[abs(unit_lit)] = (unit_lit > 0) ? 1 : 0;
        simplify(cnf->root, unit_lit);
        if (is_satisfy(cnf->root))
            return 1;
        if (is_empty_clause(cnf->root))
            return 0;
        unit_lit = find_unit_clause(cnf->root);
    }
    if (is_empty_clause(cnf->root))
        return 0;
    int lit = choose_literal_3(cnf);
    // if (flag == 1)
    //     lit = choose_literal_1(cnf); // 直接拿第一个子句的第一个单词
    // else if (flag == 2)
    //     lit = choose_literal_2(cnf); // 找出现最多的字
    // else if (flag == 3)
    //     lit = choose_literal_3(cnf); // 在最小子句找出现次数最多的字
    // else if (flag == 4)
    //     lit = choose_literal_moms(cnf);

    // 假设TRUE
    CNF cnf_new = new cnf_node;
    cnf_new->root = copy_cnf(cnf->root);
    cnf_new->bool_count = cnf->bool_count;
    cnf_new->clause_count = cnf->clause_count;

    clause_list cl_new = new clause_node;
    cl_new->head = new literal_node;
    cl_new->head->next = NULL;
    cl_new->head->literal = lit;
    cl_new->next = cnf_new->root;
    cnf_new->root = cl_new;
    if (DPLL(cnf_new, value, flag) == 1)
        return 1;
    destroy_cnf(cnf_new);
    // 假设FALSE
    clause_list cl_new2 = new clause_node;
    cl_new2->head = new literal_node;
    cl_new2->head->next = NULL;
    cl_new2->head->literal = -lit;
    cl_new2->next = cnf->root;
    cnf->root = cl_new2;
    return DPLL(cnf, value, flag);
}

int DPLL_2(CNF cnf, bool value[], int flag, const std::chrono::steady_clock::time_point &start, double timeout_seconds)
{
    auto timed_out = [&](void) -> bool
    {
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - start).count();
        return elapsed >= timeout_seconds;
    };
    if (timed_out())
        return -1;
    int unit_lit = find_unit_clause(cnf->root);
    while (unit_lit != 0)
    {
        if (timed_out())
            return -1;
        value[abs(unit_lit)] = (unit_lit > 0) ? 1 : 0;
        simplify(cnf->root, unit_lit);
        if (is_satisfy(cnf->root))
            return 1;
        if (is_empty_clause(cnf->root))
            return 0;
        unit_lit = find_unit_clause(cnf->root);
    }

    if (is_empty_clause(cnf->root))
        return 0;

    int lit;
    // if (flag == 1)
    //     lit = choose_literal_1(cnf);
    // else if (flag == 2)
    //     lit = choose_literal_2(cnf);
    if (flag == 1)
        lit = choose_literal_3(cnf);
    else if (flag == 2)
        lit = choose_literal_4(cnf);
    else if (flag == 3)
        lit = choose_literal_1(cnf);
    if (timed_out())
        return -1;

    CNF cnf_new = new cnf_node;
    cnf_new->root = copy_cnf(cnf->root);
    cnf_new->bool_count = cnf->bool_count;
    cnf_new->clause_count = cnf->clause_count;

    clause_list cl_new = new clause_node;
    cl_new->head = new literal_node;
    cl_new->head->next = NULL;
    cl_new->head->literal = lit;
    cl_new->next = cnf_new->root;
    cnf_new->root = cl_new;

    int res = DPLL_2(cnf_new, value, flag, start, timeout_seconds);
    if (res == 1)
    {
        destroy_cnf(cnf_new);
        return 1;
    }
    if (res == -1)
    {
        destroy_cnf(cnf_new);
        return -1;
    }
    destroy_cnf(cnf_new);

    // 假设FALSE
    clause_list cl_new2 = new clause_node;
    cl_new2->head = new literal_node;
    cl_new2->head->next = NULL;
    cl_new2->head->literal = -lit;
    cl_new2->next = cnf->root;
    cnf->root = cl_new2;

    return DPLL_2(cnf, value, flag, start, timeout_seconds);
}

int solve_with_timeout_flag(CNF cnf, bool value[], int flag, double timeout_seconds, double &elapsed_seconds)
{
    auto _start = std::chrono::steady_clock::now();
    // 复制 CNF 给 DPLL 使用
    CNF cnf_copy = new cnf_node;
    cnf_copy->root = copy_cnf(cnf->root);
    cnf_copy->bool_count = cnf->bool_count;
    cnf_copy->clause_count = cnf->clause_count;

    // 重置 assignment 为未赋值（0）
    for (int i = 1; i <= cnf->bool_count; ++i)
        value[i] = 1;

    int res = DPLL_2(cnf_copy, value, flag, _start, timeout_seconds);

    auto _end = std::chrono::steady_clock::now();
    elapsed_seconds = std::chrono::duration<double>(_end - _start).count();

    destroy_cnf(cnf_copy);
    return res; // 1=SAT, 0=UNSAT, -1=TIMEOUT
}