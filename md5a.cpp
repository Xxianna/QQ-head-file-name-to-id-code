#include <iostream>
#include <string>
#include "md5.h"
#include <omp.h>
#include <atomic>
 
using std::cout; using std::endl;
using namespace std;

#define uint64_t unsigned long long

void cout_help(char *argv[])
{
    system("chcp 936");
    cout << "Usage: " << endl;
    cout << "根据头像文件名获取qq号：" << argv[0] << " [文件名] [最大猜测QQ号] [最小猜测QQ号]" << endl;
    cout << "根据qq号获取文件名：" << argv[0] << " [qq号]" << endl;
}

string MD5(const string& text)
{
    string result = md5(text);
    //转换为大写
    for (int i = 0; i < result.size(); i++)
    {
        if (result[i] >= 'a' && result[i] <= 'z')
        {
            result[i] = result[i] - 32;
        }
    }
    return result;
}

string get_head_name_from_qqcode(const string& qqcode)
{
    return MD5(MD5(MD5(qqcode) + qqcode) + qqcode);
}

//暴力破解QQ号
string get_qqcode_from_head_name(const string& head_name, uint64_t max = 9999999999, uint64_t min = 0)
{
    for (uint64_t i = max; i > min; i--)
    {
        if (head_name == get_head_name_from_qqcode(to_string(i)))
        {
            return to_string(i);
        }
    }
    return "Not Found!";
}

//多线程暴力破解QQ号
string get_qqcode_from_head_name_omp(const string& head_name, uint64_t max = 9999999999, uint64_t min = 0)
{
    std::atomic<bool> found(false);
    std::string result = "Not Found!";
    
    #pragma omp parallel for num_threads(20) shared(found, result)
    for (uint64_t i = max; i > min; i--)
    {
        if (!found.load())
        {
            if (head_name == get_head_name_from_qqcode(to_string(i)))
            {
                if (!found.exchange(true))
                {
                    result = to_string(i);
                }
            }
        }
    }
    return result;
}
 
int main(int argc, char *argv[])
{
    //如果参数是QQ号，则输出头像文件名，否则输出QQ号
    if (argc >= 2)
    {
        string arg = argv[1];
        if (arg.size() >= 32)
        {
            //是个md5值。取其前32位
            arg = arg.substr(0, 32);
            //如果有第三位则作为max，有第四位则作为min
            uint64_t max = 9999999999;
            uint64_t min = 0;
            if (argc >= 3)
            {
                max = atoll(argv[2]);
                if (argc >= 4)
                {
                    min = atoll(argv[3]);
                }
            }
            // cout << "qqcode of head filename: " << get_qqcode_from_head_name_omp(arg, max, min) << endl;
            cout << get_qqcode_from_head_name_omp(arg, max, min) << endl;
        }
        else
        {
            //如果是个纯数字，则输出头像文件名
            bool is_number = true;
            for (int i = 0; i < arg.size(); i++)
            {
                if (arg[i] < '0' || arg[i] > '9')
                {
                    is_number = false;
                    break;
                }
            }
            if (is_number)
            {
                // cout << "head filename of qqcode: " << get_head_name_from_qqcode(arg) << ".jpg_" << endl;
                cout << get_head_name_from_qqcode(arg) << ".jpg_" << endl;
            }
            //否则输出帮助信息
            else
            {
                cout_help(argv);
            }
        }
        return 0;
    }else{
        cout_help(argv);
    }

    return 0;
}
