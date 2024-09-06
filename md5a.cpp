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
    cout << "����ͷ���ļ�����ȡqq�ţ�" << argv[0] << " [�ļ���] [���²�QQ��] [��С�²�QQ��]" << endl;
    cout << "����qq�Ż�ȡ�ļ�����" << argv[0] << " [qq��]" << endl;
}

string MD5(const string& text)
{
    string result = md5(text);
    //ת��Ϊ��д
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

//�����ƽ�QQ��
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

//���̱߳����ƽ�QQ��
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
    //���������QQ�ţ������ͷ���ļ������������QQ��
    if (argc >= 2)
    {
        string arg = argv[1];
        if (arg.size() >= 32)
        {
            //�Ǹ�md5ֵ��ȡ��ǰ32λ
            arg = arg.substr(0, 32);
            //����е���λ����Ϊmax���е���λ����Ϊmin
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
            //����Ǹ������֣������ͷ���ļ���
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
            //�������������Ϣ
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
