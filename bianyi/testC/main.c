#include<stdio.h>
#include<ctype.h>
extern int TESTscan();
extern int TESTparse();
extern int TESTmachine();

char Scanin[300], Scanout[300]; 			//���ڽ�����������ļ���
FILE* fin, fout;							//����ָ����������ļ���ָ��
int main() {
	int es = 0;
	es = TESTscan();						//���ôʷ�����
	if (es > 0)	printf("�ʷ������д�����ֹͣ��\n");
	else printf("�ʷ������ɹ���\n");
	if (es == 0) {
		es = TESTparse();						//�����﷨����
		if (es == 0) {
			printf("�﷨�����ɹ���\n");
			TESTmachine();
		}
		else printf("�﷨��������\n");
	}
	return 0;
}