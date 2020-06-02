#include <iostream>
#include <fstream>
#include <utility>
#include <stack>
#include <queue>
#include <unordered_map>
#include <string>
#include<cmath>
#define NUM_E 2.7
#define NUM_PI 3.14
using namespace std;
queue<Token> tokenStream;	 //�Ǻ���
//�﷨����

class Token
{
public:
	int kind;
	//double numValue;
	string attr;		//���ԡ�����-->string������������-->double��ֵ
	Token(/* args */);
	~Token();
};

//�������

class Node
{
public:
	int kind;				//�ڵ�����
	string name;			//��������
	float value;			//ֵ
	Node *leftChild;
	Node *rightChild;
	bool flag; //�Ƿ�ֵ	//����������
	Node();
	~Node();
};

Node::Node()
{
	leftChild = NULL;
	rightChild = NULL;
	flag = false;
}

Node::~Node()
{
}

int main()
{
	/*
    =================LR================
    1.#��״̬0ѹ�����״̬ջ
    2.(ջ��״̬����ǰ����)�����ƽ���Լ��
        si������ǰ����ѹջ����״̬iѹջ
        ri��
        �����i������ʽ�Ҳ��ĳ���length��
        ����lenght*2�����ź�״̬��
        ��¼ջ��Ԫ��״̬state��
        ѹ�����ʽ�󲿣�
        ��state������ʽ�󲿣�����goto����״̬ѹջ��
        acc��
    */
	//�ʷ����������ɼǺ������Ǻ�������tokenStream����

	GrammerAnalysis();
	system("pause");
	return 0;
}
void GrammerAnalysis()
{
//�ʷ�����
	void LexicalAnalysis();
	bool IsIdentifier(int kind);
//�﷨��������
#pragma region
	int GetIndexFromT(int token);
	int GetIndexFromN(char N);
	int EnCodeLeftOfP(char leftOfP);
	void DisplayTokenStream(queue<int> tokenStream);
	void DisplaySymbolStateStack(stack<int> symbolStateStack);
	void GrammerAnalysis();
	pair<char, int> SRTable[41][18];
	int gotoTable[41][6];
	stack<int> symbolStateStack; //����״̬ջ

	int topState;
	int TIndex, NIndex;
	char sORr;
	int rightOfP_SymbolNum, leftOfPCode; //����ʽ�Ҳ��ַ�����������ʽ�󲿵��ַ��ı���
	int indexOfP;						 //����ʽ���
	int step = 0;						 //��¼����
#pragma endregion

#pragma region

	LexicalAnalysis(); //�ʷ�����
	Token *token = new Token();
	token->kind = 23;
	token->attr = "#";
	tokenStream.push(*token);
	//tokenStream.push(23); //#
#pragma endregion
//===================�����ƽ���Լ������================
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
		SRTable[i][1] = make_pair('r', i + 3);
		SRTable[i][11] = make_pair('r', i + 3);
		SRTable[i][15] = make_pair('r', i + 3);
		SRTable[i][16] = make_pair('r', i + 3);
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
		SRTable[i][1] = make_pair('r', i - 23);
		SRTable[i][6] = make_pair('r', i - 23);
		SRTable[i][7] = make_pair('r', i - 23);
		SRTable[i][8] = make_pair('s', 22);
		SRTable[i][9] = make_pair('s', 23);
		SRTable[i][15] = make_pair('r', i - 23);
	}
	//30,31
	for (int i = 30; i < 32; i++)
	{
		SRTable[i][1] = make_pair('r', i - 22);
		SRTable[i][6] = make_pair('r', i - 22);
		SRTable[i][7] = make_pair('r', i - 22);
		SRTable[i][8] = make_pair('r', i - 22);
		SRTable[i][9] = make_pair('r', i - 22);
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
		SRTable[38][i] = make_pair('s', 12 + i);
	}
	SRTable[38][14] = make_pair('s', 13);
	//39
	SRTable[39][15] = make_pair('s', 40);
#pragma endregion
//===================goto��==================
#pragma region
	gotoTable[0][0] = 1;
	gotoTable[0][1] = 2;

	gotoTable[4][2] = 7;
	gotoTable[4][3] = 8;
	gotoTable[4][4] = 9;
	gotoTable[4][5] = 10;

	gotoTable[5][0] = 18;
	gotoTable[5][1] = 2;

	gotoTable[6][2] = 19;
	gotoTable[6][3] = 8;
	gotoTable[6][4] = 9;
	gotoTable[6][5] = 10;

	gotoTable[13][2] = 27;
	gotoTable[13][3] = 8;
	gotoTable[13][4] = 9;
	gotoTable[13][5] = 10;

	gotoTable[20][3] = 28;
	gotoTable[20][4] = 9;
	gotoTable[20][5] = 10;

	gotoTable[21][3] = 29;
	gotoTable[21][4] = 9;
	gotoTable[21][5] = 10;

	gotoTable[22][4] = 30;
	gotoTable[22][5] = 10;

	gotoTable[23][4] = 31;
	gotoTable[23][5] = 10;

	gotoTable[24][5] = 32;

	gotoTable[25][5] = 33;

	gotoTable[26][5] = 34;

	gotoTable[38][5] = 39;
	//================����ʽ====================
	//����ʽ��
	char leftOfP[] =  {
		'S', 'S',
		'A', 'A',
		'B', 'B', 'B',
		'C', 'C', 'C',
		'D', 'D', 'D', 'D', 'D',
		'E', 'E', 'E', 'E', 'E'};
	//����ʽ�Ҳ�
	//id==>a    fun==>f     log==>g     PI==>P      E==>e       num==>n
	string rightOfP[] = {
		"A;S", "",
		"a=B", "?B",
		"B+C", "B-C", "C",
		"C*D", "C/D", "D",
		"E", "D^E", "f(E)", "g(E,E)", "g(E)",
		"(B)", "a", "P", "e", "n"};

	//��ʼ��
	symbolStateStack.push(23); //'#'
	symbolStateStack.push(0);  //״̬0
#pragma endregion
	Token curToken;
	int curTokenKind;
	stack<Node>NodeStack;
	stack<double>numStack;
	stack<Token>SynTokenStack;
	unordered_map<string, int> SymbolTable;	//���ű�
Node *node;
bool idFlag;//���Һ����Ƿ���ڱ�ʶ��
Token tempToken;	//�ݴ��ͬ���Ǻ�ջ�����ļǺţ����Խ�ֵ��ֵ�����
	while (!tokenStream.empty()) //�Ǻ����ǿ�
	{
		//��ӡ���裬ջ���ݣ���ǰ����
		cout << ++step << '\t';
		//DisplaySymbolStateStack(symbolStateStack);
		//cout<<'\t';
		//DisplayTokenStream(tokenStream);
		//cout<<'\t';
		idFlag=0;
		curToken = tokenStream.front();
		curTokenKind = curToken.kind;
		topState = symbolStateStack.top(); //ջ��״̬
		//���
		TIndex = GetIndexFromT(curTokenKind);
		if (TIndex != -1) //���ս������
		{
			sORr = SRTable[topState][TIndex].first;
			//�ƽ�
			if (sORr == 's')
			{
				//ѹ�뵱ǰ�Ǻţ���s�����״̬
				symbolStateStack.push(curTokenKind);

				//�ָ���;,(,),,��ѹջ;+��-��*��/��^��=��log��ѹջ
				//curTokenKind==16||curTokenKind==14||curTokenKind==15||curTokenKind==17||curTokenKind==8||curTokenKind==9||curTokenKind==10||curTokenKind==11||curTokenKind==12||curTokenKind==13
				//PI��E��num��idѹջ
				if (curTokenKind==19||curTokenKind==20||curTokenKind==21||curTokenKind==22)
				{
					SynTokenStack.push(curToken);			//�Ǻ�ѹջ
				}

				//��ӡ����
				cout << 's' << SRTable[topState][TIndex].second << endl;
				//cout <<"�Ե�һ���Ǻ�"<<tokenStream.front()<<' ';
				tokenStream.pop(); //�Ե�һ���Ǻ�
				symbolStateStack.push(SRTable[topState][TIndex].second);
				//��ӡ��ǰջ����
				//DisplaySymbolStateStack(symbolStateStack);
			}
			//��Լ
			else if (sORr == 'r')
			{
				//���ҵ�k������ʽ�Ҳ����ַ�����
				indexOfP = SRTable[topState][TIndex].second - 1; //����ʽ���
				cout << 'r' << SRTable[topState][TIndex].second << endl;
				rightOfP_SymbolNum = rightOfP[indexOfP].length(); //����ʽ�Ҳ��ַ�����
				//����rightOfP_SymbolNum*2���ַ���״̬
				for (int i = 0; i < rightOfP_SymbolNum * 2; i++)
				{
					symbolStateStack.pop();
				}
				//�ݴ�ջ��״̬������ʽ��ѹջ
				topState = symbolStateStack.top();
				leftOfPCode = EnCodeLeftOfP(leftOfP[indexOfP]); //��ȡ����ʽ�󲿵ı��룬��Ϊ����ʽ��Ϊ�ַ���Ϊ�˷������������״̬ջ��Ķ�������
				symbolStateStack.push(leftOfPCode);
				//����goto����״̬ѹջ
				NIndex = GetIndexFromN(leftOfP[indexOfP]);
				symbolStateStack.push(gotoTable[topState][NIndex]);

				//�����﷨����ע���﷨����
				//PI��E��num
				if (indexOfP==17||indexOfP==18||indexOfP==19)//����������num,����Ҫ���ǺŴӵ���SynTokenStack��������ȻҲ����Ҫѹ�룩
				{
					//������㣬ѹ����ջ���Ǻ��Ѿ��ڼǺ�ջSynTokenStack�У�
					node = new Node;
					tempToken = SynTokenStack.top();
					node->name=tempToken.attr;
					if (indexOfP==17)
					{
						node->kind=19;//PI
						node->value=NUM_PI;
					}
					else if (indexOfP==18)
					{
						node->kind=20;//E
						node->value=NUM_E;
					}
					else
					{
						node->kind=21;//num
						node->value=stod(tempToken.attr);
					}
					NodeStack.push(*node);//������
				}
				//id
				if (indexOfP==16)//��ʶ��,����Ҫ���ǺŴӵ���SynTokenStack��������ȻҲ����Ҫѹ�룩
				{
					//������㣬ѹ����ջ
					node = new Node;
					node->kind = 22;//id
					tempToken = SynTokenStack.top();	//���Ǻŵ�ֵ��Ϊ���Ը�ֵ�����
					node->name=tempToken.attr;
					node->flag=false;				//��ʶ��ֻ���壬��δ��ʼ��
					NodeStack.push(*node);			//������
					//��д���ű�
					SymbolTable.insert({{tempToken.attr,-1}});//û�г�ֵ
				}
				//˫������
				if (indexOfP==4||indexOfP==5||indexOfP==7||indexOfP==8||indexOfP==11||indexOfP==13)
				{
					//SynTokenStack���������Ǻţ����´���һ���Ǻţ����üǺ�ѹ��
					//NodeStack���������ڵ���Ϊ��ǰ�ڵ�����Һ��ӣ�������ǰ���ѹ��
					node=new Node();
					node->rightChild=&NodeStack.top();	//�Ҳ�����
					NodeStack.pop();//����һ�����
					SynTokenStack.pop();//����һ���Ǻ�

					node->leftChild=&NodeStack.top();//�������
					NodeStack.pop();//����һ�����
					SynTokenStack.pop();//����һ���Ǻ�

					//������ҽ���Ƿ��Ǳ�ʶ�����ǣ��鿴flags�Ƿ��и�ֵ��
					//û�и�ֵ��������֮��Ľ������Ϊid������Ϊnum//�ǣ�������ű���ʶ�������ã�����ķ�������Ҫ�������ֱ��ʹ��....��
					token = new Token();		//�����Ǻ�
					// if(IsIdentifier(node->leftChild->kind)==true||IsIdentifier(node->rightChild->kind)==true)
					// {
					// 	node->kind = 22;//id
					// 	token->kind = 22;//id
					// 	idFlag =1;
					// }
					// else
					// {
					// 	node->kind=21;//num
					// 	token->kind = 21;//num
					// }
					#pragma region
					if (indexOfP==4)	//+
					{
						node->name = '+';
						node->kind=8;//+
						// if (idFlag)//id
						// {
						// 	node->flag = 0;
						// 	token->attr='+';
						// }
						// else//num
						// {
						// 	node->value=node->leftChild->value + node->rightChild->value;
						// 	node->flag=true;
						// 	token->attr=to_string(node->value);
						// }
						NodeStack.push(*node);
					}
					else if (indexOfP==5)	//-
					{
						node->name = '-';
						node->kind=9;
						// if (idFlag)//id
						// {
						// 	node->flag = 0;
						// 	token->attr='-';
						// }
						// else//num
						// {
						// 	node->value=node->leftChild->value - node->rightChild->value;
						// 	node->flag=true;
						// 	token->attr=to_string(node->value);
						// }
						NodeStack.push(*node);
					}
					else if (indexOfP==7)	//*
					{
						node->name = '*';
						node->kind=10;
						// if (idFlag)//id
						// {
						// 	node->flag = 0;
						// 	token->attr='*';
						// }
						// else//num
						// {
						// 	node->value=node->leftChild->value * node->rightChild->value;
						// 	node->flag=true;
						// 	token->attr=to_string(node->value);
						// }
						NodeStack.push(*node);
					}
					else if (indexOfP==8)	// /
					{
						node->name = '/';
						node->kind=11;
						// if (idFlag)//id
						// {
						// 	node->flag = 0;
						// 	token->attr='/';
						// }
						// else//num
						// {
						// 	node->value=node->leftChild->value / node->rightChild->value;
						// 	node->flag=true;
						// 	token->attr=to_string(node->value);
						// }
						NodeStack.push(*node);
					}
					else if (indexOfP==11)	// ^
					{
						node->name = '^';
						node->kind=13;
						// if (idFlag)//id
						// {
						// 	node->flag = 0;
						// 	token->attr='^';
						// }
						// else//num
						// {
						// 	node->value=pow(node->leftChild->value,node->rightChild->value);
						// 	node->flag=true;
						// 	token->attr=to_string(node->value);
						// }
						NodeStack.push(*node);
					}
					else if (indexOfP==13)	// log(E,E)
					{
						node->name = 'log';
						node->kind=7;
						// if (idFlag)//id
						// {
						// 	node->flag = 0;
						// 	token->attr='log';
						// }
						// else//num
						// {
						// 	node->value=log(node->rightChild->value) / log(node->leftChild->value);
						// 	node->flag=true;
						// 	token->attr=to_string(node->value);
						// }
						NodeStack.push(*node);
					}
					#pragma endregion
				}
				//��������	=,?,sin,cos,log,lg,ln,log
				if (indexOfP==2||indexOfP==3||indexOfP==12||indexOfP==8||indexOfP==11||indexOfP==13)
				{
					Token arg1,op;
					node = new  Node();//�������
					node->leftChild =& NodeStack.top();
					NodeStack.pop();//����һ�����

					arg1 = SynTokenStack.top();//��ȡ������
					SynTokenStack.pop();//����������
					op = SynTokenStack.top();//��ȡ�����
					SynTokenStack.pop();//���������

				}
				if (indexOfP==7)//log(E)
				{
					/* code */
				}
				//��ӡ��ǰջ����
				//DisplaySymbolStateStack(symbolStateStack);
			}
			else if (sORr == 'a') //����״̬
			{
				cout << "accept!" << endl;
				break;
				//return 0;
			}
			else //�������Ϊ��
			{
				perror("Lookup table, the content is empty��");
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
}

/**
 * @description: �ж��Ƿ��Ǳ�ʶ��
 * @param {type}
 * @return:
 */
bool IsIdentifier(int kind)
{
	if (kind == 22)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int GetIndexFromT(int token)
{
	switch (token)
	{
	case 18:
		return 0; //?
	case 16:
		return 1; //;
	case 22:
		return 2; //id==>a
	case 19:
		return 3; //PI==>P
	case 20:
		return 4; //E==>e
	case 21:
		return 5; //num==>n
	case 8:
		return 6; //+
	case 9:
		return 7; //-
	case 10:
		return 8; //*
	case 11:
		return 9; ///
	case 12:
		return 10; //=
	case 13:
		return 11; //^
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		return 12; //fun==>f sin,cos,cg,ctg,lg,ln
	case 7:
		return 13; //log==>g
	case 14:
		return 14; //(
	case 15:
		return 15; //)
	case 17:
		return 16; //,
	case 23:
		return 17; //#
	default:
		return -1;
	}
}

int GetIndexFromN(char N)
{
	switch (N)
	{
	case 'S':
		return 0;
	case 'A':
		return 1;
	case 'B':
		return 2;
	case 'C':
		return 3;
	case 'D':
		return 4;
	case 'E':
		return 5;
	default:
		return -1;
	}
}
int EnCodeLeftOfP(char leftOfP)
{
	switch (leftOfP)
	{
	case 'S':
		return 1000;
	case 'A':
		return 1001;
	case 'B':
		return 1010;
	case 'C':
		return 1011;
	case 'D':
		return 1100;
	case 'E':
		return 1101;
	default:
		return -1;
	}
}

void DisplayTokenStream(queue<int> tokenStream)
{
	//��ǰ���루�Ǻ�����
	while (!tokenStream.empty())
	{
		cout << tokenStream.front() << ' ';
		tokenStream.pop();
	}
	cout << endl;
}
void DisplaySymbolStateStack(stack<int> symbolStateStack)
{
	//cout<<"ջ����";
	while (!symbolStateStack.empty())
	{
		cout << symbolStateStack.top() << ' ';
		symbolStateStack.pop();
	}
	cout << endl;
}

//�ʷ��������躯��
#pragma region
void LexicalAnalysis()
{
	int Kind(int finishState, string str,unordered_map<string,int> Hash);
	int Move(int curState, char ch);
	bool isFinishState(int curState);
	char ch;
	int curState = 0, kind, lastKind, lastState = 0;
	string str; //�ݴ������ַ�
	fstream infile("sourceCode.txt", ios::in);
	unordered_map<string, int> Hash;
	//�Ǻű�
	Hash.insert({
		{"sin", 1},
		{"cos", 2},
		{"tg", 3},
		{"ctg", 4},
		{"lg", 5},
		{"ln", 6},
		{"log", 7},
		{"+", 8},
		{"-", 9},
		{"*", 10},
		{"/", 11},
		{"=", 12},
		{"^", 13},
		{"(", 14},
		{")", 15},
		{";", 16},
		{",", 17},
		{"?", 18},
		{"PI", 19},
		{"E", 20},
		{"num", 21},
		{"id", 22},
		{"#", 23},
		{"�Ƿ��ַ�", 24},
	});
//��ȡ�ļ�
	if (!infile)
	{
		perror("�ļ���ʧ�ܣ�");
		exit(0);
	}
	Token *token;
	//��ȡ�ַ������ļ�ĩβ
	while ((ch = infile.get()) != EOF)
	{
		switch (ch)
		{
		//�ַ�Ϊ\n��\t����ն��У�״̬��ΪS0
		case '\n':
		case '\t':
		case ' ':
		{
			continue;
		}
		default:
		{
			//�����һ���ֱ�����num��id��PI��E��÷����Ǽ���
			if (ch == '-')
			{ //����
				if (lastKind == 22 || lastKind == 21 || lastKind == 19 || lastKind == 20)
				{
					str = str + ch;
					kind = Hash[str];
					if (kind) //�ڼǺű����ҵ��ֱ���
					{
						token = new Token(); //�����Ǻ�
						token->kind = kind;
						token->attr = str;
						tokenStream.push(*token); //���뻺�����
						lastKind = kind;
						//cout << "<" << str << "," << kind << ">" << endl;
						curState = 0;
						lastState = 0;
						str = "";
						continue;
					}
					else
					{
						perror("�Ƿ��ַ�!");
						//return 0;
					}
				}
			}
			//���š�����������Ƿָ���
			curState = Move(curState, ch);
			if (curState != -1)
			{
				lastState = curState;
				str = str + ch;
				continue;
			}
			else
			{
				//�ж���һ��״̬�Ƿ��ǽ���״̬
				if (isFinishState(lastState)) //�ǽ���״̬
				{
					kind = Kind(lastState, str,Hash);
					if (kind != -1)
					{
						token = new Token(); //�����Ǻ�
						token->kind = kind;
						token->attr = str;
						tokenStream.push(*token); //���뻺�����
						//tokenStream.push(kind); //���뻺�����
						lastKind = kind;
						//cout << "<" << str << "," << kind << ">" << endl;
						str = ch;
						curState = 0;
						curState = Move(curState, ch);
						lastState = curState;
						if (curState == 7)
						{
							kind = Kind(curState, str,Hash);
							token = new Token(); //�����Ǻ�
							token->kind = kind;
							token->attr = str;
							tokenStream.push(*token); //���뻺�����
							//cout << "<" << str << "," << kind << ">" << endl;
							str = "";
							curState = 0;
							lastState = 0;
						}
					}
					else
					{
						perror("�Ƿ��ַ���");
					}
				}
				else //���ǽ���״̬
				{
					perror("�ǽ���״̬��");
					//return 0;
				}
			}
		}
		}
	}
}
/*
״̬ת�ƺ���
*/
int Move(int curState, char ch)
{
	switch (curState)
	{
	case 0:
		switch (ch)
		{
		//��ĸ
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
		//����0
		case '0':
			return 4;
		//����1~9
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
		//�»���
		case '_':
			return 1;
		//����
		case '-':
			return 2;
		//��������ָ���
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
		//�Ƿ��ַ�
		default:
			return -1;
		}
	case 1:
		switch (ch)
		{
		//��ĸ������0~9���»���
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
		//�Ƿ��ַ�
		default:
			return -1;
		}
	case 2:
		switch (ch)
		{
		//����0
		case 0:
			return 4;
		//����1~9
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
		//�Ƿ��ַ�
		default:
			return -1;
		}
	case 3:
		switch (ch)
		{
		//����0~9
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
		//С����
		case '.':
			return 5;
		//�Ƿ��ַ�
		default:
			return -1;
		}
	case 4:
		switch (ch)
		{
		//С����
		case '.':
			return 5;
		//�Ƿ��ַ�
		default:
			return -1;
		}
	case 5:
	case 6:
		switch (ch)
		{
		//����0
		case 0:
			return 5;
		//����1~9
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
		//�Ƿ��ַ�
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

int Kind(int finishState, string str,unordered_map<string,int> Hash)
{
	int kind;
	switch (finishState)
	{
	//����״̬Ϊ1����ʶ�������йؼ��֣�
	case 1:
	{
		kind = Hash[str];
		if (kind) //�ǹؼ���
		{
			return kind;
		}
		else //�ǹؼ���
		{
			kind = 22; //id�ֱ��룺22
			return kind;
		}
	}
	//����״̬Ϊ3��4��6������
	case 3:
	case 4:
	case 6:
		kind = 21; //num�ֱ��룺21
		return kind;
	//����״̬Ϊ7���ָ������������
	case 7:
		kind = Hash[str];
		return kind;
	default:
		return -1;
	}
}
#pragma endregion
