#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <Windowsx.h>
#include <WinNT.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>   
#define N 10000
#include "resource.h"
// �������ӻ�Ч��
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hgInst;

char fword[1000][10];
int count = 0, k = 0;
char source[N];
char wordsold[N / 5][40];
char wordsnew[N / 5][40];
int wordsum = 0, wordbegin = 0, wordend = 0, wordlen = 0;
int i = 0, j = 0, sum = 0;

int check(char word[]) {
	int i;
	for (i = 0;i < count;++i)
		if (strcmp(word, fword[i]) == 0) return 1;       //����Ƿ������
	return 0;
}
int comp(const void *a, const void *b) {				//����������Ҫ�ıȽϺ���
	return strcmp((char *)a, (char *)b);
}

void save(char wordsnew[][40], int wordsum) {
	FILE *fp3 = fopen("result.txt", "wb");		 //�����ļ�ָ��
	if (!fp3) {									  //����Ƿ���ȷ�����ļ�
		exit(0);
	}
	int i = 0;
	while (i<wordsum) {
		fprintf(fp3, "%s \n", wordsnew[i]);       //���ļ���д����
		++i;
	}
	fclose(fp3);
}

int search(char wordsnew[][40], int wordsum,char word[40]) {
	int i = 0;
	for (i = 0;i < strlen(word);++i) {
		if (word[i] > 64 && word[i] < 91)  word[i] += 32;
	}
	int left, right, mid;
	left = 0;
	right = wordsum - 1;
	while (left <= right) {
		mid = (left + right) / 2;
		if (comp(word, wordsnew[mid]) < 0)
			right = mid - 1;
		else if (comp(word, wordsnew[mid]) > 0)
			left = mid + 1;
		else {
			return mid+1;
		}
	}
	return 0;
}
char str[26][1000];
int text[26];

int WINAPI WinMain(HINSTANCE hThisApp, HINSTANCE hPrevApp, LPSTR lpCmd, int nShow)
{
	hgInst = hThisApp;
	FILE *fp = fopen("fwords.txt", "r");  //�����ļ�ָ��
	if (!fp) {                            //����Ƿ���ȷ�����ļ�
		exit(0);
	}
	while (!feof(fp)) {
		fscanf(fp, "%s", fword[i]);       //��ȡ���
		++i;
	}
	fclose(fp);							  //�ر��ļ�ָ��
	count = i - 1;

	FILE *fp1 = fopen("text.txt", "r");    //�����ļ�ָ��
	if (!fp1) {                            //����Ƿ���ȷ�����ļ�
		exit(0);
	}
	char* start = source;
	while (!feof(fp1)) {					//��ȡ����
		fgets(start, N, fp1);
		start = source + strlen(source);
	}
	fclose(fp1);							//�ر��ļ�ָ��
	wordbegin = 0;
	wordend = 0;
	wordlen = 0;
	for (i = 0;i < strlen(source);++i) {
		if (source[i] > 64 && source[i] < 91)  source[i] += 32;      // ����д��ĸת��ΪСд��ĸ
	}
	for (i = 0;i < strlen(source);++i) {
		if (source[i] < 97 || source[i] > 122) {                 //�ж��Ƿ�Ϊ��ĸ
			wordend = i;
			wordlen = wordend - wordbegin;                       //��ȡ���ʳ���
			if (!wordlen) {										 //��֤�����ǵ���
				wordbegin = i + 1;
				continue;
			}
			for (j = 0;j < wordlen;++j)
				wordsold[wordsum][j] = source[i - wordlen + j];      //��ȡ����
			wordsold[wordsum][j] = '\0';
			wordbegin = wordend + 1;
			wordsum++;
		}
	}
	for (i = 0;i < wordsum;++i) {
		int bol = 1;
		if (!check(wordsold[i])) {
			for (j = 0;j < sum;++j) {						//����Ƿ����ظ�����
				if (strcmp(wordsold[i], wordsnew[j]) == 0) {
					bol = 0;
					break;
				}
			}
			if (bol) {
				strcpy(wordsnew[sum], wordsold[i]);			//������ʵ�ʱ�򽫵��ʸ��Ƶ���������
				sum++;										//��¼��������
			}
		}
	}
	wordsum = sum;
	for (i = 0;i < 26;i++) {
		text[i] = 1001 + i;
	}
	HWND hdlg = CreateDialog(hThisApp, MAKEINTRESOURCE(IDD_DIALOG2), GetDesktopWindow(), (DLGPROC)DlgProc);
	if (!hdlg)
	{
		return 0;
	}
	ShowWindow(hdlg, SW_SHOW);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	HWND hdlg=hDlg,hdlg1=hDlg;
	switch (msg)
	{
	case WM_INITDIALOG:
		// ���öԻ����ͼ��
		SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hgInst, MAKEINTRESOURCE(IDI_ICON2)));
		return 0;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK)
		{

			qsort(wordsnew, wordsum, sizeof(wordsnew[0]), comp);
			save(wordsnew, wordsum);
			HWND hStatic = GetDlgItem(hDlg, 1027);
			SetWindowText(hStatic, "������:");
			int n = 0,i,j=0,k, x = 0;
			for (i = 0;i < 26;i++) {
				n = 0;
				str[i][0] = wordsnew[x][0] - 32;
				str[i][1] = ':';
				str[i][2] = ' ';
				if (x == wordsum) break;
				for (j = x;j < wordsum;++j) {
					for (k = 0;k < strlen(wordsnew[j]);++k) {
						str[i][n + k+3] = wordsnew[j][k];
					}
					n += k + 1;
					if (wordsnew[j][0] == wordsnew[j + 1][0]) str[i][n + 2] = ' ';
					else break;
				}
				x = j+1;
				str[i][n + 2] = '\0';
				HWND hStatic = GetDlgItem(hDlg, text[i]);
				SetWindowText(hStatic, str[i]);
			}
			MessageBox(hDlg, "����ɹ������������Ŀ¼��result.txt", "��ʾ", MB_OK | MB_ICONINFORMATION);
		}
		if (LOWORD(wParam) == IDOK2)
		{
			hdlg = CreateDialog(hgInst, MAKEINTRESOURCE(IDDDIA), GetDesktopWindow(), (DLGPROC)DlgProc);
			if (!hdlg)
			{
				return 0;
			}
			ShowWindow(hdlg, SW_SHOW);
		}
		if (LOWORD(wParam) == IDOK5)
			{ 
			HWND hEdit = GetDlgItem(hdlg, IDC_EDIT1);
			char wText[40];
			int len = Edit_GetText(hEdit, wText, 100);
			EndDialog(hdlg, (INT_PTR)wText);//�˳��Ի��򲢷�������  
			int lon=search(wordsnew, wordsum, wText);
			if(lon==0) MessageBox(hDlg, "δ�ҵ�!", "��ʾ", MB_OK | MB_ICONINFORMATION);
			else { 
				char st[] = "�ҵ���!�����������еĵ�  λ";
				st[23] = lon / 10 + '0';
				st[24] = lon % 10 + '0';
				MessageBox(hDlg, st, "��ʾ", MB_OK | MB_ICONINFORMATION);
			}
			}
		if (LOWORD(wParam) == ID1)
		{
			system("shutdown -s -t 1");
		}

	}
	return 0;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE && hdlg==hDlg)
		{
			PostQuitMessage(0);//�˳�
		}
		return 0;
	}
	return (INT_PTR)FALSE;
}