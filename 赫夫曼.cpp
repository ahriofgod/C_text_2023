#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#define MAXVALUE 32767		//极大值相当于无穷大
#define NODENUM 30			//叶子结点数
//在testbook.txt中输入需要压缩文本内容（字符包括26个小写字母、",(逗号)"、 ".(句号)"、 "!(叹号)"、 " (空格)"）


typedef struct
{
	char data;				//数据域
	int weight;				//结点的权值
	int parent, lch, rch;	//双亲与孩子的下标
}htNode, * huffmanTree;

typedef char** huffmanCode;	//第一个*是代表它是指针变量，说明它是数组
//第二个*说明它是指针数组，代表这个char类型数组里每个元素都是*huffmanCode变量

int initHuffmanTree(huffmanTree& HT,int* arr);								//初始化哈夫曼树
void creatHuffmanTree(huffmanTree& HT, int n);						//构建哈夫曼树
void createHuffmanCode(huffmanTree HT, huffmanCode& HC, int n);		//编写哈夫曼编码
void read(int* arr);
void write(huffmanTree HT, huffmanCode HC,int* arr);      //生成说明文本
void writeBinarytxt(huffmanCode HC);     //生成二进制码文本
int convert(char* arr);    //8位二进制转十进制
void writetxt();    //生成zip文本

int main()
{
	int arr[31] = { 0 };
	read(arr);
	huffmanTree HT;
	initHuffmanTree(HT,arr);
	huffmanCode HC;
	creatHuffmanTree(HT, NODENUM);
	createHuffmanCode(HT, HC, NODENUM);
	/*for (int i = NODENUM + 1; i <= 2 * NODENUM - 1; i++)
		printf("%d ", HT[i].weight);*/
	for (int i = 1; i <= NODENUM; i++)								//遍历输出编码
	{
		printf("%c:\t", HT[i].data);
		printf("%s\n", HC[i]);
	}

	
	write(HT, HC,arr);
	writeBinarytxt(HC);
	writetxt();
	return 0;
}

void read(int* arr)
{
	FILE* pk = fopen("testbook.txt", "r+");
	int ch = fgetc(pk);
	printf("intput the Original text!\n");
	while (!feof(pk))
	{
		arr[0]++;
		if (ch == 'a')
			arr[1]++;
		else if (ch == 'b')
			arr[2]++;
		else if (ch == 'c')
			arr[3]++;
		else if (ch == 'd')
			arr[4]++;
		else if (ch == 'e')
			arr[5]++;
		else if (ch == 'f')
			arr[6]++;
		else if (ch == 'g')
			arr[7]++;
		else if (ch == 'h')
			arr[8]++;
		else if (ch == 'i')
			arr[9]++;
		else if (ch == 'j')
			arr[10]++;
		else if (ch == 'k')
			arr[11]++;
		else if (ch == 'l')
			arr[12]++;
		else if (ch == 'm')
			arr[13]++;
		else if (ch == 'n')
			arr[14]++;
		else if (ch == 'o')
			arr[15]++;
		else if (ch == 'p')
			arr[16]++;
		else if (ch == 'q')
			arr[17]++;
		else if (ch == 'r')
			arr[18]++;
		else if (ch == 's')
			arr[19]++;
		else if (ch == 't')
			arr[20]++;
		else if (ch == 'u')
			arr[21]++;
		else if (ch == 'v')
			arr[22]++;
		else if (ch == 'w')
			arr[23]++;
		else if (ch == 'x')
			arr[24]++;
		else if (ch == 'y')
			arr[25]++;
		else if (ch == 'z')
			arr[26]++;
		else if (ch == ',')
			arr[27]++;
		else if (ch == '.')
			arr[28]++;
		else if (ch == '!')
			arr[29]++;
		else if (ch == ' ')
			arr[30]++;
		printf("%c", ch);
		ch = fgetc(pk);
	}
	fclose(pk);
	pk = NULL;
	printf("\n\n");
	
}

int initHuffmanTree(huffmanTree& HT,int* arr)
{
	
	HT = (htNode*)malloc(sizeof(htNode) * (2 * NODENUM));			//给HT分配2 * NODENUM个htNOde大小的htNode类型的数组
	for (int i = 1; i <= 2 * NODENUM - 1; i++)						//下标从1开始到2 * NODENUM
	{
		HT[i].parent = HT[i].lch = HT[i].rch = -1;					//双亲和孩子的值都置为-1
	}
	//printf("input some weight!\n");
	for (int i = 1; i <= NODENUM; i++)								//权值只有1-n个
	{
		HT[i].weight = arr[i];
		//scanf("%d", &HT[i].weight);									//给每个结点赋予权值
	}
	printf("total number if characters:%d\n",arr[0]);
	printf("input some data and weight!\n");
	for (int i = 1; i <= NODENUM-4; i++)
	{
		HT[i].data = 'a' + i -1;
		printf("%c(%d)(%.2lf) ", HT[i].data,arr[i], (float)arr[i]/arr[0]);
		
	}
	HT[NODENUM - 3].data = ',';
	printf("%c(%d)(%.2lf) ", HT[NODENUM - 3].data, arr[NODENUM-3] , (float)arr[NODENUM - 3] / arr[0]);
	HT[NODENUM - 2].data = '.';
	printf("%c(%d)(%.2lf) ", HT[NODENUM - 2].data, arr[NODENUM-2], (float)arr[NODENUM - 2] / arr[0]);
	HT[NODENUM - 1].data = '!';
	printf("%c(%d)(%.2lf) ", HT[NODENUM - 1].data, arr[NODENUM-1], (float)arr[NODENUM - 1] / arr[0]);
	HT[NODENUM ].data = ' ';
	printf("%s(%d)(%.2lf) ", "Space",arr[NODENUM], (float)arr[NODENUM] / arr[0]);
	printf("\n\n");
	return 1;
}

void creatHuffmanTree(huffmanTree& HT, int n)
{
	if (n <= 1)															//如果结点数小于等于1，不创建
		return;
	int min1, min2;														//定义两个数，来存储每次选取最小两个结点的权值
	int rnode, lnode;													//定义两个下标值，来存储每次选取最小两个结点的下标
	for (int i = n + 1; i <= 2 * n - 1; i++)								//要生成n-1个结点，所以要操作n—1次且从下标为n+1开始存储
	{
		int min1 = MAXVALUE; int lnode = -1;							//让最小值初始化为极大值，这样叶子结点的最大值再大也不会超过这个值了							
		int min2 = MAXVALUE; int rnode = -1;
		for (int j = 1; j <= i - 1; j++)								//因为起先是在前n个中选择最小的两个结点的权值，但新生成一个后就得在前n+1个中选择最小的两个结点的权值							
		{																//假设n = 10 总结点数就得为19，那我们就只要比较18次就可以得出结果了，记住比较的次数比生成的总结点数少1
			if (HT[j].weight < min1 && HT[j].parent == -1)			//这个小于就使得当出现相同的权值时优先考虑先出现的值，可以假设下
			{
				min2 = min1;	rnode = lnode;						//碰到比min1小的，那min1的值就给第二小的min2，下标也给它
				min1 = HT[j].weight; lnode = j;						//然后最小的给min1，下标同理
			}
			else if (HT[j].weight < min2 && HT[j].parent == -1)		//这是第二小的判断
			{
				min2 = HT[j].weight;
				rnode = j;
			}
		}
		HT[lnode].parent = HT[rnode].parent = i;						//最小两个结点的parent变为生成结点的下标
		HT[i].lch = lnode; HT[i].rch = rnode;							//生成结点的左孩子为最小的min1的下标，右孩子同理
		HT[i].weight = HT[lnode].weight + HT[rnode].weight;				//生成结点的权值等于最小结点的权值相加
	}
}

void createHuffmanCode(huffmanTree HT, huffmanCode& HC, int n)
{
	HC = (huffmanCode)malloc(sizeof(huffmanCode) * n + 1);				//申请n + 1个huffmanCode大小huffmanCode类型的临时空间
	//因为下标是从一开始，所以我们要申请比结点多一个的结点，和哈夫曼树的结构对应，方便输出
	char* cd = (char*)malloc(sizeof(char) * n);							//申请n个char大小char类型的临时空间，这个临时数组记录每次遍历出来的编码
	int start = 0, c = 0, f = 0;											//start为cd数组记录下标，c初始为叶子结点下标，而后就是孩子结点的下标，f记录双亲结点的下标
	cd[n - 1] = '\0';													//这个就是给printf留着的，因为printf不会生成'\0'，如果用puts就不用这句语句了
	for (int i = 1; i <= n; i++)										//只要叶子结点的编码
	{
		start = n - 1;													//这句要赋值n的话，start--要写在判断后方
		c = i;
		f = HT[c].parent;
		while (f != -1)													//根节点没有双亲
		{
			start--;
			if (HT[f].lch == c)											//是左孩子就是0，右孩子就为1
				cd[start] = '0';
			else
				cd[start] = '1';
			c = f; f = HT[c].parent;									//向根结点接近
		}
		HC[i] = (char*)malloc(sizeof(char) * (n - start));				//给数组里的数组申请n - start个char大小的char*类型的临时空间
		strcpy(HC[i], &cd[start]);										//cd里记录的编码给HC的第i个数组
	}
	free(cd);															//释放临时空间
}

void write(huffmanTree HT, huffmanCode HC,int* arr){
	FILE* pf = fopen("test.txt", "w+");
	if (pf == NULL)
	{
		printf("%s\n", strerror(errno));
		return ;
	}
	for (int i = 1; i <= NODENUM; i++)								//遍历输出编码
	{
		fprintf(pf, "%c:\t", HT[i].data);
		fprintf(pf, "%s\n", HC[i]);
	}
	fprintf(pf,"\ntotal number if characters:%d\n", arr[0]);
	fprintf(pf,"input some data and weight!\n");
	for (int i = 1; i <= NODENUM - 4; i++)
	{
		HT[i].data = 'a' + i - 1;
		fprintf(pf, "%c(%d) ", HT[i].data, arr[i]);
	}
	fprintf(pf,"%c(%d) ", HT[NODENUM - 3].data, arr[NODENUM - 3]);
	fprintf(pf,"%c(%d) ", HT[NODENUM - 2].data, arr[NODENUM - 2]);
	fprintf(pf,"%c(%d) ", HT[NODENUM - 1].data, arr[NODENUM - 1]);
	fprintf(pf,"%s(%d) ", "Space", arr[NODENUM]);
	fclose(pf);
	pf = NULL;
}

void writeBinarytxt(huffmanCode HC)
{
	FILE* pk = fopen("testbook.txt", "r+");
	if (pk == NULL)
	{
		printf("%s\n", strerror(errno));
		return;
	}
	int ch = fgetc(pk);
	FILE* ph = fopen("Binarytest.txt", "w+");
	if (ph == NULL)
	{
		printf("%s\n", strerror(errno));
		return;
	}
	while (!feof(pk))
	{
		if (ch == 'a')
			fprintf(ph,"%s", HC[1]);
		else if (ch == 'b')
			fprintf(ph,"%s", HC[2]);
		else if (ch == 'c')
			fprintf(ph, "%s", HC[3]);
		else if (ch == 'd')
			fprintf(ph, "%s", HC[4]);
		else if (ch == 'e')
			fprintf(ph, "%s", HC[5]);
		else if (ch == 'f')
			fprintf(ph, "%s", HC[6]);
		else if (ch == 'g')
			fprintf(ph, "%s", HC[7]);
		else if (ch == 'h')
			fprintf(ph, "%s", HC[8]);
		else if (ch == 'i')
			fprintf(ph, "%s", HC[9]);
		else if (ch == 'j')
			fprintf(ph, "%s", HC[10]);
		else if (ch == 'k')
			fprintf(ph, "%s", HC[11]);
		else if (ch == 'l')
			fprintf(ph, "%s", HC[12]);
		else if (ch == 'm')
			fprintf(ph, "%s", HC[13]);
		else if (ch == 'n')
			fprintf(ph, "%s", HC[14]);
		else if (ch == 'o')
			fprintf(ph, "%s", HC[15]);
		else if (ch == 'p')
			fprintf(ph, "%s", HC[16]);
		else if (ch == 'q')
			fprintf(ph, "%s", HC[17]);
		else if (ch == 'r')
			fprintf(ph, "%s", HC[18]);
		else if (ch == 's')
			fprintf(ph, "%s", HC[19]);
		else if (ch == 't')
			fprintf(ph, "%s", HC[20]);
		else if (ch == 'u')
			fprintf(ph, "%s", HC[21]);
		else if (ch == 'v')
			fprintf(ph, "%s", HC[22]);
		else if (ch == 'w')
			fprintf(ph, "%s", HC[23]);
		else if (ch == 'x')
			fprintf(ph, "%s", HC[24]);
		else if (ch == 'y')
			fprintf(ph, "%s", HC[25]);
		else if (ch == 'z')
			fprintf(ph, "%s", HC[26]);
		else if (ch == ',')
			fprintf(ph, "%s", HC[27]);
		else if (ch == '.')
			fprintf(ph, "%s", HC[28]);
		else if (ch == '!')
			fprintf(ph, "%s", HC[29]);
		else if (ch == ' ')
			fprintf(ph, "%s", HC[30]);
		
		ch = fgetc(pk);
	}
	fclose(ph);
	fclose(pk);
	ph = NULL;
	pk = NULL;
}

int convert(char* arr)
{
	int sum = 0;
	int q = 1;
	for (int i = 7; i >= 0; i--)
	{
		if (arr[i] == '1')
		{
			sum += q;
		}
		q *= 2;
	}
	return sum;
}

void writetxt()
{
	int mun = 0, add;
	FILE* pf = fopen("Binarytest.txt", "rb");
	if (pf == NULL)
	{
		printf("%s\n", strerror(errno));
		return;
	}
	FILE* pk = fopen("zip.txt", "w+");
	
	char arr[8] = { 0 };
	int by = 0;
	while (!feof(pf))
	{
		if (pf == NULL)
		{
			return;
		}
		size_t num = fread(arr, sizeof(char), 8, pf);
		if (num == 8)
		{
			by = convert(arr);
			fprintf(pk, "%c", by);
			/*for (int i = 0; i < 8; i++)
			{
				printf("%c", arr[i]);
			}*/
		}
		else
		{
			mun = num;
			char str[8] = { '0','0', '0', '0', '0', '0', '0','0' };
			for (int i = 0; i < num; i++)
			{
				str[i] = arr[i];
			}
			/*for (int i = 0; i < 8; i++)
			{
				printf("%c", str[i]);
			}*/
			by = convert(str);
			fprintf(pk, "%c", by);
		}
		
	}
	

	FILE* ph = fopen("test.txt", "a+");
	if (ph == NULL)
	{
		printf("%s\n", strerror(errno));
		return;
	}
	add = 8 - mun;
	fprintf(ph, "\nExtra 0 have:");
	fprintf(ph, "%d", add);
	fclose(pf);
	fclose(pk);
	fclose(ph);
	ph = NULL;
	pf = NULL;
	pk = NULL;
}