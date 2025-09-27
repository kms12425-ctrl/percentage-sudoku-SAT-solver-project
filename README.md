
[English](#english-version) | [中文](#百分号数独与-sat-求解工具)

# 百分号数独与 SAT 求解工具

## 项目简介
本项目提供了一个命令行工具 `main.exe`，用于生成、游玩和求解“百分号数独”谜题，并支持将数独转化为 SAT 问题（DIMACS CNF 格式）及自动求解。适合数独爱好者、算法学习者和 SAT 研究者使用。

## 功能特性
- 生成带百分号约束（撇对角线+两个3×3窗口）的数独谜题
- 支持手动游玩、自动求解、查看答案
- 支持将数独导出为 CNF 文件，或对任意 CNF 文件进行 SAT 求解
- 批量处理 test 目录下的 .cnf 文件，自动输出结果和性能统计
- 结果可保存为 .res 文件，便于验证

## 快速开始
1. **交互模式**：
   - 直接双击 `main.exe` 或命令行运行 `main.exe`，进入菜单，按提示操作。
   - 首次游玩请选择“生成百分号数独”，可体验手动输入和自动求解。
2. **命令行模式**：
   - 求解单个 CNF 文件：
     ```
     main.exe test\1.cnf
     ```
   - 批量求解：
     ```
     main.exe test\*.cnf
     ```
   - 保存解答：在主界面输入 1 可保存 .res 文件。

## 文件格式说明
- `.cnf`：标准 DIMACS 格式，首行为 `p cnf 变量数 子句数`，后续为以 0 结尾的子句。
- `.res`：SAT 求解结果，内容为 SAT/UNSAT 及变量赋值。
- `percent_sudoku.cnf`：自动生成的百分号数独 SAT 文件。

## 命令行参数
- 无参数：进入交互菜单
- 输入 .cnf 文件：自动求解并输出结果

## 常见问题
- 游玩时可输入“0 0”查看答案，或主界面选择“查看答案”
- 输入行列（如 3 3）和数字（如 5）进行填数
- 若遇到“UNSAT”，请检查谜题输入或约束是否矛盾
- 支持批量处理和性能统计，适合算法实验

## 致谢
- 本工具基于经典数独与 SAT 求解算法开发
- 感谢开源 SAT/DPLL 相关资料与数独社区

如需详细操作说明，请参见 `操作手册.txt`。

---

## English Version
<a name="english-version"></a>

# Percent Sudoku & SAT Solver Tool

## Project Overview
This project provides a command-line tool `main.exe` for generating, playing, and solving "Percent Sudoku" puzzles, as well as converting Sudoku to SAT problems (DIMACS CNF format) and solving them automatically. It is suitable for Sudoku enthusiasts, algorithm learners, and SAT researchers.

## Features
- Generate Percent Sudoku puzzles (anti-diagonal + two 3×3 windows constraints)
- Support manual play, auto-solve, and answer reveal
- Export Sudoku as CNF files, or solve any CNF file as a SAT problem
- Batch process `.cnf` files in the `test` directory with automatic result and performance output
- Save results as `.res` files for verification

## Quick Start
1. **Interactive Mode:**
   - Double-click `main.exe` or run `main.exe` in the command line to enter the menu and follow the prompts.
   - For first-time play, select "Generate Percent Sudoku" to experience manual input and auto-solve.
2. **Command Line Mode:**
   - Solve a single CNF file:
     ```
     main.exe test\1.cnf
     ```
   - Batch solve:
     ```
     main.exe test\*.cnf
     ```
   - Save solution: Enter `1` in the main menu to save as a `.res` file.

## File Formats
- `.cnf`: Standard DIMACS format, first line is `p cnf [variables] [clauses]`, followed by 0-terminated clauses.
- `.res`: SAT solving result, containing SAT/UNSAT and variable assignments.
- `percent_sudoku.cnf`: Auto-generated Percent Sudoku SAT file.

## Command Line Arguments
- No arguments: Enter interactive menu
- Input a `.cnf` file: Auto-solve and output result

## FAQ
- During play, enter `0 0` to view the answer, or select "View Answer" in the main menu
- Enter row and column (e.g., `3 3`) and a number (e.g., `5`) to fill a cell
- If you get "UNSAT", check for input or constraint conflicts
- Supports batch processing and performance statistics, suitable for algorithm experiments

## Acknowledgements
- This tool is based on classic Sudoku and SAT solving algorithms
- Thanks to open-source SAT/DPLL resources and the Sudoku community

For detailed instructions, see `操作手册.txt` (Chinese manual).
