//박소현님
/*******************************************************************************************************************************
Hashtable Implementation (STsize = 1000)
Programmer : 지현서, 조은혜, 조윤아, 박소현
Date: 03/15/2024

Description:
The input to the program is a file, consisting of identifiers separated by spaces tab characters newlines and punctuation marks.
An identifier is a string of letters and digits, starting with a letter. Case is significant. The program reads in each identifier from
the input file directly into ST(string table) and append it to the previous identifier, terminated by null character. Compute its 
hashcode. The hashcode of an identifier is computed by summing the original values of its characters and then taking the sum 
modulo the size of HT. Look up the identifier in ST starting with HT[hashcode]. If the listhead is nil, simply add a list element, 
the starting index of the identifier in ST. Otherwise search the list for a previous occurrence of the identifier. If not match add 
a new element to the list, pointing to the new identifier. If match, delete the new identifier from ST and print the ST-index of 
the matching identifier. For each identifier encountered, print the identifier and its index in the string table, whether it was entered
or already existed. After the program is finished processing its input, print hash table. If the ST overflows, print the hashtable 
as above and abort by calling the function “exit()”.

Input: 
A file consisting of identifiers separated by spaces, tab characters, newlines and punctuation marks. 
An identifier is a string of letters and digits, starting with a letter.

 Output:
The identifier, its index in the string table and whether entered or present.
Prints error message for illegal identifier(starting with a digit), illegal separator and over string. 
Prints the hash table before terminating. Simply write out hashcode and the list of identifiers 
associated with each hashcode, but not only for non-empty lists. Finally, print out the number 
of characters used up in ST.

Restriction:
If the ST overflows, print the hash table as above, and abort by calling the function “exit()”. 
“exit()” terminates the execution of a program.

Global variables:
ST - Array of string table
HT - Array of list head of hashtable
letters - Set of letters A..Z, a..z, _
digits - Set of digits 0..9
seperators - null , . ; : ? ! \t \n
*****************************************************************************************************************************/
//지현서
//*****************************************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME "testdata.txt"
#define STsize 1000         //String Table 사이즈
#define HTsize 100          //Hash Table 사이즈
#define FALSE 0
#define TRUE 1


//문자는 a...z, A...Z, _
#define isLetter(x) (  ((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z') || ((x) == '_'))
//숫자는 1,2 ... 9
#define isDigit(x) ( (x) >= '0' && (x) <= '9')


//HashTable 들어가는
typedef struct HTentry *HTpointer;

typedef struct HTentry {
int index;              //StringTable에서의 인덱스
HTpointer next;         //다음 identifier 포인터
} HTentry;


//에러 타입 정의
enum errorTypes { noerror, illsp, illid, overst };
typedef enum errorTypes ERRORtypes;


//delimeter 정의
char seperators[] = " .,;:?!\t\n";


//HashTable, StringTable 선언
HTpointer HT[HTsize];
char ST[STsize];



int nextid = 0;     //현재 identifier
int nextfree = 0;   // 비어있는 다음 StringTable 인덱스
                    //이 다음부터 nextid 인덱스임.
int hashcode;       // 현재 identifier의 hash code 값
int sameid;         // 해당 identifier의 첫번째 hashtable 인덱스(중복 대비)

int found;          //for the previous occurrence of an identifier
                    //HT 중복하면, 돌아갈 identifier의 인덱스
ERRORtypes err;

FILE *fp;            //hash대상 파일
char input;



//Initialize - input file 열기
void initialize()
{
fp = fopen(FILE_NAME, "r");
input = fgetc( fp );
}


//is Seperator - delimeter(seperator) 구별해내기
//seperator이면 1 리턴
int isSeperator(char c )
{
    int i;
    int sep_len;

    sep_len = strlen(seperators);
    for (i = 0; i < sep_len; i ++) {
        if (c == seperators[i] )
            return 1;
    }
    return 0;
}


//PrintHStable - 해쉬테이블 출력.
//맨 위에 [[ HASH TABLE ]] 출력 후
//hascode 와 이에 해당하는 identifiers 출력(단, 비어있지 않은 경우에만)
//String Table에서 쓰인 character 수 출력
//마지막에 팀원들 이름, 학번 출력
void PrintHStable()
{
    int i,j ;
    HTpointer here;

    printf("\n\n\n\n\n [[ HASH TABLE]] \n\n");


    for (i = 0; i <HTsize; i++)
        if (HT[i] != NULL){
            printf(" HashCode %3d :", i);
            for (here = HT[i]; here!=NULL; here = here-> next) {
                j = here-> index;
                while(ST[j] != '\0' && j < STsize)
                    printf("%c", ST[j++]);
                printf("    ");

            }
            printf("\n");
        }
        printf("\n\n\n <%5d characters are used in the string table> \n", nextfree);
        printf("2176368 지현서, 2176365 조은혜, 2171047 조윤아, 2176143 박소현");
}


//print Heading - HEADING 출력
void PrintHeading()
{
    printf("\n\n");
    printf(" ------------   ------------ \n");
    printf(" Index in ST    Identifier \n");
    printf(" ------------   ------------ \n");
    printf("\n");
}





//*****************************************************************************************************************************
//조은혜님
// 
// PrintError - 에러 메시지 출력 함수
//overst : ST에서의 오버플로우
//해시테이블 출력, abort()함수에 의해 중단
// illid : illegal identifier (올바르지 않은 identifier)
// illsp :illegal seperator (올바르지 않은 seperator)
void PrintError( ERRORtypes err )
{
    switch( err ) {
        case overst :   // ST에서 오버플로우가 발생한 경우
            printf(" ... Error ... OVERFLOW ");
            PrintHStable();
            exit(0);
            break;
        case illsp :    // 올바르지 않은 seperator가 입력된 경우
            printf(" ... Error ... %c is illegal seperator \n ", input);    //입력이 올바르지 않은 seperator임을 출력
            
            break;
        case illid :    // 올바르지 않은 identifier가 입력된 경우
            printf(" ... Error ...");
            while(input != EOF && (isLetter(input) || isDigit(input)) ) {
                printf("%c", input);
                input = fgetc(fp);

            }
            printf(" start with digit \n"); // 숫자로 시작함을 출력
            break;
    }
}


// Skip Seperators - spaces,tabs,newlines, . , ; : ? ! 등의 seperator들을 저장하지 않고 넘어가는 함수
// 만약 올바르지 않은 seperator가 입력된 경우 에러 메시지 출력
void SkipSeperators()
{
    while (input != EOF && !( isLetter(input) || isDigit(input) ) ) {   // 입력된 값이 문자나 숫자가 아닌지 확인
        if (!isSeperator(input) ) {
            err = illsp;
            PrintError(err);
        }
        input = fgetc(fp);
    }
}


//ReadIO - 입력에서 identifier를 읽어 스트링테이블 ST에 저장
// ST(append it to the previous identifier).
// identifier는 문자로 시작하는 문자와 숫자의 문자열
// 첫 글자가 숫자인 경우 에러 메시지 출력 
void ReadID() 
{
    nextid = nextfree ;
    if (isDigit(input)) {
        err = illid;
        PrintError(err);
    }else {
        while(input != EOF && (isLetter(input) || isDigit(input))) {    // 입력된 값이 문자나 숫자인지 확인
            if (nextfree == STsize){    // 오버플로우가 발생한 경우
                err = overst;
                PrintError(err);    // 오버플로우 에러 메시지 출력
            }
            ST[nextfree ++] = input;
            input = fgetc(fp);
        }
    }
}
// ComputeHS - 문자의 ordinal values를 합한 다음 HT 크기로 mod 계산하여 해시 코드를 계산하는 함수
void ComputeHS( int nid, int nfree )
{
    int code, i;
    code = 0;
    for (i = nid;i <nfree -1; i++)
        code += (int)ST[i]; // (int)ST[i]: 해당 문자의 ordinal values
    hashcode = code % HTsize;
}



//***************************************************************************************************************************** 
// 조윤아
// LookupHS - 각 identifier에 대해 해시 테이블에 이전에 삽입됐는지 찾음
// 이전에 삽입됐으면 found flag를 true로 설정, 
// ST의 시작 인덱스를 동일한 아이디로 설정
// 이전에 삽입되지 않았으면 found flag를 false로 설정
void LookupHS( int nid, int hscode )
{
    HTpointer here;
    int i,j;

    found = FALSE;
    if (HT[hscode] != NULL) {
        here = HT[hscode];
        while(here != NULL && found == FALSE){
            found = TRUE;
            i = here-> index;
            j = nid;
            sameid = i;

            while(ST[i] != '\0' && ST[j] != '\0' && found == TRUE){
                if (ST[i] != ST[j])
                    found = FALSE;

                    else{
                    i++;
                    j++;
                    }

            }
            here = here -> next;
        }
    }
}
// ADDHT - 해시 테이블(HT)에 새로운 identifier 삽입
// 만약 HT[hscode]가 null이면 ST identifier의 시작 인덱스를 리스트에 바로 삽입
// HT[hscode]가 null이 아니면 HTpointer가 null이 될 때까지 포인터를 따라가다가 삽입
void ADDHT( int hscode )
{
    HTpointer ptr ;

    ptr = (HTpointer) malloc(sizeof(ptr));
    ptr -> index = nextid ;
    ptr -> next = HT[hscode];
    HT[hscode] = ptr;
}
// main - 파일에서 identifier를 직접 읽어 ST에 삽입
// 해시 코드 계산
// 해시 테이블 HT[hashcode]에서 identifier 검색
// 일치하는 경우 ST에서 identifier를 삭제하고 그 identifier의 ST-index를 출력
// 일치하지 않는 경우 새 identifier를 가리키는 새로운 element를 list에 추가
// identifier와 ST의 index를 출력하고 입력 또는 표시 여부 출력
// 해시 테이블과 ST에서 사용된 문자 수 출력
int main()
{
int i;
PrintHeading();
initialize();

while (input != EOF) {
err = noerror;
SkipSeperators();
ReadID();
if( input != EOF && err != illid ) {
if( nextfree == STsize ) {
    err = overst;
    PrintError( err );
}
ST[nextfree++] = '\0';
ComputeHS( nextid, nextfree );
LookupHS( nextid, hashcode );
if( !found ) {
    printf("%6d     ", nextid);
    for (i = nextid; i < nextfree -1 ; i ++)
        printf("%c", ST[i]);
    printf("        (entered) \n");
    ADDHT(hashcode);
}
else {
    printf("%6d     ", sameid);
    for( i = nextid; i< nextfree -1; i++)
        printf("%c", ST[i]);
    printf("    (alreadyexisted)\n");
    nextfree = nextid;

}
}
}


    if (input == EOF) {
        if (nextfree > nextid){
            //마지막 단어까지
            ST[nextfree++] = '\0';
            ComputeHS(nextid, nextfree);
            LookupHS(nextid, hashcode);
            if( !found ) {
                printf("%6d     ", nextid);
                for (i = nextid; i < nextfree -1 ; i ++)
                    printf("%c", ST[i]);
                    printf("        (entered) \n");
                    ADDHT(hashcode);
            } else {
                printf("%6d     ", sameid);
                for( i = nextid; i< nextfree -1; i++)
                    printf("%c", ST[i]);
                    printf("    (alreadyexisted)\n");
                    nextfree = nextid;
            }
        }
    }
PrintHStable();
}



