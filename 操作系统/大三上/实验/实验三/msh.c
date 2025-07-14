#include <stdio.h>          // 标准输入输出函数
#include <stdlib.h>         // 标准库函数，如 malloc 和 exit
#include <string.h>         // 字符串操作函数
#include <unistd.h>         // POSIX 操作系统 API，如 fork 和 execvp
#include <sys/wait.h>       // 等待子进程的函数
#include <signal.h>         // 信号处理函数
#include <readline/readline.h>  // 用于命令行输入的 readline 库
#include <readline/history.h>   // 用于命令历史记录的 history 库
#include <fcntl.h>          // 文件控制选项

#define MAX_CMD_LENGTH 1024 // 最大命令长度
#define HISTORY_SIZE 30     // 历史记录大小

// 处理 SIGINT 信号（Ctrl+C）
void handle_sigint(int sig) {
    printf("\nInterrupted by user.\n");
    rl_on_new_line();       // 让 readline 重新显示提示符
    rl_replace_line("", 0); // 清空当前输入行
    rl_redisplay();         // 重新显示提示符
    signal(SIGINT, handle_sigint);
}

// 解析命令行输入
int parse_command(char *line, char ***args, int *input_fd, int *output_fd, int *pipe_fd, int *background) {
    *input_fd = STDIN_FILENO;  // 默认输入为标准输入
    *output_fd = STDOUT_FILENO; // 默认输出为标准输出
    *pipe_fd = -1;              // 默认没有管道
    *background = 0;            // 默认不在后台运行

    char *token = strtok(line, " \t\n"); // 使用空格、制表符和换行符分割命令行
    int argc = 0;                       // 命令参数计数器

    while (token != NULL) {
        if (strcmp(token, "<") == 0) {  // 输入重定向
            token = strtok(NULL, " \t\n");
            if (token) {
                *input_fd = open(token, O_RDONLY); // 打开文件作为输入
                if (*input_fd == -1) {
                    perror("open"); // 打开文件失败
                    return -1;
                }
            } else {
                fprintf(stderr, "msh: missing file operand after '<'\n");
                return -1;
            }
        } else if (strcmp(token, ">") == 0) { // 输出重定向
            token = strtok(NULL, " \t\n");
            if (token) {
                *output_fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0644); // 打开文件作为输出
                if (*output_fd == -1) {
                    perror("open"); // 打开文件失败
                    return -1;
                }
            } else {
                fprintf(stderr, "msh: missing file operand after '>'\n");
                return -1;
            }
        } else if (strcmp(token, "|") == 0) { // 管道符
            if (*pipe_fd == -1) {
                *pipe_fd = argc; // 记录管道符的位置
            } else {
                fprintf(stderr, "msh: too many pipes\n");
                return -1;
            }
        } else if (strcmp(token, "&") == 0) { // 后台运行
            *background = 1;
        } else {
            (*args)[argc++] = token; // 添加命令参数
        }
        token = strtok(NULL, " \t\n"); // 获取下一个分割字符段
    }
    (*args)[argc] = NULL; // 结束命令参数数组

    return argc; // 返回命令参数数量
}

// 执行命令
void execute_command(char **args, int input_fd, int output_fd, int background) {
    pid_t pid;
    int status;

    if ((pid = fork()) == -1) { // 创建子进程
        perror("fork");
        exit(1);
    } else if (pid == 0) { // 子进程
        if (input_fd != STDIN_FILENO) {
            close(STDIN_FILENO);
            dup2(input_fd, STDIN_FILENO); // 将输入重定向为 input_fd
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            close(STDOUT_FILENO);
            dup2(output_fd, STDOUT_FILENO); // 将输出重定向为 output_fd
            close(output_fd);
        }

        if (execvp(args[0], args) == -1) { // 执行命令
            perror("execvp");
            exit(1);
        }
    } else { // 父进程
        if (!background) { // 如果不是后台运行
            do {
                waitpid(pid, &status, WUNTRACED); // 等待子进程结束
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
}

int main() {
    char *line = NULL;         // 用户输入的命令行
    char **args = NULL;        // 命令参数数组
    char *prompt = "#";        // 提示符
    int should_run = 1;        // 是否继续运行 shell
    int input_fd, output_fd, pipe_fd, background;

    signal(SIGINT, handle_sigint); // 设置 SIGINT 信号处理器

    using_history();              // 使用命令历史记录
    read_history("msh_history");  // 读取历史记录文件

    while (should_run) {
        background = 0;           // 初始化背景标志
        if (isatty(STDIN_FILENO)) {
            printf("%s", prompt);   //检查是否来自终端，判断是否是管道输入
        }
        line = readline("");      // 读取用户输入

        if (line) {
            add_history(line);    // 添加到历史记录
            write_history("msh_history"); // 写入历史记录文件

            args = (char **)malloc(MAX_CMD_LENGTH * sizeof(char *)); // 分配内存给命令参数数组
            int argc = parse_command(line, &args, &input_fd, &output_fd, &pipe_fd, &background); // 解析命令

            if (argc == -1) { // 解析失败
                free(line);
                free(args);
                continue;
            }

            if (strcmp(args[0], "exit") == 0) { // 处理 exit 命令，args[0]保存开头第一个单词
                should_run = 0;
            } else if (args[0]) { // 处理其他命令
                if (pipe_fd != -1) { // 处理管道命令
                    int pipe_fds[2];
                    if (pipe(pipe_fds) == -1) { // 创建管道
                        perror("pipe");
                        free(line);
                        free(args);
                        continue;
                    }

                    pid_t pid = fork();
                    if (pid == -1) { // 创建子进程
                        perror("fork");
                        close(pipe_fds[0]);
                        close(pipe_fds[1]);
                        free(line);
                        free(args);
                        continue;
                    } else if (pid == 0) { // 处理管道左部分
                        close(pipe_fds[0]); // 关闭读端
                        close(STDOUT_FILENO);
                        dup2(pipe_fds[1], STDOUT_FILENO); // 标准输出改为管道
                        close(pipe_fds[1]);

                        char **left_args = args;
                        for (int i = pipe_fd; args[i] != NULL; i++) {
                            left_args[i] = NULL; // 将从 pipe_fd 开始的所有元素设置为 NULL
                        }

                        execute_command(left_args, input_fd, STDOUT_FILENO, background);
                        exit(0);
                    } else { // 父进程
                        waitpid(pid, NULL, 0); // 等待左部分子进程结束
                        close(pipe_fds[1]); // 关闭写端

                        pid_t pid2 = fork();
                        if (pid2 == -1) { // 创建子进程
                            perror("fork");
                            close(pipe_fds[0]);
                            free(line);
                            free(args);
                            continue;
                        } else if (pid2 == 0) { // 处理管道右部分
                            close(pipe_fds[1]); // 再次关闭写端
                            close(STDIN_FILENO);
                            dup2(pipe_fds[0], STDIN_FILENO); // 标准输入来自管道
                            close(pipe_fds[0]);

                            char **right_args = args + pipe_fd; // 获取右部分命令数组
                            execute_command(right_args, STDIN_FILENO, output_fd, background);
                            exit(0);
                        } else { // 父进程
                            waitpid(pid2, NULL, 0); // 等待右部分子进程结束
                            close(pipe_fds[0]);
                        }
                    }
                } else { // 处理非管道命令
                    execute_command(args, input_fd, output_fd, background);
                }
            }

            free(line); // 释放命令行内存
            free(args); // 释放命令参数数组内存
        }
        else{
            if (!isatty(STDIN_FILENO)) should_run=0;
        }
    }

    return 0;
}