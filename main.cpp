#include <iostream>
#include <fstream>
#include <utility>
#include<stack>
#include<queue>
#include <unordered_map>
#include <string>
using namespace std;

int GetIndexFromT(int token);
int GetIndexFromN(char N);
int EnCodeLeftOfP(char leftOfP);
void DisplayTokenStream(queue<int> tokenStream);
void DisplaySymbolStateStack(stack<int> symbolStateStack);
pair<char, int> SRTable[41][18];
int gotoTable[41][6];
stack<int> symbolStateStack;       //符号状态栈
queue<int> tokenStream;             //记号流
//词法分析变量
#pragma region 
unordered_map<string, int> Hash;
void LexicalAnalysis();
int Kind(int finishState, string str);
int Move(int curState, char ch);
bool isFinishState(int curState);
char ch;
int curState = 0, kind, lastKind, lastState = 0;
string str; 	//暂存读入的字符
fstream infile("sourceCode.txt", ios::in);
#pragma endregion
int main()
{
    /*
    =================LR================
    1.#和状态0压入符号状态栈
    2.(栈顶状态，当前输入)查找移进规约表
        si：将当前输入压栈，将状态i压栈
        ri：
        计算第i个产生式右部的长度length；
        弹出lenght*2个符号和状态；
        记录栈顶元素状态state；
        压入产生式左部；
        （state，产生式左部）查找goto表，将状态压栈；
        acc：
    */
//词法分析，生成记号流，记号流存入tokenStream队列
#pragma region 
//记号表
Hash.insert({
		{"sin", 1},{"cos", 2},{"tg", 3},{"ctg", 4},{"lg", 5},
		{"ln", 6},{"log", 7},{"+", 8},{"-", 9},{"*", 10},
		{"/", 11},{"=", 12},{"^", 13},{"(", 14},{")", 15},
		{";", 16},{",", 17},{"?", 18},{"PI", 19},{"E", 20},
		{"num", 21},{"id", 22},{"#", 23},{"非法字符", 24},
	});
//读取文件
	if (!infile)
	{
		perror("文件打开失败！");
		exit(0);
	}
LexicalAnalysis();
tokenStream.push(23);     //#
#pragma endregion
//===================构造移进规约分析表================
#pragma region 
//0
    SRTable[0][0] = make_pair('s', 4);
    SRTable[0][2] = make_pair('s', 3);
    SRTable[0][17] = make_pair('r', 2);
//1
    SRTable[1][17] = make_pair('a', 0);
//2
    SRTable[2][1] = make_pair('s', 5);
//3
    SRTable[3][10] = make_pair('s', 6);
//4,6
    for (int i = 2; i < 6; i++)
    {
        SRTable[4][i] = make_pair('s', 12 + i);
        SRTable[6][i] = make_pair('s', 12 + i);
    }
    for (int i = 12; i < 15; i++)
    {
        SRTable[4][i] = make_pair('s', i - 1);
        SRTable[6][i] = make_pair('s', i - 1);
    }
//5
    SRTable[5][0] = make_pair('s', 4);
    SRTable[5][2] = make_pair('s', 3);
    SRTable[5][17] = make_pair('r', 2);
//7
    SRTable[7][1] = make_pair('r', 4);
    SRTable[7][6] = make_pair('s', 20);
    SRTable[7][7] = make_pair('s', 21);
//8
    SRTable[8][1] = make_pair('r', 7);
    SRTable[8][6] = make_pair('r', 7);
    SRTable[8][7] = make_pair('r', 7);
    SRTable[8][8] = make_pair('s', 22);
    SRTable[8][9] = make_pair('s', 23);
    SRTable[8][15] = make_pair('r', 7);
//9
    SRTable[9][1] = make_pair('r', 10);
    for (int i = 6; i < 10; i++)
    {
        SRTable[9][i] = make_pair('r', 10);
    }
    SRTable[9][11] = make_pair('s', 24);
    SRTable[9][15] = make_pair('r', 10);
//10,32,40
    SRTable[10][1] = make_pair('r', 11);
    SRTable[32][1] = make_pair('r', 12);
    SRTable[40][1] = make_pair('r', 14);
    for (int i = 6; i < 10; i++)
    {
        SRTable[10][i] = make_pair('r', 11);
        SRTable[32][i] = make_pair('r', 12);
        SRTable[40][i] = make_pair('r', 14);
    }
    SRTable[10][11] = make_pair('r', 11);
    SRTable[10][15] = make_pair('r', 11);
    SRTable[32][11] = make_pair('r', 12);
    SRTable[32][15] = make_pair('r', 12);
    SRTable[40][11] = make_pair('r', 14);
    SRTable[40][15] = make_pair('r', 14);
//11
    SRTable[11][14] = make_pair('s', 26);
//12
    SRTable[12][14] = make_pair('s', 26);
//13
    for (int i = 2; i < 6; i++)
    {
        SRTable[13][i] = make_pair('s', 12 + i);
    }
    for (int i = 12; i < 15; i++)
    {
        SRTable[13][i] = make_pair('s', i - 1);
    }

//14,15,16,17
    for (int i = 6; i < 10; i++)
    {
        SRTable[14][i] = make_pair('r', 17);
        SRTable[15][i] = make_pair('r', 18);
        SRTable[16][i] = make_pair('r', 19);
        SRTable[17][i] = make_pair('r', 20);
    }
    for (int i = 14; i < 18; i++)
    {
        SRTable[i][1] = make_pair('r', i+3);
        SRTable[i][11] = make_pair('r', i+3);
        SRTable[i][15] = make_pair('r', i+3);
        SRTable[i][16] = make_pair('r', i+3);
    }
//18
    SRTable[18][17] = make_pair('r', 1);
//19
    SRTable[19][1] = make_pair('r', 3);
    SRTable[19][6] = make_pair('s', 20);
    SRTable[19][7] = make_pair('s', 21);
    
//20,21,22,23   
    for (int i = 20; i < 24; i++)
    {
        SRTable[i][2] = make_pair('s', 14);
        SRTable[i][3] = make_pair('s', 15);
        SRTable[i][4] = make_pair('s', 16);
        SRTable[i][5] = make_pair('s', 17);
        SRTable[i][12] = make_pair('s', 11);
        SRTable[i][13] = make_pair('s', 12);
        SRTable[i][14] = make_pair('s', 13);
    }
 //24,25,26
    for (int i = 24; i < 27; i++)
    {
        SRTable[i][2] = make_pair('s', 14);
        SRTable[i][3] = make_pair('s', 15);
        SRTable[i][4] = make_pair('s', 16);
        SRTable[i][5] = make_pair('s', 17);
        SRTable[i][14] = make_pair('s', 13);
    }
//27
    SRTable[27][6] = make_pair('s', 20);
    SRTable[27][7] = make_pair('s', 21);
    SRTable[27][15] = make_pair('s', 35);
//28,29
    for (int i = 28; i < 30; i++)
    {
        SRTable[i][1] = make_pair('r', i-23);
        SRTable[i][6] = make_pair('r', i-23);
        SRTable[i][7] = make_pair('r', i-23);
        SRTable[i][8] = make_pair('s', 22);
        SRTable[i][9] = make_pair('s', 23);
        SRTable[i][15] = make_pair('r', i-23);
    }
//30,31
    for (int i = 30; i < 32; i++)
    {
        SRTable[i][1] = make_pair('r', i-22);
        SRTable[i][6] = make_pair('r', i-22);
        SRTable[i][7] = make_pair('r', i-22);
        SRTable[i][8] = make_pair('r', i-22);
        SRTable[i][9] = make_pair('r', i-22);
        SRTable[i][11] = make_pair('s', 24);
    }
    SRTable[30][15] = make_pair('r', 8);
    SRTable[31][15] = make_pair('r', 9);
//33
    SRTable[33][15] = make_pair('s', 36);
//34
    SRTable[34][15] = make_pair('s', 37);
    SRTable[34][16] = make_pair('s', 38);
//35,36,37
    SRTable[35][1] = make_pair('r', 16);
    SRTable[36][1] = make_pair('r', 13);
    SRTable[37][1] = make_pair('r', 15);
    for (int i = 6; i < 10; i++)
    {
        SRTable[35][i] = make_pair('r', 16);
        SRTable[36][i] = make_pair('r', 13);
        SRTable[37][i] = make_pair('r', 15);
    }
    SRTable[35][11] = make_pair('r', 16);
    SRTable[36][11] = make_pair('r', 13);
    SRTable[37][11] = make_pair('r', 15);
    SRTable[35][16] = make_pair('r', 16);
    SRTable[36][15] = make_pair('r', 13);
    SRTable[37][15] = make_pair('r', 15);
//38
    for (int i = 2; i < 6; i++)
    {
        SRTable[38][i] = make_pair('s', 12+i);
    }
    SRTable[38][14] = make_pair('s', 13);
//39
    SRTable[39][15] = make_pair('s', 40);
#pragma endregion
//===================goto表==================
#pragma region 
gotoTable[0][0]=1;
gotoTable[0][1]=2;

gotoTable[4][2]=7;
gotoTable[4][3]=8;
gotoTable[4][4]=9;
gotoTable[4][5]=10;

gotoTable[5][0]=18;
gotoTable[5][1]=2;

gotoTable[6][2]=19;
gotoTable[6][3]=8;
gotoTable[6][4]=9;
gotoTable[6][5]=10;

gotoTable[13][2]=27;
gotoTable[13][3]=8;
gotoTable[13][4]=9;
gotoTable[13][5]=10;

gotoTable[20][3]=28;
gotoTable[20][4]=9;
gotoTable[20][5]=10;

gotoTable[21][3]=29;
gotoTable[21][4]=9;
gotoTable[21][5]=10;

gotoTable[22][4]=30;
gotoTable[22][5]=10;

gotoTable[23][4]=31;
gotoTable[23][5]=10;

gotoTable[24][5]=32;

gotoTable[25][5]=33;

gotoTable[26][5]=34;

gotoTable[38][5]=39;
#pragma endregion
//================产生式====================
//产生式左部
char leftOfP[]={
    'S','S',
    'A','A',
    'B','B','B',
    'C','C','C',
    'D','D','D','D','D',
    'E','E','E','E','E'};
//产生式右部
//id==>a    fun==>f     log==>g     PI==>P      E==>e       num==>n
string rightOfP[]={
    "A;S","",
    "a=B","?B",
    "B+C","B-C","C",
    "C*D","C/D","D",
    "E","D^E","f(E)","g(E,E)","g(E)",
    "(B)","a","P","e","n"};

/*int tokensArr[] = {
		22,12,21,10,19,16,
        22,12,20,16,
		22,12,21,16,
        18,21,10,14,6,14,22,15,
        8,21,10,1,14,22,15,15,
        8,14,21,8,22,15,13,21,16
	};
//将记号压入队列
for (int i = 0; i < sizeof(tokensArr)/4; i++)
{
    tokenStream.push(tokensArr[i]);
}*/

//初始化
#pragma region 
symbolStateStack.push(23);//'#'
symbolStateStack.push(0);//状态0
int curToken;
int topState;
int TIndex,NIndex;
char sORr;
int rightOfP_SymbolNum,leftOfPCode; //产生式右部字符个数；产生式左部的字符的编码
int indexOfP;       //产生式序号
int step=0;       //记录步骤
#pragma endregion
while (!tokenStream.empty())//记号流非空
{
    //打印步骤，栈内容，当前输入
    cout<<++step<<'\t';
    //DisplaySymbolStateStack(symbolStateStack);
    //cout<<'\t';
    //DisplayTokenStream(tokenStream);
    //cout<<'\t';

    curToken=tokenStream.front();
    topState=symbolStateStack.top(); //栈顶状态
    //查表
    TIndex = GetIndexFromT(curToken);
    if (TIndex!=-1)//该终结符存在
    {
        sORr = SRTable[topState][TIndex].first;
        //移进
        if (sORr=='s')
        {
            //压入当前记号，及s后面的状态
            symbolStateStack.push(curToken);
            //打印动作
            cout<<'s'<<SRTable[topState][TIndex].second<<endl;
            //cout <<"吃掉一个记号"<<tokenStream.front()<<' ';
            tokenStream.pop();  //吃掉一个记号
            symbolStateStack.push(SRTable[topState][TIndex].second);
            //打印当前栈内容
            //DisplaySymbolStateStack(symbolStateStack);
        }
        //规约
        else if(sORr=='r')
        {
            //查找第k个产生式右部的字符个数
            indexOfP=SRTable[topState][TIndex].second-1;  //产生式序号
            cout<<'r'<<SRTable[topState][TIndex].second<<endl;
            rightOfP_SymbolNum=rightOfP[indexOfP].length(); //产生式右部字符个数
            //弹出rightOfP_SymbolNum*2个字符和状态
            for (int i = 0; i < rightOfP_SymbolNum*2; i++)
            {    
                symbolStateStack.pop();
            }
            //暂存栈顶状态，产生式左部压栈
            topState=symbolStateStack.top();
            leftOfPCode=EnCodeLeftOfP(leftOfP[indexOfP]);//获取产生式左部的编码，因为产生式左部为字符，为了方便起见，符号状态栈存的都是整数
            symbolStateStack.push(leftOfPCode);
            //查找goto表，将状态压栈
            NIndex=GetIndexFromN(leftOfP[indexOfP]);
            symbolStateStack.push(gotoTable[topState][NIndex]);
            //打印当前栈内容
            //DisplaySymbolStateStack(symbolStateStack);
        }
        else if (sORr=='a')//接受状态
        {
            cout<<"accept!"<<endl;
            break;
			//return 0;
        }
        else//查表，内容为空
        {
            perror("Lookup table, the content is empty！");
			break;
            //return -1;
        }
    }
    else
    {
        perror("Don't have this Terminal Symbol!");
        break;
		//return -1;
    }
}
    system("pause");
    return 0;
}
int GetIndexFromT(int token)
{
    switch (token)
    {
        case 18:return 0;//?
        case 16:return 1;//;
        case 22:return 2;//id==>a
        case 19:return 3;//PI==>P
        case 20:return 4;//E==>e
        case 21:return 5;//num==>n
        case 8:return 6;//+
        case 9:return 7;//-
        case 10:return 8;//*
        case 11:return 9;///
        case 12:return 10;//=
        case 13:return 11;//^
        case 1:case 2:case 3:case 4:case 5:case 6:return 12;//fun==>f sin,cos,cg,ctg,lg,ln
        case 7:return 13;//log==>g
        case 14:return 14;//(
        case 15:return 15;//)
        case 17:return 16;//,
        case 23:return 17;//#
        default: return -1;
	}
}

int GetIndexFromN(char N)
{
    switch (N)
    {
    case 'S':return 0;
    case 'A':return 1;
    case 'B':return 2;
    case 'C':return 3;
    case 'D':return 4;
    case 'E':return 5;
    default:return -1;
    }
}
int EnCodeLeftOfP(char leftOfP)
{
    switch (leftOfP)
    {
    case 'S':return 1000;
    case 'A':return 1001;
    case 'B':return 1010;
    case 'C':return 1011;
    case 'D':return 1100;
    case 'E':return 1101;
    default:return -1;
    }
}

void DisplayTokenStream(queue<int> tokenStream)
{
    //当前输入（记号流）
    while (!tokenStream.empty())
    {
        cout<<tokenStream.front()<<' ';
        tokenStream.pop();
    }
    cout <<endl;
}
void DisplaySymbolStateStack(stack<int> symbolStateStack)
{
    //cout<<"栈内容";
    while (!symbolStateStack.empty())
    {
        cout<<symbolStateStack.top()<<' ';
        symbolStateStack.pop();
    }
    cout<<endl;
}

//词法分析所需函数
#pragma region 
void LexicalAnalysis()
{
	//读取字符流至文件末尾
	while ((ch = infile.get()) != EOF)
	{
		switch (ch)
		{
		//字符为\n或\t，清空队列，状态置为S0
		case '\n':
		case '\t':
		case ' ':
		{
			continue;
		}
		default:
		{
			//如果上一个种别码是num、id、PI、E则该符号是减号
			if (ch == '-')
			{ //减号
				if (lastKind == 22 || lastKind == 21 || lastKind == 19 || lastKind == 20)
				{
					str = str + ch;
					kind = Hash[str];
					if (kind) //在记号表中找到种别码
					{
						tokenStream.push(kind); //存入缓冲队列
						lastKind = kind;
						//cout << "<" << str << "," << kind << ">" << endl;
						curState = 0;
						lastState = 0;
						str = "";
						continue;
					}
					else
					{
						perror("非法字符!");
						//return 0;
					}
				}
			}
			//负号、非运算符、非分隔符
			curState = Move(curState, ch);
			if (curState != -1)
			{
				lastState = curState;
				str = str + ch;
				continue;
			}
			else
			{
				//判断上一个状态是否是接受状态
				if (isFinishState(lastState)) //是接受状态
				{
					kind = Kind(lastState, str);
					if (kind != -1)
					{
						tokenStream.push(kind); //存入缓冲队列
						lastKind = kind;
						//cout << "<" << str << "," << kind << ">" << endl;
						str = ch;
						curState = 0;
						curState = Move(curState, ch);
						lastState = curState;
						if (curState == 7)
						{
							kind = Kind(curState, str);
							tokenStream.push(kind); //存入缓冲队列
							//cout << "<" << str << "," << kind << ">" << endl;
							str = "";
							curState = 0;
							lastState = 0;
						}
					}
					else
					{
						perror("非法字符！");
					}
				}
				else //不是接受状态
				{
					perror("非接受状态！");
					//return 0;
				}
			}
		}
		}
	}
}
/*
状态转移函数
*/
int Move(int curState, char ch)
{
	switch (curState)
	{
	case 0:
		switch (ch)
		{
		//字母
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
			return 1;
		//数字0
		case '0':
			return 4;
		//数字1~9
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return 3;
		//下划线
		case '_':
			return 1;
		//负号
		case '-':
			return 2;
		//运算符、分隔符
		case '+':
		case '*':
		case '/':
		case '^':
		case '?':
		case ';':
		case ',':
		case '(':
		case ')':
		case '=':
		case '#':
			return 7;
		//非法字符
		default:
			return -1;
		}
	case 1:
		switch (ch)
		{
		//字母，数字0~9，下划线
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '_':
			return 1;
		//非法字符
		default:
			return -1;
		}
	case 2:
		switch (ch)
		{
		//数字0
		case 0:
			return 4;
		//数字1~9
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return 3;
		//非法字符
		default:
			return -1;
		}
	case 3:
		switch (ch)
		{
		//数字0~9
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return 3;
		//小数点
		case '.':
			return 5;
		//非法字符
		default:
			return -1;
		}
	case 4:
		switch (ch)
		{
		//小数点
		case '.':
			return 5;
		//非法字符
		default:
			return -1;
		}
	case 5:
	case 6:
		switch (ch)
		{
		//数字0
		case 0:
			return 5;
		//数字1~9
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return 6;
		//非法字符
		default:
			return -1;
		}
	default:
		return -1;
	}
}

bool isFinishState(int curState)
{
	if (curState == 1 || curState == 3 || curState == 4 || curState == 6 || curState == 7)
		return true;
	else
		return false;
}

int Kind(int finishState, string str)
{
	int kind;
	switch (finishState)
	{
	//接受状态为1：标识符（含有关键字）
	case 1:
	{
		kind = Hash[str];
		if (kind) //是关键字
		{
			return kind;
		}
		else //非关键字
		{
			kind = 22; //id种别码：22
			return kind;
		}
	}
	//接受状态为3，4，6：常量
	case 3:
	case 4:
	case 6:
		kind = 21; //num种别码：21
		return kind;
	//接受状态为7：分隔符、运算符等
	case 7:
		kind = Hash[str];
		return kind;
	default:
		return -1;
	}
}
#pragma endregion