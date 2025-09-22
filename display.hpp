#include "SAT.hpp"
void main_display()
{
    CNF cnf;
    bool file_exist = 0;
    int choice = 1;
    char file_name[100];
    while (choice)
    {
        display_menu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // SAT求解
        {
            int ch = 0;
            if (!file_exist)
            {
                do
                {
                    printf("请输入文件地址\n");
                    scanf("%s", file_name);
                    file_exist = 1;
                } while (!read_file(file_name, cnf)); // 检查cnf文件是否存在，顺便初始化cnf链表
            }
            else
            {
                printf("已经存在cnf文件,是否覆盖?[1/0]\n");
                scanf("%d", &ch);
                if (ch == 1)
                {
                    destroy_cnf(cnf);
                    do
                    {
                        printf("请输入文件地址\n");
                        scanf("%s", file_name);
                    } while (!read_file(file_name, cnf));
                }
            }
            printf("需要我遍历一遍cnf文件吗[1/0]\n");
            scanf("%d", &ch);
            if (ch)
            {
                print_cnf(cnf);
                getchar();
                getchar();
            }
            // printf("求解中...\n");
            // CNF cnf_new = new cnf_node;
            // cnf_new->root = copy_cnf(cnf->root);
            // cnf_new->bool_count = cnf->bool_count;
            // cnf_new->clause_count = cnf->clause_count;
            // bool *value = new bool[cnf->bool_count + 1];
            // for (int i = 1; i <= cnf->bool_count; i++)
            //     value[i] = 1;

            printf("尝试在最小子句找出现次数最多的字并求解...\n");
            bool *value = new bool[cnf->bool_count + 1];
            for (int i = 1; i <= cnf->bool_count; ++i)
                value[i] = 1;
            double t1 = 0.0, t2 = 0.0, t3 = 0.0;
            int res1 = solve_with_timeout_flag(cnf, value, 1, 30.0, t1);
            printf("尝试使用MOMS算法并求解...\n");
            int res2 = solve_with_timeout_flag(cnf, value, 2, 30.0, t2);
            printf("尝试取第一个单词并求解...\n");
            int res3 = solve_with_timeout_flag(cnf, value, 2, 30.0, t3);
            printf("flag1 (method1) time: %.6f s, result=%d\n", t1, res1);
            printf("flag2 (method2) time: %.6f s, result=%d\n", t2, res2);
            printf("flag2 (method2) time: %.6f s, result=%d\n", t3, res3);

            double optimization_rate = 0.0;
            int faster_flag = 0;
            double faster_time = 0.0, slower_time = 0.0;

            // 判断哪个方法有有效结果（res==1 或 res==0 表示有结论）
            if (!(res1 == res2 && res1 == -1 && res3 != -1)) // 只要有算出结果
            {
                faster_time = MIN(MIN(t1, t2), t3);
                if (faster_time == t1)
                    faster_flag = 1;
                if (faster_time == t2)
                    faster_flag = 2;
                if (faster_time == t3)
                    faster_flag = 3;
                slower_time = MAX(MAX(t1, t2), t3);
                optimization_rate = ((slower_time - faster_time) / slower_time) * 100;
                if (faster_flag == 1)
                    printf("更快的方法: 在最小子句找出现次数最多的字, time=%.6f s\n", faster_time);
                else if (faster_flag == 2)
                    printf("更快的方法: MOMS算法, time=%.6f s\n", faster_time);
                else if (faster_flag == 3)
                    printf("更快的方法: 取第一个单词, time=%.6f s\n", faster_time);

                if (res1 != -1 && res2 != -1 && res3 != -1)
                    printf("优化率: %.2f%%\n", optimization_rate);
                else
                    printf("优化率:100%%\n");
                printf("是否要保存到文件中?[1/0]\n");
                scanf("%d", &ch);
                if (ch)
                {
                    int result = t1 < t2 ? res1 : res2;
                    if (save_file(result, file_name, slower_time, value, cnf->bool_count, faster_time))
                        printf("保存成功!\n");
                    else
                        printf("保存失败\n");
                }
            }
            else
            {
                printf("两种方法均无有效结论。\n");
            }
            printf("\n");
            delete[] value;
            break;
        }
        case 2: // 百分号数独游戏
            main_sudoku();
            break;
        case 0: // Exit
            return;
        default:
            printf("指令无效\n");
            break;
        }
    }
    return;
}
void display_menu()
{
    printf("\n|--------------------------------------------|\n");
    printf("|----------------请给出你的选择--------------|\n");
    printf("|--------------------------------------------|\n\n");
    printf("|==================Main Menu=================|\n");
    printf("|--------------------------------------------|\n");
    printf("|            1. SAT求解                      |\n");
    printf("|            2. 百分号数独游戏               |\n");
    printf("|            0.  EXIT                        |\n");
    printf("|============================================|\n\n");
    printf("请选择[1/2/0]\n");
    return;
}
void display_sudoku()
{
    printf("|**************Menu for Percent_Sudoku*******|\n");
    printf("|--------------------------------------------|\n");
    printf("|               1. 生成数独                  |\n");
    printf("|               2. 查看答案                  |\n");
    printf("|               3. 游玩数独                  |\n");
    printf("|               0. EXIT                      |\n");
    printf("|********************************************|\n\n");
    printf("请选择[1/2/3/0]\n");
    return;
}