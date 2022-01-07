#include<stdio.h>
#include<ctype.h>
#include<conio.h>
#include<string.h>
#include"stdlib.h"
#define maxvartablep 500 	//���V���ű�������


// Ҫ��name_def������for�е�i==0��Ϊi>=0��
// ֱ�ӳ�ʼ����ʽ�ļ�·��
// compound_stat()���������˶ԡ�}�����ж�
// ���ű��ṹ���ж���һ��initvalue����
// �ṹ��������һ��initvalueȥ���ڱ�Ƿ����Ƿ��Ѿ���ֵ������ֵ״̬��0��ʾδ����ֵ��1��ʾ�Ѹ���ֵ��
// �޸Ĳ�����ű����������������ʼ0
// expression������read_stat������readchang
// program����
// factor initcheck
// �����޸ĸ�ֵ״̬�ĺ���initchange
// �����ж��Ƿ��Ѿ���ֵ�ĺ���initcheck

// ���������·���޸�
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
char token[20], token1[40];          //token���浥�ʷ���,token1���浥��ֵ

char Scanout[] = "�ʷ��������.txt";
char Codeout[] = "�﷨������������.txt";

FILE* fp, * fout;                     //����ָ����������ļ���ָ��

struct {                               //������ű��ṹ
	char name[20];
	int address;
	///
	int initvalue;							//�Ƿ񸳳�ֵ 0Ϊδ����ֵ
	///
} vartable[maxvartablep];             //�ķ��ű��������maxvarltablep����¼

int vartablep = 0, labelp = 0, datap = 0;

//������ű�����@name-def��n,t�ĳ������£�
int name_def(char* name) {
	int i, es = 0;
	if (vartablep >= maxvartablep) return(21);
	for (i = vartablep - 1; i >= 0; i--) {            //����ű�
		if (strcmp(vartable[i].name, name) == 0) {
			es = 22;                                    //22��ʾ�����ظ�����
			break;
		}
	}
	if (es > 0) return(es);
	strcpy(vartable[vartablep].name, name);
	vartable[vartablep].address = datap;
	///
	vartable[vartablep].initvalue = 0;                     //����δ����ֵ
	///
	datap++;                                     //����һ����Ԫ,������ָ���1
	vartablep++;
	return(es);
}

//���ƺ��ʼ����Ϊ1
int initchange(char* name) {
	int i, es = 0;
	for (i = 0; i < vartablep; i++) {
		if (strcmp(vartable[i].name, name) == 0) {
			vartable[i].initvalue = 1;	//
			return(es);
		}
	}
	es = 23;                                       //����û������(������ûʲô��)
	return(es);
}

int initcheck(char* name) {
	int i, es = 0;
	for (i = 0; i < vartablep; i++) {
		if (strcmp(vartable[i].name, name) == 0) {
			if (vartable[i].initvalue == 0) {//Ϊ0��ʾδ��ֵ
				es = 24;					//����δ��ֵ
				break;
			}
			
		}
	}
	return(es);
}

//��ѯ���ű����ص�ַ
int lookup(char* name, int* paddress) {
	int i, es = 0;
	for (i = 0; i < vartablep; i++) {
		if (strcmp(vartable[i].name, name) == 0) {
			*paddress = vartable[i].address;
			return(es);
		}
	}
	es = 23;                                       //����û�и���ֵ
	return(es);
}



//�﷨������������������ɳ���
int TESTparse() {
	int es = 0;
	//fp = fopen

//	printf("������ʷ���������ļ���(����·��):");
//	scanf("%s", Scanout);
	if ((fp = fopen(Scanout, "r")) == NULL) {
		printf("\n��%s����\n", Scanout);
		es = 10;
		return(es);
	}
	//	printf("������Ŀ���ļ���(����·��):");
	//	scanf("%s", Codeout);/*ERROR*/
	if ((fout = fopen(Codeout, "w")) == NULL) {
		printf("\n ���� %s ���� \n", Codeout);
		es = 10;
		return(es);
	}
	if (es == 0) es = program();
	printf("==�﷨������������������ɳ�����==\n");
	switch (es) {
	case 0:
		printf("�﷨����������ɹ��������������ɴ���!\n");
		break;
	case 10:
		printf("���ļ� %s ʧ��\n", Scanout);
		break;
	case 1:
		printf("ȱ��{! \n");
		break;
	case 2:
		printf("ȱ��}! \n");
		break;
	case 3:
		printf("ȱ�ٱ�ʶ��!");
		break;
	case 4:
		printf("�ٷֺ�! \n");
		break;
	case 5:
		printf("ȱ��(��\n");
		break;
	case 6:
		printf("ȱ��)��\n");
		break;
	case 7:
		printf("ȱ�ٲ�����!\n");
		break;
	case 21:
		printf("���ű������\n");
		break;
	case 22:
		printf("�����ظ����壡\n");
		break;
	case 23:
		printf("����δ����!\n");
		break;
	case 24:
		printf("����δ����ֵ!\n");
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
	if (strcmp(token, "{")) {                             //�ж��Ƿ�"{"
		es = 1;
		return(es);
	}
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = declaration_list();
	if (es > 0) return(es);
	printf("���ű�\n");
	/////////////
	printf("����\t��ַ\t��ֵ\n");						//���ӳ�ֵ��ʾ
	
	for (i = 0; i < vartablep; i++)
		printf("%s\t%d\t%d\n", vartable[i].name, vartable[i].address, vartable[i].initvalue);

	/////////////
	es = statement_list();
	if (es > 0) return(es);
	if (strcmp(token, "}")) { //�ж��Ƿ�"}"
		es = 2;
		return(es);
	}
	fprintf(fout, "	STOP\n");	            //����ָֹͣ��
	return(es);
}
//<declaration_list>::=<declaration_list><declaration_stat>|<declarartion_stat>
//�ĳ�<declaration_list>::={<declarationi_stat>}
int declaration_list() {
	int es = 0;
	while (strcmp(token, "int") == 0) {
		es = declaration_stat();
		if (es > 0) return(es);
	}
	return(es);
}

//<declaration_stat>��vartablep,datap,codep->int ID��n@name-def��n,t;
int declaration_stat() {
	int es = 0;
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "ID")) return(es = 3);	     //���Ǳ�ʶ��
	es = name_def(token1);	                     //������ű�
	if (es > 0) return(es);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, ";")) return(es = 4);
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s%s\n", token, token1);
	return(es);
}

//<statement_list>::=<statement_list><statement>|<statement>
//�ĳ�<statement_list>::={<statement>}
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
	if (es == 0 && strcmp(token, "if") == 0)       es = if_stat();//<if���>
	if (es == 0 && strcmp(token, "while") == 0) es = while_stat();//<while ���>
	if (es == 0 && strcmp(token, "for") == 0)
		es = for_stat();				//<for���>
	//���ڴ˴����ӹ�do������
	if (es == 0 && strcmp(token, "read") == 0)  es = read_stat();//<read ���>
	if (es == 0 && strcmp(token, "write") == 0) es = write_stat();//<write @^>
	if (es == 0 && strcmp(token, "{") == 0)es = compound_stat();//<�������>
	if (es == 0 && (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0))
		es = expression_stat();	        //<����ʽ���>
	return(es);
}

//<if_stat>::=if(<expr)<statement>[else<statement>]
/*
if(<expression>)@BRF��label1<statement>@BR��label2@SETlabel��label1
[else<statement>]@SETlabel��label2
�䶯�����ŵĺ�������
@BRF��label1�����BRF labell
@BR��label��label1:���ñ��label1
@SETlabel��label2�����ñ��label2
*/
int if_stat() {
	int es = 0, label1, label2;                      //if
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "(")) return(es = 5);        //��������
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	if (strcmp(token, ")")) return(es = 6);      //��������
	label1 = labelp++;                            //��label1��ס����Ϊ��ʱҪת��ı��
	fprintf(fout, "	BRF LABEL%d\n", label1);//�����ת��ָ��
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = statement();
	if (es > 0) return(es);
	label2 = labelp++;                            //��label2��סҪת��ĻӺ�
	fprintf(fout, "		BR LABEL%d\n", label2);      //���������ת��ָ��
	fprintf(fout, "LABEL%d: \n", label1);       //����label1��ס�ı��
	if (strcmp(token, "else") == 0) {             //else���ִ���
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s %s\n", token, token1);
		es = statement();
		if (es > 0) return(es);
	}


	fprintf(fout, "LABEL%d:\n", label2);                    //����label2��ס�ı��
	return(es);

}

//<while_stat> :: = while(<expression> ) <statement>
//<while_stat>::=while@SETlabel��label1(expression>)@BRF��label2
//				<statement>@BR��label1@SETlabel��label2
//�����������£�
//@SETlabel��label1�����ñ�� lable1
//@BRF��label2����� BRF label2
//@BR��label1����� BR label1
//@SETlabel��label2�����ñ�� label2
int while_stat() {
	int es = 0, label1, label2;
	label1 = labelp++;
	fprintf(fout, "LABEL%d:\n", label1);	//����1���
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "(")) return(es = 5);	//��������
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	if (strcmp(token, ")")) return(es = 6);	//��������
	label2 = labelp++;
	fprintf(fout, "		BRF LABEL%d\n", label2); //�����ת��ָ��
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = statement();
	if (es > 0) return(es);
	fprintf(fout, "		BR LABEL%d\n", label1);	//���������ת��ָ��
	fprintf(fout, "LABEL%d:\n", label2);	    //����2���.
	return(es);

}



//< for_stat> ::for(<expr> ,<expr> ,<expr>) < statement>
/*
<for_stat>::=for(<expression>@POP;
@SETlabel��label1<expression>@BRF��label2@BR��label3;
@SETlabel��label4<expression>@POP@BR��label1)
@SETlabel��label3<���>@BR��label4@SETlabel��label2
�������ͣ�
1��@SETlabel��label1�����ñ�� label1
2��@BRF��label2����� BRF label2
3��@BR��label3����� BR label3
4��@SETlabel��label4�����ñ�� label1
5��@BR��label1����� BR label1
6��@SETlabel��label3�����ñ�� label3
7��@BR��label4����� BR label4
8��@SETlabel��label2�����ñ�� label2
*/
int for_stat() {
	int es = 0, label1, label2, label3, label4;
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "(")) return(es = 5);	//��������
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	fprintf(fout, "	POP\n");                 //�����ջָ��
	if (strcmp(token, ";")) return(es *= 4);//�ٷֺ�
	label1 = labelp++;
	fprintf(fout, "LABEL%d:\n", label1);       //���� label1���
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	label2 = labelp++;
	fprintf(fout, " 	BRF LABEL%d\n", label2); //���������ת��ָ��
	label3 = labelp++;
	fprintf(fout, "	    BR LABEL%d\n", label3);   //���������ת��ָ��
	if (strcmp(token, ";"))  return(es = 4);         //�ٷֺ�
	label4 = labelp++;
	fprintf(fout, "LABEL%d:\n", label4);         //����label4���
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	fprintf(fout, "	POP\n");                //�������ָ��
	fprintf(fout, "	BR LABEL%d\n", label1);     //���������ת��ָ��
	if (strcmp(token, ")")) return(es = 6);        //��������
	fprintf(fout, "LABEL%d:\n", label3);          //����label3���
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = statement();
	if (es > 0) return(es);
	fprintf(fout, "	BR LABEL%d\n", label4);     //���������ת��ָ��
	fprintf(fout, "LABEL%d:\n", label2);       //����label2���
	return(es);

}

//<write_stat>::=write<expression>;
//<write_stat>::=write<expression>@OUT;
//�������ͣ�
//@OUT�����OUT
int write_stat() {
	int es = 0;
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	es = expression();
	if (es > 0) return(es);
	if (strcmp(token, ";")) return(es = 4);	//�ٷֺ�
	fprintf(fout, "	OUT\n");	            //���0utָ��
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	return(es);
}

//<read_stat>::==read ID;
//<read_stat>::read ID
//<read_stat>::read ID��n@LOOK��n��d@IN@STO��d@POP;
//�������ͣ�
//@LOOK��n��d������ű�n������������ַd����û�У������δ����
//@IN:��� IN
//@STO��d�����ָ�����STO d
int read_stat() {
	int es = 0, address;
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, "ID")) return(es = 3);    //�ٱ�ʶ��
	es = lookup(token1, &address);
	if (es > 0) return(es);
	////
	initchange(token1);	//��ֵ
	////
	fprintf(fout, "	IN\n");               //����ָ��
	fprintf(fout, "	STO %d\n", address);    //���STOָ��
	fprintf(fout, "	POP\n");
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s %s\n", token, token1);
	if (strcmp(token, ";")) return(es = 4);    //�ٷֺ�
	fscanf(fp, "%s %s\n", &token, &token1);
	printf("%s%s\n", token, token1);
	return(es);
}


//<compound_stat> ::= {<statement_list>}
//������亯��
int compound_stat() {
	int es = 0;
	fscanf(fp, "%s %s\n", &token, &token1); //������亯��
	printf("%s %s\n", token, token1);
	es = statement_list();
	if (es > 0) return (es);
	//ERROR
	if (strcmp(token, "}")) return(es = 2);	//���Ҵ�����
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
	fprintf(fout, "	POP\n");       //���POPָ��
	if (strcmp(token, ";") == 0) {
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s %s\n", token, token1);
		return(es);
	}
	else {
		es = 4;
		return(es);                //�ٷֺ�
	}
}

//<expression>::=ID��n@LOOK��n��d@ASSIGN=<bool_expr>@STO��d@POP|<bool_expr>
int expression() {
	int es = 0, fileadd;
	char token2[20], token3[40], token4[40];
	if (strcmp(token, "ID") == 0) {
		//////
		strcpy(token4, token1);//����ԭID�������ڸ���ֵ
		//////
		fileadd = ftell(fp);	//@ASSIGN��ס��ǰ�ļ�λ��
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
			initchange(token4);//��ֵ���ʼֵ״̬��Ϊ1��˵���Ѹ���ֵ
			////////
			fprintf(fout, "   STO %d\n", address);

			printf("\t���ű�\n");
			printf("\t����\t��ַ\t��ֵ\n");
			for (int i = 0; i < vartablep; i++)
				printf("\t%s\t%d\t%d\n", vartable[i].name, vartable[i].address, vartable[i].initvalue);

		}
		else {
			fseek(fp, fileadd, 0);	//�ļ�ָ��ص�֮ǰ�ı�ʶ��
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
		strcpy(token2, token);	//���������
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
//<factor>::=(<expressioin>)|ID��n@LOOK��n��d@LOAD��d|NUM��i@LOADI��i
int factor() {
	int es = 0;
	if (strcmp(token, "(") == 0) {
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s %s\n", token, token1);
		es = expression();
		if (es > 0) return(es);
		if (strcmp(token, ")")) return(es = 6);	//��������
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s %s\n", token, token1);
	}
	else {
		if (strcmp(token, "ID") == 0) {
			int address;
			es = lookup(token1, &address);	//�˷��ű�,��ȡ������ַ
			if (es > 0) return(es);	        //����û����

			///////
			es = initcheck(token1);			//�ж��Ƿ񸳳�ֵ
			//////
			if (es > 0) return(es);			//����δ����ֵ

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
			es = 7;	//ȱ�ٲ�����
			return(es);
		}
	}

	return(es);
}