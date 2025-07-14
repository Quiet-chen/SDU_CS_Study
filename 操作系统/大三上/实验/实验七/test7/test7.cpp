#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

// 定义颜色和样式宏
#define CLOSE "\001\033[0m\002"                   // 关闭所有属性
#define BLOD "\001\033[1m\002"                    // 强调、加粗、高亮
#define BEGIN(x, y) "\001\033[" #x ";" #y "m\002" // x: 背景，y: 前景
#define MAX_LINE 100                              // 限定用户输入的命令行长度不超过100字符
#define MAX_PARA 30                               // 限定用户输入命令的参数个数不超过30

using namespace std;

string Input;

void Command(string Input)
{
    stringstream ss(Input);
    string tmp;
    vector<string> para;
    while (getline(ss, tmp, ' ')) // 将命令的每个字符串都分别存储
        para.push_back(tmp);
    
    char *args[MAX_PARA + 1] = {};
    char **temp = new char *[MAX_PARA + 1]; // 二维数组，tmp[i]是一个字符串
    for (int i = 0; i < para.size(); i++)   // 开辟空间，否则strcpy时只有指针，会报段错误
        temp[i] = new char[MAX_PARA + 1];
    
    for (int i = 0; i < para.size(); i++)
    {
        strcpy(temp[i], para[i].c_str()); // 将string转化到char*
        args[i] = temp[i];
    }
    args[para.size()] = nullptr; // 终止符
    
    execvp(args[0], args); // 使用参数+文件的exec
    perror("Command execution failed"); // 输出错误信息
    exit(1); // 必须要退出，否则子进程会循环创建自己的子进程
}

void process()
{
    while (1)
    {
        char *cmd = readline(BEGIN(49, 36) BLOD "orange_Shell~: " CLOSE);
        if (cmd == nullptr) {
            continue;
        }
        add_history(cmd);
        write_history("command_history.txt");
        
        int background = 0; /* 如果命令行的最后字符是&，则 background=1 */
        Input = cmd;
        const char *tmm = "quit";
        if (strcmp(Input.c_str(), tmm) == 0)
            break;
        
        if (Input[0] == '\r' || Input[0] == '\n')
            continue;
        
        stringstream ss(Input);
        string tmp;
        char buffer[65536];
        vector<string> para;
        while (getline(ss, tmp, ' ')) // 将命令的每个字符串都分别存储
            para.push_back(tmp);
        
        int pid = fork();
        if (pid < 0)
        {
            perror("Pid failed!");
            continue;
        }
        else if (pid == 0)
        {   
            if (para.size() < 3)
            {
                Command(Input);
            }
            else if (para[1][1] == 'x')
            { // proc/pid/maps
                cout << para[2] << "        ./a" << endl;
                string path = "/proc/" + para[2] + "/smaps";
                cout << path << endl; 
                
                int fd = open(path.c_str(), O_RDONLY);
                if (fd == -1)
                {
                    perror("Maps failure open!");
                    exit(EXIT_FAILURE);
                }
                
                int cnt = read(fd, buffer, sizeof(buffer));
                if (cnt < 0)
                {
                    perror("read");
                    exit(EXIT_FAILURE);
                }
                close(fd);
                
                cout << "Address\t\tKbytes\tRss\tDirty\tMode\tMapping\n";
                string opt(buffer);
                stringstream ss(opt);
                string tmpe;
                vector<string> PARA;
                while (getline(ss, tmpe, '\n')) // 将命令的每个字符串都分别存储
                    PARA.push_back(tmpe);
                
                vector<vector<string>> lines(PARA.size());
                for (int i = 0; i < PARA.size(); i++)
                {
                    stringstream sput(PARA[i]);
                    string tmptt;
                    while (getline(sput, tmptt, ' '))
                    {
                        if (!tmptt.empty())
                            lines[i].push_back(tmptt);
                    }
                }
                
                int sumkb = 0;
                for (int k = 0; k < PARA.size(); k = k + 2)
                {
                    for (int i = 0; i < lines[k].size(); i++)
                    {
                        if (i == 0)
                        {
                            stringstream sput1(lines[k][i]);
                            string tmptt1;
                            vector<string> tim;
                            while (getline(sput1, tmptt1, '-'))
                                tim.push_back(tmptt1);
                            
                            cout << tim[0] << "\t";
                        }
                        else if (i == 1)
                        {
                            cout << lines[k + 1][1] << "\t";
                            sumkb += atoi(lines[k + 1][1].c_str());
                        }
                        else if (i == 5)
                        {
                            cout << lines[k][i] << "\t";
                        }
                        else
                        {
                            cout << "---" << "\t";
                        }
                    }
                    cout << endl;
                }
                cout << "total kbytes:\t" << sumkb << "kb\t0\t0" << endl;
            }
            else if (para[1][1] == 'X')
            { // proc/pid/smaps
                cout << para[2] << "     ./a" << endl;
                string path = "/proc/" + para[2] + "/smaps";
                int fd = open(path.c_str(), O_RDONLY);
                if (fd == -1)
                {
                    perror("Smaps failure open!");
                    exit(EXIT_FAILURE);
                }
                
                int cnt = read(fd, buffer, sizeof(buffer));
                if (cnt < 0)
                {
                    perror("read");
                    exit(EXIT_FAILURE);
                }
                close(fd);
                
                cout << "Address\t\tPerm\tOffset\t\tDevice\tInode\tPss\tMapping\n";
                string opt(buffer);
                stringstream ss(opt);
                string tmpe;
                vector<string> PARA;
                while (getline(ss, tmpe, '\n')) // 将命令的每个字符串都分别存储
                    PARA.push_back(tmpe);
                
                vector<vector<string>> lines(PARA.size());
                for (int i = 0; i < PARA.size(); i++)
                {
                    stringstream sput(PARA[i]);
                    string tmptt;
                    while (getline(sput, tmptt, ' '))
                    {
                        if (!tmptt.empty())
                            lines[i].push_back(tmptt);
                    }
                }
                
                int sumkb = 0;
                for (int k = 0; k < PARA.size(); k = k + 2)
                {
                    for (int i = 0; i < lines[k].size(); i++)
                    {
                        if (i == 0)
                        {
                            stringstream sput1(lines[k][i]);
                            string tmptt1;
                            vector<string> tim;
                            while (getline(sput1, tmptt1, '-'))
                                tim.push_back(tmptt1);
                            
                            cout << tim[0] << "\t";
                        }
                        else if (i == 5)
                        {
                            cout << lines[k + 1][1] << "\t";
                            sumkb += atoi(lines[k + 1][1].c_str());
                            cout << lines[k][i] << "\t";
                        }
                        else
                        {
                            cout << lines[k][i] << "\t";
                        }
                    }
                    cout << endl;
                }
                cout << "                                                        " << sumkb << "Kb" << endl;
            }
        }
        wait(NULL);
    }
}

int main()
{
    read_history("command_history.txt");
    process();
}

//把输入通过空格分割成n块
//我们检查第二块的内容，判断是-x还是-X
//如果是-x，我们通过输入的pid读取/proc/pid/maps的内容
//如果是-X，我么通过输入的pid读取/proc/pid/smaps的内容
//pid就是分割后第三块的内容
//然后输出的内容先存到缓冲区，并不是所有内容都要输出，我们只输出对应的部分
//注意排版