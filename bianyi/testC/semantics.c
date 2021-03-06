#include<stdio.h>
#include<ctype.h>
#include<conio.h>
#include<string.h>
#include"stdlib.h"
#define maxvartablep 500 	//定乂符号表的容量


// 要将name_def函数中for中的i==0改为i>=0。
// 直接初始化方式文件路径
// compound_stat()函数上少了对’}’的判断
// 符号表结构体中定义一个initvalue变量
// 结构体中增加一个initvalue去用于标记符号是否已经赋值，即赋值状态，0表示未赋初值，1表示已赋初值。
// 修改插入符号表动作函数，定义初始0
// expression函数、read_stat函数，readchang
// program函数
// factor initcheck
// 增加修改赋值状态的函数initchange
// 增加判断是否已经赋值的函数initcheck

// 虚拟机输入路径修改
int TESTparse();
int program();
int compound_stat();
int statement();
int expression_stat();
int expression();
int bool_expr();
int additive_expr();
int term();
int factor();
int if_stat();
int while_stat();
int for_stat();
int write_stat();
int read_stat();
int declaration_stat();
int declaration_list();
int statement_list();
int compound_stat();
int name_def(char* name);
char token[20], token1[40];          //token保存单词符号,token1保存单词值

char Scanout[] = "词法分析结果.txt";
char Codeout[] = "语法及语义分析结果.txt";

FILE* fp, * fout;                     //用于指向输入输出文件的指针

struct {                               //定义符号表结构
	char name[20];
	int address;
	///
	int initvalue;							//是否赋初值 0为未赋初值
	///
} vartable[maxvartablep];             //改符号表最多容纳maxvarltablep个记录

int vartablep = 0, labelp = 0, datap = 0;

//插入符号表动作@name-def↓n,t的程序如下：
int name_def(char* name) {
	int i, es = 0;
	if (vartablep >= maxvartablep) return(21);
	for (i = vartablep - 1; i >= 0; i--) {            //查符号表
		if (strcmp(vartable[i].name, name) == 0) {
			es = 22;                                    //22表示变量重复声明
			break;
		}
	}
	if (es > 0) return(es);
	strcpy(vartable[vartablep].name, name);
	vartable[vartablep].address = datap;
	///
	vartable[vartablep].initvalue = 0;                     //变量未赋初值
	///
	datap++;                                     //分配一个单元,数据区指针加1
	vartablep++;
	return(es);
}

//复制后初始化变为1
int initchange(char* name) {
	int i, es = 0;
	for (i = 0; i < vartablep; i++) {
		if (strcmp(vartable[i].name, name) == 0) {
			vartable[i].initvalue = 1;	//
			return(es);
		}
	}
	es = 23;                                       //变量没有声明(在这里没什么用)
	return(es);
}

int initcheck(char* name) {
	int i, es = 0;
	for (i = 0; i < vartablep; i++) {
		if (strcmp(vartable[i].name, name) == 0) {
			if (vartable[i].initvalue == 0) {//为0表示未赋值
				es = 24;					//变量未赋值
				break;
			}
			
		}
	}
	return(es);
}

//查询符号表返回地址
int lookup(char* name, int* paddress) {
	int i, es = 0;
	for (i = 0; i < vartablep; i++) {
		if (strcmp(vartable[i].name, name) == 0) {
			*paddress = vartable[i].address;
			return(es);
		}
	}
	es = 23;                                       //变量没有赋初值
	return(es);
}



//语法、语义分析及代码生成程序
int TESTparse() {
	int es = 0;
	//fp = fopen

//	printf("请输入词法分析结果文件名(包括路径):");
//	scanf("%s", Scanout);
	if ((fp = fopen(Scanout, "r")) == NULL) {
		printf("\n打开%s错误！\n", Scanout);
		es = 10;
		return(es);
	}
	//	printf("请输入目标文件名(包括路径):");
	//	scanf("%s", Codeout);/*ERROR*/
	if ((fout = fopen(Codeout, "w")) == NULL) {
		printf("\n 创建 %s 错误！ \n", Codeout);
		es = 10;
		return(es);
	}
	if (es == 0) es = program();
	printf("==语法、语义分析及代码生成程序结果==\n");
	switch (es) {
	case 0:
		printf("语法、语义分析成功并抽象机汇编生成代码!\n");
		break;
	case 10:
		printf("打开文件 %s 失败\n", Scanout);
		break;
	case 1:
		printf("缺少{! \n");
		break;
	case 2:
		printf("缺少}! \n");
		break;
	case 3:
		printf("缺少标识符!");
		break;
	case 4:
		printf("少分号! \n");
		break;
	case 5:
		printf("缺少(！\n");
		break;
	case 6:
		printf("缺少)！\n");
		break;
	case 7:
		printf("缺少操作数!\n");
		break;
	case 21:
		printf("符号表溢出！\n");
		break;
	case 22:
		printf("变量重复定义！\n");
		break;
	case 23:
		printf("变量未声明!\n");
		break;
	case 24:
		printf("变量未赋初值!\n");
		break;
	}

	fclose(fp);
	fclose(fout);
	return(es);
}

//program::= {<declaration_list><statement list> }
int program() {
	int es = 0, i;
	fscanf(fp, "%s %s\n", token, token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "{")) {                             //判断是否"{"
		es = 1;
		return(es);
	}
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = declaration_list();
	if (es > 0) return(es);
	printf("符号表\n");
	/////////////
	printf("名字\t地址\t初值\n");						//增加初值显示
	
	for (i = 0; i < vartablep; i++)
		printf("%s\t%d\t%d\n", vartable[i].name, vartable[i].address, vartable[i].initvalue);

	/////////////
	es = statement_list();
	if (es > 0) return(es);
	if (strcmp(token, "}")) { //判断是否"}"
		es = 2;
		return(es);
	}
	fprintf(fout, "	STOP\n");	            //产生停止指令
	return(es);
}
//<declaration_list>::=<declaration_list><declaration_stat>|<declarartion_stat>
//改成<declaration_list>::={<declarationi_stat>}
int declaration_list() {
	int es = 0;
	while (strcmp(token, "int") == 0) {
		es = declaration_stat();
		if (es > 0) return(es);
	}
	return(es);
}

//<declaration_stat>↓vartablep,datap,codep->int ID↑n@name-def↓n,t;
int declaration_stat() {
	int es = 0;
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "ID")) return(es = 3);	     //不是标识符
	es = name_def(token1);	                     //插入符号表
	if (es > 0) return(es);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, ";")) return(es = 4);
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s%s\n", token, token1);
	return(es);
}

//<statement_list>::=<statement_list><statement>|<statement>
//改成<statement_list>::={<statement>}
int statement_list() {
	int es = 0;
	while (strcmp(token, "}")) {
		es = statement();
		if (es > 0) return(es);
	}
	return(es);
}

//<statement> ::*<if_stat> |<while_stat> |<for_stat>
//  	        |<compound_stat> |<expression_stat>
int statement() {
	int es = 0;
	if (es == 0 && strcmp(token, "if") == 0)       es = if_stat();//<if语句>
	if (es == 0 && strcmp(token, "while") == 0) es = while_stat();//<while 语句>
	if (es == 0 && strcmp(token, "for") == 0)
		es = for_stat();				//<for语句>
	//可在此处添加过do语句调用
	if (es == 0 && strcmp(token, "read") == 0)  es = read_stat();//<read 语句>
	if (es == 0 && strcmp(token, "write") == 0) es = write_stat();//<write @^>
	if (es == 0 && strcmp(token, "{") == 0)es = compound_stat();//<复合语句>
	if (es == 0 && (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0))
		es = expression_stat();	        //<表达式语句>
	return(es);
}

//<if_stat>::=if(<expr)<statement>[else<statement>]
/*
if(<expression>)@BRF↑label1<statement>@BR↑label2@SETlabel↓label1
[else<statement>]@SETlabel↓label2
其动作符号的含义如下
@BRF↑label1：输出BRF labell
@BR↑label↓label1:设置标号label1
@SETlabel↓label2：设置标号label2
*/
int if_stat() {
	int es = 0, label1, label2;                      //if
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "(")) return(es = 5);        //少左括号
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	if (strcmp(token, ")")) return(es = 6);      //少右括号
	label1 = labelp++;                            //用label1记住条件为假时要转向的标号
	fprintf(fout, "	BRF LABEL%d\n", label1);//输出假转移指令
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = statement();
	if (es > 0) return(es);
	label2 = labelp++;                            //用label2记住要转向的挥号
	fprintf(fout, "		BR LABEL%d\n", label2);      //输出无条件转移指令
	fprintf(fout, "LABEL%d: \n", label1);       //设置label1记住的标号
	if (strcmp(token, "else") == 0) {             //else部分处理
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s %s\n", token, token1);
		es = statement();
		if (es > 0) return(es);
	}


	fprintf(fout, "LABEL%d:\n", label2);                    //设置label2记住的标号
	return(es);

}

//<while_stat> :: = while(<expression> ) <statement>
//<while_stat>::=while@SETlabel↑label1(expression>)@BRF↑label2
//				<statement>@BR↓label1@SETlabel↓label2
//动作解释如下：
//@SETlabel↑label1：设置标号 lable1
//@BRF↑label2：输出 BRF label2
//@BR↓label1：输出 BR label1
//@SETlabel↓label2：设置标号 label2
int while_stat() {
	int es = 0, label1, label2;
	label1 = labelp++;
	fprintf(fout, "LABEL%d:\n", label1);	//设置1标号
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "(")) return(es = 5);	//少左括号
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	if (strcmp(token, ")")) return(es = 6);	//少右括号
	label2 = labelp++;
	fprintf(fout, "		BRF LABEL%d\n", label2); //输出假转移指令
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = statement();
	if (es > 0) return(es);
	fprintf(fout, "		BR LABEL%d\n", label1);	//输出无条件转移指令
	fprintf(fout, "LABEL%d:\n", label2);	    //设置2标号.
	return(es);

}



//< for_stat> ::for(<expr> ,<expr> ,<expr>) < statement>
/*
<for_stat>::=for(<expression>@POP;
@SETlabel↑label1<expression>@BRF↑label2@BR↓label3;
@SETlabel↑label4<expression>@POP@BR↓label1)
@SETlabel↑label3<语句>@BR↓label4@SETlabel↑label2
动作解释：
1、@SETlabel↑label1：设置标号 label1
2、@BRF↑label2：输出 BRF label2
3、@BR↓label3：输出 BR label3
4、@SETlabel↑label4：设置标号 label1
5、@BR↓label1：输出 BR label1
6、@SETlabel↑label3：设置标号 label3
7、@BR↓label4：输出 BR label4
8、@SETlabel↑label2：设置标号 label2
*/
int for_stat() {
	int es = 0, label1, label2, label3, label4;
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "(")) return(es = 5);	//少左括号
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	fprintf(fout, "	POP\n");                 //输出出栈指令
	if (strcmp(token, ";")) return(es *= 4);//少分号
	label1 = labelp++;
	fprintf(fout, "LABEL%d:\n", label1);       //设置 label1标号
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	label2 = labelp++;
	fprintf(fout, " 	BRF LABEL%d\n", label2); //输出假条件转移指令
	label3 = labelp++;
	fprintf(fout, "	    BR LABEL%d\n", label3);   //输出无条件转移指令
	if (strcmp(token, ";"))  return(es = 4);         //少分号
	label4 = labelp++;
	fprintf(fout, "LABEL%d:\n", label4);         //设置label4标号
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	fprintf(fout, "	POP\n");                //输出出找指令
	fprintf(fout, "	BR LABEL%d\n", label1);     //输出无条件转移指令
	if (strcmp(token, ")")) return(es = 6);        //少右括号
	fprintf(fout, "LABEL%d:\n", label3);          //设置label3标号
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = statement();
	if (es > 0) return(es);
	fprintf(fout, "	BR LABEL%d\n", label4);     //输出无条件转移指令
	fprintf(fout, "LABEL%d:\n", label2);       //设置label2标号
	return(es);

}

//<write_stat>::=write<expression>;
//<write_stat>::=write<expression>@OUT;
//动作解释：
//@OUT：输出OUT
int write_stat() {
	int es = 0;
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	if (strcmp(token, ";")) return(es = 4);	//少分号
	fprintf(fout, "	OUT\n");	            //输出0ut指令
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	return(es);
}

//<read_stat>::==read ID;
//<read_stat>::read ID
//<read_stat>::read ID↑n@LOOK↓n↑d@IN@STO↓d@POP;
//动作解释：
//@LOOK↓n↑d：查符号表n，给出变量地址d，若没有，则变量未定义
//@IN:输出 IN
//@STO↓d：输出指令代码STO d
int read_stat() {
	int es = 0, address;
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "ID")) return(es = 3);    //少标识符
	es = lookup(token1, &address);
	if (es > 0) return(es);
	////
	initchange(token1);	//赋值
	////
	fprintf(fout, "	IN\n");               //输入指令
	fprintf(fout, "	STO %d\n", address);    //输出STO指令
	fprintf(fout, "	POP\n");
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, ";")) return(es = 4);    //少分号
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s%s\n", token, token1);
	return(es);
}


//<compound_stat> ::= {<statement_list>}
//复合语句函数
int compound_stat() {
	int es = 0;
	fscanf(fp, "%s %s\n", &token, &token1); //复合语句函数
	printf("%s %s\n", token, token1);
	es = statement_list();
	if (es > 0) return (es);
	//ERROR
	if (strcmp(token, "}")) return(es = 2);	//少右大括号
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	return(es);
}


//<expression_stat>::==<expression> @ POP;|;
int expression_stat() {
	int es = 0;
	if (strcmp(token, ";") == 0) {
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s%s\n", token, token1);
		return(es);
	}
	es = expression();
	if (es > 0) return(es);
	fprintf(fout, "	POP\n");       //输出POP指令
	if (strcmp(token, ";") == 0) {
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s %s\n", token, token1);
		return(es);
	}
	else {
		es = 4;
		return(es);                //少分号
	}
}

//<expression>::=ID↑n@LOOK↓n↑d@ASSIGN=<bool_expr>@STO↓d@POP|<bool_expr>
int expression() {
	int es = 0, fileadd;
	char token2[20], token3[40], token4[40];
	if (strcmp(token, "ID") == 0) {
		//////
		strcpy(token4, token1);//保留原ID变量用于赋初值
		//////
		fileadd = ftell(fp);	//@ASSIGN己住当前文件位置
		fscanf(fp, "%s %s\n", &token2, &token3);
		printf("%s %s\n", token2, token3);
		if (strcmp(token2, "=") == 0) {
			int address;
			es = lookup(token1, &address);
			fscanf(fp, "%s %s\n", &token, &token1);
			printf("%s %s\n", token, token1);
			
			es = bool_expr();
			if (es > 0)  return(es);
			////////
			initchange(token4);//赋值后初始值状态边为1，说明已赋初值
			////////
			fprintf(fout, "   STO %d\n", address);

			printf("\t符号表\n");
			printf("\t名字\t地址\t初值\n");
			for (int i = 0; i < vartablep; i++)
				printf("\t%s\t%d\t%d\n", vartable[i].name, vartable[i].address, vartable[i].initvalue);

		}
		else {
			fseek(fp, fileadd, 0);	//文件指针回到之前的标识符
			printf("%s %s\n", token, token1);
			es = bool_expr();
			if (es > 0) return(es);
		}
	}
	else es = bool_expr();

	return(es);
}

//<bool_expr>::=<additive_expr>
//	|<additive_expr>(>|<|>=|<=|==|!=)<additive_expr>
/*
<bool_expr>::=<additive_expr>
|<additive_expr>><additive_expr>@GT
|<additive_expr><<additive_expr>@LES
|<additive_expr>>=<additive_expr>@GE
|<additive_expr><=<additive_expr>@LE
|<additive_expr>==<additive_expr>@EQ
|<additive_expr>!=<additive_expr>@NOTEQ
*/
int bool_expr() {
	int es = 0;
	es = additive_expr();
	if (es > 0) return(es);
	if (strcmp(token, ">") == 0 || strcmp(token, ">=") == 0 || strcmp(token, "<") == 0 || strcmp(token, "<=") == 0 || strcmp(token, "==") == 0 || strcmp(token, "!=") == 0) {
		char token2[20];
		strcpy(token2, token);	//保存运算符
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s%s\n", token, token1);
		es = additive_expr();
		if (es > 0) return(es);
		if (strcmp(token2, ">") == 0) fprintf(fout, "   GT\n");
		if (strcmp(token2, ">=") == 0) fprintf(fout, "   GE\n1");
		if (strcmp(token2, "<") == 0) fprintf(fout, "   LES\n");
		if (strcmp(token2, "<=") == 0) fprintf(fout, "   LE\n");
		if (strcmp(token2, "==") == 0) fprintf(fout, "   EQ\n");
		if (strcmp(token2, "!=") == 0) fprintf(fout, "   NOTEQ\n");
	}

	return(es);
}

//<additive_expr>::<term>{(+|-)<term>}
//<additibe_expr>::<term>{(+<term>@ADD|-<term>@SUB)}
int additive_expr() {
	int es = 0;
	es = term();
	if (es > 0) return(es);
	while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0) {
		char token2[20];
		strcpy(token2, token);
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s %s\n", token, token1);
		es = term();
		if (es > 0) return(es);
		if (strcmp(token2, "+") == 0) fprintf(fout, "	ADD\n");
		if (strcmp(token2, "-") == 0)fprintf(fout, "	SUB\n");
	}
	return(es);
}

//<term>::=<factor>{(*|/)<factor>}
//<term>::=<factor>{(*<factor>@MULT|/<factor>@DIV)}
int term() {
	int es = 0;
	es = factor();
	if (es > 0) return(es);
	while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
		char token2[20];
		strcpy(token2, token);
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s %s\n", token, token1);
		es = factor();
		if (es > 0) return(es);
		if (strcmp(token2, "*") == 0) fprintf(fout, "	MULT\n");
		if (strcmp(token2, "/") == 0)fprintf(fout, "	DIV\n");
	}

	return(es);

}

//<factor>::=(<additive_expr>)|ID|NUM
//<factor>::=(<expressioin>)|ID↑n@LOOK↓n↑d@LOAD↓d|NUM↑i@LOADI↓i
int factor() {
	int es = 0;
	if (strcmp(token, "(") == 0) {
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s %s\n", token, token1);
		es = expression();
		if (es > 0) return(es);
		if (strcmp(token, ")")) return(es = 6);	//少右括号
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s %s\n", token, token1);
	}
	else {
		if (strcmp(token, "ID") == 0) {
			int address;
			es = lookup(token1, &address);	//査符号表,获取变量地址
			if (es > 0) return(es);	        //变量没声明

			///////
			es = initcheck(token1);			//判断是否赋初值
			//////
			if (es > 0) return(es);			//变量未赋初值

			fprintf(fout, "	LOAD %d\n", address);
			fscanf(fp, "%s %s\n", &token, &token1);
			printf("%s %s\n", token, token1);
			return(es);
		}
		if (strcmp(token, "NUM") == 0) {
			fprintf(fout, "	LOADI %s\n", token1);
			fscanf(fp, "%s %s\n", &token, &token1);
			printf("%s %s\n", token, token1);
			return(es);
		}
		else {
			es = 7;	//缺少操作数
			return(es);
		}
	}

	return(es);
}