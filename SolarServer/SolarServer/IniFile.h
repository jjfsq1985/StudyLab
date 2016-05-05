#ifndef _INIFILE_H
#define _INIFILE_H

#include <list>
#include <vector>
#include <string>

using namespace std;

namespace inifile
{
    const int RET_OK = 0;
    const int RET_ERR = -1;
    const string delim = "\n";
    struct IniItem
    {
        string key;
        string value;
        string comment;
    };
    struct IniSection {
        typedef vector<IniItem>::iterator iterator;
        iterator begin() {
            return items.begin();
        }
        iterator end() {
            return items.end();
        }

        string name;
        string comment;
        vector<IniItem> items;
    };

    struct IniGroup {
        typedef list<IniSection*>::iterator iterator;
        iterator begin() {
            return sectionsItems.begin();
        }
        iterator end() {
            return sectionsItems.end();
        }
        string group;
        string groupcomment;
        list<IniSection*> sectionsItems;//makepair<sectionname, IniSection>
    };

    enum IniToken
    {
        Default = 0,
        InGroup, 
        InSection,
        GroupSkip,
        SectionSkip
    };


    class IniFile
    {
    public:
        IniFile();
        ~IniFile()
        {
            release();
        }

    public:
        typedef list<IniGroup *>::iterator iterator;

        iterator begin() {
            return groups_.begin();
        }
        iterator end() {
            return groups_.end();
        }
    public:
        /* �򿪲�����һ����Ϊfname��INI�ļ� */
        int load(const string &fname);
        /*�����ݱ��浽��ǰ�ļ�*/
        int save();
        /*��������浽һ����Ϊfname���ļ�*/
        int saveas(const string &fname);

        /*��ȡsection�ε�һ����Ϊkey��ֵ,��������string�͵�ֵ*/
        string getStringValue(const string &group, const string &section, const string &key, int &ret);
        /*��ȡsection�ε�һ����Ϊkey��ֵ,��������int�͵�ֵ*/
        int getIntValue(const string &group, const string &section, const string &key, int &ret);
        /*��ȡsection�ε�һ����Ϊkey��ֵ,��������double�͵�ֵ*/
        double getDoubleValue(const string &group, const string &section, const string &key, int &ret);

        /*��ȡsection�ε�һ����Ϊkey��ֵ,����ֵ����value��*/
        int getValue(const string &group, const string &section, const string &key, string &value);
        /*��ȡsection�ε�һ����Ϊkey��ֵ,����ֵ����value��,��ע�͸���comment��*/
        int getValue(const string &group, const string &section, const string &key, string &value, string &comment);

        /*��ȡsection�����м�Ϊkey��ֵ,����ֵ����values��vector��*/
        int getValues(const string &group, const string &section, const string &key, vector<string> &values);
        /*��ȡsection�����м�Ϊkey��ֵ,����ֵ����values��vector��,,��ע�͸���comments��vector��*/
        int getValues(const string &group, const string &section, const string &key, vector<string> &value, vector<string> &comments);

        bool hasSection(const string &group,const string &section);
        bool hasKey(const string &group, const string &section, const string &key);

        /* ��ȡgroup�ε�ע�� */
        int getGroupComment(const string &group, string &comment);
        /* ����group�ε�ע�� */
        int setGroupComment(const string &group, const string &comment);

        /* ��ȡsection�ε�ע�� */
        int getSectionComment(const string &group, const string &section, string &comment);
        /* ����section�ε�ע�� */
        int setSectionComment(const string &group, const string &section, const string &comment);
        /*��ȡע�ͱ�Ƿ��б�*/
        void getCommentFlags(vector<string> &flags);
        /*����ע�ͱ�Ƿ��б�*/
        void setCommentFlags(const vector<string> &flags);

        /*ͬʱ����ֵ��ע��*/
        int setValue(const string &group, const string &section, const string &key, const string &value, const string &comment = "");
        /*ɾ����*/
        void deleteGroup(const string &group);
        /*ɾ����*/
        void deleteSection(const string &group, const string &section);
        /*ɾ���ض��ε��ض�����*/
        void deleteKey(const string &group, const string &section, const string &key);
    public:
        /*ȥ��strǰ���c�ַ�*/
        static void trimleft(string &str, char c = ' ');
        /*ȥ��str�����c�ַ�*/
        static void trimright(string &str, char c = ' ');
        /*ȥ��strǰ��ͺ���Ŀո��,Tab���ȿհ׷�*/
        static void trim(string &str);
        /*���ַ���str���ָ��delim�ָ�ɶ���Ӵ�*/
    private:
        int ParseTokenName(const string& sName, const string& sGroup, IniToken& current_,string& comment);
        void ResetParse(const string& line, int nConstBrace, IniToken ConstToken, int& nBrace, IniToken& current_);
        IniSection *getSection(const string &group, const string &section);
        IniGroup * getGroup(const string &group);
        void release();
        int getline(string &str, FILE *fp);
        bool isComment(const string &str);
        bool suffixComment(const string &str);
        bool parse(const string &content, string &key, string &value, char c = '=');
      
        //for debug
    private:
        void print();

    private:
        IniToken current_;
        list<IniGroup *> groups_;
        string fname_;  //�ļ�����
        vector<string> flags_;//ע�ͱ�־
    };
}

#endif
