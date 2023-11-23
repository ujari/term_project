/*
작업자   : 20233523_최윤호
소속     : 순천향대학교 컴퓨터소프트웨어공학과
작업환경 : desktop
os       : windows 10
IDE      : Visual Studio 2022
컴파일러 : Microsoft® C/C++ Compiler
링커 인터페이스 : Microsoft® Incremental Linker
플랫폼 도구 집합 : Visual Studio 2022 (v143)
*/



/**
* 이 프로그램은 엘리베이터 시뮬레이터로 총 6대의 엘리베이터가 
* 사용자의 요청에 따라 이동하며 생기는 상황을 재현하며 엘리베이터 지정
* 무게 제한 , 점검 등 모든 상황들을 재현하고 있습니다.
*/




#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>
#include<process.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_ELEVATORS 6
#define PASSENGER_KG 1450 //총 탑승자의 최대 무게를 전처리
#define MAN_KG 75 //남자의 몸무게
#define WOMAN_KG 55 //여자의 몸무게
#define MAX_FLOOR 100 //최대층을 100으로 전처리
#define MIN_FLOOR -10 //최하층을 -10 으로 전처리

// 엘리베이터의 상태를 저장하는 구조체 선언
struct elevator
{
    int num[6];        // 각 엘리베이터의 번호를 저장합니다.
    int kg[6];         // 각 엘리베이터의 현재 무게를 저장합니다.
    int floor[6];      // 각 엘리베이터가 현재 위치한 층을 저장합니다.
    int Inspection[6]; // 각 엘리베이터의 점검 상태를 저장합니다. (점검 중이면 1, 아니면 0)
    int direction[6];  // 각 엘리베이터의 이동 방향을 저장합니다. (상승이면 1, 하강이면 -1)
    int status[6];     // 각 엘리베이터의 현재 상태를 저장합니다. (예: 운행 중, 대기 중 등)
    int target[6];     // 각 엘리베이터의 목표 층을 저장합니다.
    int user_floor[6]; // 각 엘리베이터를 호출한 사용자의 층을 저장합니다.
    char button[];     // 각 엘리베이터의 버튼 상태를 저장합니다. (눌린 버튼은 1, 아니면 0)
}ele;// ele라는 이름의 구조체 변수를 선언합니다.

// 사용자가 현재 위치한 층과 목표 층
volatile int user_floor = 0;  // 사용자가 현재 위치한 층을 저장하는 변수를 선언하고 0으로 초기화합니다.
volatile int target_floor = 0;  // 사용자가 이동하려는 목표 층을 저장하는 변수를 선언하고 0으로 초기화합니다.

/** @brief  -10 ~ 100 까지 난수를 생성하는 함수
*  @date 23/11/10
*  @param ramdom : 생성된 난수를 반환
*/
int random() // -10 ~ 100 까지 난수 생성. 엘리베이터가 이동할 층을 무작위로 결정하는데 사용
{
    int random;  // 난수를 저장할 변수를 선언합니다.
    do {
        random = (rand() % 111) - 10;  // -10 ~ 100 사이의 난수를 생성합니다.
    } while (random < -10);  // 생성된 난수가 -10보다 작으면 다시 난수를 생성합니다.
    return random;  // 생성된 난수를 반환합니다.
}

/** @brief 무작위로 성별을 결정하는 함수.
*  @date 23/11/19
*  @return 0 또는 1을 무작위로 반환
*/
int random_gender() {  // 무작위로 성별을 결정하는 함수를 정의합니다.
    return rand() % 2;  // 0 또는 1을 무작위로 반환합니다.
}

/** @brief 성별에 따라 무게를 계산하는 함수.
*  @date 23/11/17
*  @return MAN_KG, WOMAN_KG : 성별이 '0'이면 'MAN_KG'를, 아니면 'WOMAN_KG'를 반환
*/
int calculate_weight(int gender) {  // 성별에 따라 무게를 계산하는 함수를 정의합니다.
    return (gender == 0) ? MAN_KG : WOMAN_KG;  // 성별이 '0'이면 'MAN_KG'를, 아니면 'WOMAN_KG'를 반환합니다.
}

bool is_valid_password(const char* password) {
    int length = strlen(password);
    if (length < 8)
        return false;

    bool has_digit = false;
    bool has_special = false;

    for (int i = 0; i < length; i++) {
        if (isdigit(password[i]))
            has_digit = true;
        else if (!isalpha(password[i]))
            has_special = true;
    }

    return has_digit && has_special;
}



/** @brief 사용자로부터 이동하려는 층을 입력받고 검증하는 함수
*  @date 23/11/15
*  @return floor : 사용자가 입력한 층을 반환
*/
int user_input()  // 사용자로부터 이동하려는 층을 입력받는 함수를 정의합니다.
{
    int floor;  // 사용자가 입력한 층을 저장할 변수를 선언합니다.
    printf("어느 층으로 이동하시겠습니까? (범위: -10 ~ 100 / 0 제외): ");  // 사용자에게 입력을 요청하는 메시지를 출력합니다.
    scanf("%d", &floor);  // 사용자의 입력을 받아 'floor' 변수에 저장합니다.

    // 사용자가 입력한 층이 -10 ~ 100 사이가 아니면 다시 입력받습니다.
    while ((floor < -10 || floor > 100)||(floor==0))
    {
        printf("잘못 입력하셨습니다. 다시 입력해 주세요 (-10 ~ 100 / 0 제외): ");  // 잘못 입력하였음을 알리는 메시지를 출력합니다.
        scanf("%d", &floor);  // 사용자의 입력을 다시 받아 'floor' 변수에 저장합니다.
    }

    return floor;  // 사용자가 입력한 층을 반환합니다.
}

char password[] = "1234567*";  // 초기 비밀번호를 설정합니다.
int weight[MAX_ELEVATORS] = { 0 };  // 각 엘리베이터의 현재 무게를 저장하는 배열을 선언하고 0으로 초기화합니다.
int adminMode = 0; // 관리자 모드 상태를 저장하는 변수를 선언하고 비활성화 상태(0)로 초기화합니다.
int elevatorStatus[MAX_ELEVATORS] = { 0 }; // 각 엘리베이터의 점검 상태를 저장하는 배열을 선언하고 정상 상태(0)로 초기화합니다.

/** @brief 비밀번호를 변경하는 함수
*  @date 23/11/16
*/
char user_password[21]="1234567*";  // 비밀번호를 저장할 문자열 변수
void change_password() {
    

    while (true) {
        printf("새로운 비밀번호를 입력해주세요 (8자 이상, 특수문자 및 숫자 포함): ");
        scanf("%20s", user_password);

        if (is_valid_password(user_password)) {
            printf("비밀번호가 변경되었습니다.\n");
                break;
        }
        else {
            printf("올바른 형식의 비밀번호를 입력해주세요.\n");
        }
    }
}
bool verify_password(const char* password) {
    // 초기 비밀번호와 사용자가 입력한 비밀번호를 비교하여 인증하는 로직을 구현합니다.
    // 예를 들어, 초기 비밀번호가 "admin123"이라고 가정하고 비교합니다.
    if (strcmp(password,user_password) == 0)
        return true;
    else
        return false;
}
/** @brief 엘리베이터 점검여부를 선택하고 엘리베이터를 점검 모드로 전환하는 함수
*  @date 23/11/16
*/
void select_elevator_for_maintenance() {  // 엘리베이터 점검을 선택하는 함수를 정의합니다.
    int pick = 2;  // 사용자의 선택을 저장하는 변수를 선언하고 초기화합니다.
    int lift = 2;  // 엘리베이터 점검 상태를 저장하는 변수를 선언하고 초기화합니다.

    while (1)  // 무한 루프를 시작합니다.
    {
        printf("돌아가기는 1번을 점검을 진행은 0을 입력하세요 >> >");  // 사용자에게 선택을 요청하는 메시지를 출력합니다.
        scanf("%d", &pick);  // 사용자의 입력을 받아 'pick' 변수에 저장합니다.

        if (pick == 0)  // 사용자가 점검을 선택한 경우
        {
            while ((lift != 0) || (lift != 1))
            {

                printf("점검 해제는 0번 점검 설정은 1번 돌아가기는 2를 눌러주세요 >> ");  // 사용자에게 점검 상태를 선택하도록 요청하는 메시지를 출력합니다.
                scanf("%d", &lift);  // 사용자의 입력을 받아 'lift' 변수에 저장합니다.

                if (lift == 1)  // 사용자가 점검 설정을 선택한 경우
                {
                    int elevator;  // 점검할 엘리베이터 번호를 저장하는 변수를 선언합니다.
                    printf("점검할 엘리베이터를 선택해주세요 (1-6): ");  // 사용자에게 점검할 엘리베이터 번호를 입력하도록 요청하는 메시지를 출력합니다.
                    scanf("%d", &elevator);  // 사용자의 입력을 받아 'elevator' 변수에 저장합니다.

                    if (elevator >= 1 && elevator <= MAX_ELEVATORS) {  // 사용자가 올바른 엘리베이터 번호를 입력한 경우
                        ele.status[elevator - 1] = 1;  // 해당 엘리베이터의 상태를 점검 중(1)으로 변경합니다.
                        printf("%d번 엘리베이터가 점검 모드로 전환되었습니다.\n", elevator);  // 점검 모드 전환 메시지를 출력합니다.
                    }
                    else {  // 사용자가 잘못된 엘리베이터 번호를 입력한 경우
                        printf("잘못된 엘리베이터 번호입니다.\n");  // 오류 메시지를 출력합니다.
                    }
                }
                else if (lift == 0) {  // 사용자가 점검 해제를 선택한 경우
                    int elevator;  // 점검을 해제할 엘리베이터 번호를 저장하는 변수를 선언합니다.
                    printf("해제할 엘리베이터를 선택해주세요 (1-6): ");  // 사용자에게 점검을 해제할 엘리베이터 번호를 입력하도록 요청하는 메시지를 출력합니다.
                    scanf("%d", &elevator);  // 사용자의 입력을 받아 'elevator' 변수에 저장합니다.

                    if (elevator >= 1 && elevator <= MAX_ELEVATORS) {  // 사용자가 올바른 엘리베이터 번호를 입력한 경우
                        ele.status[elevator - 1] = 0;  // 해당 엘리베이터의 상태를 정상(0)으로 변경합니다.
                        printf("%d번 엘리베이터가 점검 해제로 전환되었습니다.\n", elevator);  // 점검 해제 메시지를 출력합니다.
                    }
                    else {  // 사용자가 잘못된 엘리베이터 번호를 입력한 경우
                        printf("잘못된 엘리베이터 번호입니다.\n");  // 오류 메시지를 출력합니다.
                    }
                }
                else {
                    break;
                }
            }
        }
        else if (pick == 1) {  // 사용자가 돌아가기를 선택한 경우
            break;  // 무한 루프를 종료합니다.
        }
    }
}

/** @brief 엘리베이터의 점검 상태를 확인하고 처리하는 함수.
*  @date 23/11/17
*  @param int i : 해당 엘리베이터의 인덱스 번호.
*/
void Inspection_confirm(int i) // 엘리베이터의 점검 상태를 확인하고 처리하는 함수를 정의합니다.
{
    if (ele.status[i] == 1) { // 만약 엘리베이터가 점검중인 경우,
        ele.status[i] = 3; // 엘리베이터의 상태를 "점검중"으로 설정합니다.
        ele.user_floor[i] =-10; // 엘리베이터의 현재 층을 임의의 값으로 설정합니다.
        ele.direction[i] = 0; // 엘리베이터의 방향을 "정지"로 설정합니다.
        ele.floor[i] = 0;
        ele.kg[i] = 0;
    }
    else { // 엘리베이터가 점검중이 아닌 경우,
        weight[i] = calculate_weight(random_gender()); // 엘리베이터의 무게를 임의의 값으로 설정합니다.
    }
}

/** @brief 탑승자가 탑승할 때 엘리베이터의 무게를 계산하는 함수.
*  @date 23/11/17
*  @param int i : 해당 엘리베이터의 인덱스 번호.
*/
void kg(int i) // 탑승자가 탑승할 때 엘리베이터의 무게를 계산하는 함수를 정의합니다.
{
    while (ele.kg[i] + weight[i] <= PASSENGER_KG) { // 무게 제한을 초과하지 않을 때까지 탑승자를 추가합니다.
        ele.kg[i] += weight[i]; // 현재 엘리베이터의 무게에 탑승자의 무게를 더합니다.
        weight[i] = calculate_weight(random_gender()); // 다음 탑승자의 무게를 계산합니다.
    }
    Sleep(500); // 0.5초 동안 대기합니다.
    ele.direction[i] = (ele.target[i] > ele.floor[i]) ? 1 : -1; // 목표 층이 현재 층보다 높으면 방향을 1로, 낮으면 -1로 설정합니다.
    ele.user_floor[i] = -11; // 사용자의 현재 층을 임의의 값으로 설정합니다.
}

/** @brief 탑승자가 내릴 때 엘리베이터의 무게를 계산하는 함수.
*  @date 23/11/17
*  @param int i : 해당 엘리베이터의 인덱스 번호.
*/
void get_off(int i) // 탑승자가 내릴 때 엘리베이터의 무게를 계산하는 함수를 정의합니다.
{
    if (ele.floor[i] == ele.target[i]) { // 엘리베이터가 목표 층에 도착한 경우,
        ele.kg[i] -= weight[i]; // 엘리베이터의 무게에서 탑승자의 무게를 뺍니다.
        if (ele.kg[i] < 0) ele.kg[i] = 0; // 무게가 음수가 되지 않도록 합니다.
        ele.direction[i] = 0; // 엘리베이터의 방향을 정지 상태로 설정합니다.
    }
}

/** @brief admin모드에서 엘리베이터의 번호, 층, 무게, 점검여부, 목표 층, 방향을 알려주는 함수
*  @date 23/11/17
*/
void admin_display()  // 관리자 화면을 출력하는 함수를 정의합니다.
{
   
            system("cls");  // 화면을 지웁니다.
            // 아래는 엘리베이터의 상태를 출력하는 부분입니다.
            printf("\n┌────────────┬────────────┬────────────┬────────────┬────────────┬────────────┐\n");
            printf("  Elevator 1   Elevator 2   Elevator 3   Elevator 4   Elevator 5   Elevator 6  \n");
            printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");
            printf("  Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d \n", ele.floor[0] != 0 ? ele.floor[0] : (ele.direction[0] == -1 ? -1 : 1),
                ele.floor[1] != 0 ? ele.floor[1] : (ele.direction[1] == -1 ? -1 : 1),
                ele.floor[2] != 0 ? ele.floor[2] : (ele.direction[2] == -1 ? -1 : 1),
                ele.floor[3] != 0 ? ele.floor[3] : (ele.direction[3] == -1 ? -1 : 1),
                ele.floor[4] != 0 ? ele.floor[4] : (ele.direction[4] == -1 ? -1 : 1),
                ele.floor[5] != 0 ? ele.floor[5] : (ele.direction[5] == -1 ? -1 : 1)); // 각 엘리베이터의 현재 층을 출력합니다.
            printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");
            printf("  Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);// 각 엘리베이터의 현재 무게를 출력합니다.
            printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");
            for (int i = 0; i < MAX_ELEVATORS; i++) {  // 각 엘리베이터에 대해 반복합니다.
                if (ele.status[i] == 3) {  // 해당 엘리베이터가 점검 중인 경우
                    printf("점검중 ");  // '점검중'이라는 메시지를 출력합니다.
                }
                else {// 해당 엘리베이터가 점검 중이 아닌 경우
                    printf("            ");// 빈 칸을 출력합니다.
                }
            }
            printf("  목표층 :%3d   목표층 :%3d   목표층 :%3d   목표층 :%3d   목표층 :%3d   목표층 :%3d \n", ele.target[0], ele.target[1], ele.target[2], ele.target[3], ele.target[4], ele.target[5]);// 각 엘리베이터의 목표 층을 출력합니다.
            printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");
            printf("  방향 : %c   방향 :%c   방향  :%c   방향 :%c   방향 :%c   방향 :%c \n", ele.button[0], ele.button[1], ele.button[2], ele.button[3], ele.button[4], ele.button[5]);// 각 엘리베이터의 이동 방향을 출력합니다.
            printf("└────────────┴────────────┴────────────┴────────────┴────────────┴────────────┘\n");
            printf("엘리베이터 현황 나가기 >>> enter 클릭");
            fflush(stdin);  // 입력 버퍼를 비웁니다.
            if (_kbhit()) {  // 키보드 입력이 있는지 체크
                if (_getch() == '\r') {  // 사용자가 엔터 키를 누르면 관리자 모드로 돌아갑니다.
                    select_elevator_for_maintenance();  // 관리자 모드 함수를 호출합니다.
                }
            }
            
}

/** @brief admin 모드에서 엘리베이터를 이동해주는 함수
*  @date 23/11/17
*/
void admin_move_elevator()
{
    int weight[6] = { 0 }; // 각 엘리베이터의 탑승자 무게를 저장하는 배열 추가

    for (int i = 0; i < 6; i++)
    {
        ele.floor[i] = random(); // 각 엘리베이터의 초기 위치를 무작위로 설정합니다.
    }

    // 각 엘리베이터의 사용자 현재 위치와 목적지를 설정합니다.
    ele.user_floor[0] = random();//1번 엘리베이터의 유저 위치를 랜덤으로 설정합니다.
    ele.user_floor[1] = random();//2번 엘리베이터의 유저 위치를 랜덤으로 설정합니다.
    ele.user_floor[2] = random();//3번 엘리베이터의 유저 위치를 랜덤으로 설정합니다.
    ele.user_floor[3] = random();//4번 엘리베이터의 유저 위치를 랜덤으로 설정합니다.
    ele.user_floor[4] = random();//5번 엘리베이터의 유저 위치를 랜덤으로 설정합니다.
    ele.user_floor[5] = random();//6번 엘리베이터의 유저 위치를 랜덤으로 설정합니다.

    ele.target[0] = random();//1번 엘리베이터의 타겟 층 위치를 랜덤으로 설정합니다.
    ele.target[1] = random();//2번 엘리베이터의 타겟 층 위치를 랜덤으로 설정합니다.
    ele.target[2] = random();//3번 엘리베이터의 타겟 층 위치를 랜덤으로 설정합니다.
    ele.target[3] = random();//4번 엘리베이터의 타겟 층 위치를 랜덤으로 설정합니다.
    ele.target[4] = random();//5번 엘리베이터의 타겟 층 위치를 랜덤으로 설정합니다.
    ele.target[5] = random();//6번 엘리베이터의 타겟 층 위치를 랜덤으로 설정합니다.

    //1번 엘리베이터의 타겟 층 위치를 랜덤으로 설정합니다.
    ele.direction[0] = (ele.user_floor[0] > ele.floor[0]) ? 1 : -1; // 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    ele.direction[1] = (ele.user_floor[1] > ele.floor[1]) ? 1 : -1; // 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    ele.direction[2] = (ele.user_floor[2] > ele.floor[2]) ? 1 : -1; // 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    ele.direction[3] = (ele.user_floor[3] > ele.floor[3]) ? 1 : -1; // 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    ele.direction[4] = (ele.user_floor[4] > ele.floor[4]) ? 1 : -1; // 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    ele.direction[5] = (ele.user_floor[5] > ele.floor[5]) ? 1 : -1; // 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.

    // 사용자가 있는 층으로 이동
    while (1) {

         // 1번 엘리베이터
        Inspection_confirm(0); // 엘리베이터 점검을 확인합니다.
        ele.floor[0] += ele.direction[0]; // 엘리베이터를 이동시킵니다.
        if (ele.floor[0] == ele.user_floor[0])
        {
            kg(0);// kg 함수를 호출하여 탑승자의 무게를 체크합니다.
        }
        get_off(0);//탑승자의 무게를 제외시킵니다.

        // 2번 엘리베이터
        Inspection_confirm(1);// 엘리베이터 점검을 확인합니다.
        ele.floor[1] += ele.direction[1];// 엘리베이터를 이동시킵니다.
        if (ele.floor[1] == ele.user_floor[1])//엘리베이터가 탑승자의 위치에 도달할 경우
        {
            kg(1);// kg 함수를 호출하여 탑승자의 무게를 체크합니다.
        }
        get_off(1);//탑승자의 무게를 제외시킵니다.

        // 3번 엘리베이터
        Inspection_confirm(2);// 엘리베이터 점검을 확인합니다.
        ele.floor[2] += ele.direction[2];// 엘리베이터를 이동시킵니다.
        if (ele.floor[2] == ele.user_floor[2])//엘리베이터가 탑승자의 위치에 도달할 경우
        {
            kg(2);// kg 함수를 호출하여 탑승자의 무게를 체크합니다.
        }
        get_off(2);//탑승자의 무게를 제외시킵니다.

        // 4번 엘리베이터
        Inspection_confirm(3);// 엘리베이터 점검을 확인합니다.
        ele.floor[3] += ele.direction[3];// 엘리베이터를 이동시킵니다.
        if (ele.floor[3] == ele.user_floor[3])//엘리베이터가 탑승자의 위치에 도달할 경우
        {
            kg(3);// kg 함수를 호출하여 탑승자의 무게를 체크합니다.
        }
        get_off(3);//탑승자의 무게를 제외시킵니다.

        // 5번 엘리베이터
        Inspection_confirm(4);// 엘리베이터 점검을 확인합니다.
        ele.floor[4] += ele.direction[4];// 엘리베이터를 이동시킵니다.
        if (ele.floor[4] == ele.user_floor[4])//엘리베이터가 탑승자의 위치에 도달할 경우
        {
            kg(4);// kg 함수를 호출하여 탑승자의 무게를 체크합니다.
        }
        get_off(4);//탑승자의 무게를 제외시킵니다.

        // 6번 엘리베이터
        Inspection_confirm(5);// 엘리베이터 점검을 확인합니다.
        ele.floor[5] += ele.direction[5];// 엘리베이터를 이동시킵니다.
        if (ele.floor[5] == ele.user_floor[5])//엘리베이터가 탑승자의 위치에 도달할 경우
        {
            kg(5);// kg 함수를 호출하여 탑승자의 무게를 체크합니다.
        }
        get_off(5);//탑승자의 무게를 제외시킵니다.

        Sleep(500);// 0.5초 동안 프로그램을 정지합니다.
        system("cls");// 콘솔 화면을 지웁니다.
        admin_display();// admin_display 함수를 호출하여 엘리베이터의 상태를 출력합니다.

        // 모든 엘리베이터가 움직이지 않았을 경우 무한 반복문을 종료합니다.
        if ((ele.direction[0] == 0) && (ele.direction[1] == 0) && (ele.direction[2] == 0) && (ele.direction[3] == 0) && (ele.direction[5] == 0) && (ele.direction[5] == 0))
        {
            break;
        }
    }
}

/**@brief 관리자 모드를 실행할지 물어보고 어떤 기능을 사용할지 물어봄 
*  @date 23/11/17
*/
void adminmode() {  // 관리자 모드를 실행하는 함수를 정의합니다.
    int e = 2;  // 사용자의 선택을 저장하는 변수를 선언하고 초기화합니다.
    printf("프로그램 종료를 원하시면 0을, 계속하려면 1을, 관리자 모드 진입은 2를 입력해주세요: ");  // 사용자에게 입력을 요청하는 메시지를 출력합니다.
    scanf("%d", &e);  // 사용자의 입력을 받아 변수에 저장합니다.

    while (e != 0 && e != 1 && e != 2) {  // 사용자의 입력이 0, 1, 2 중 하나가 아닌 경우에 반복문을 실행합니다.
        printf("잘못 입력하셨습니다. 다시 입력해 주세요 (0, 1 또는 2): ");  // 잘못된 입력임을 알리는 메시지를 출력하고, 다시 입력을 요청하는 메시지를 출력합니다.
        scanf("%d", &e);  // 사용자의 새로운 입력을 받습니다.
    }
    if (e == 0) {  // 사용자의 입력이 0인 경우,
        printf("프로그램을 종료합니다.\n");  // 프로그램 종료 메시지를 출력하고,
        exit(0);  // 프로그램을 종료합니다.
    }
    else if (e == 2) {  // 사용자의 입력이 2인 경우,
        char input_password[21];  // 사용자의 비밀번호 입력을 저장하는 변수를 선언합니다.
        printf("3번 일치하지않으면 사용자 모드로 돌아갑니다.\n");  // 비밀번호 오류 횟수 제한을 알리는 메시지를 출력합니다.
        printf("비밀번호를 입력해주세요: ");  // 비밀번호 입력을 요청하는 메시지를 출력합니다.
        scanf("%20s", &input_password);  // 사용자의 입력을 받아 변수에 저장합니다.
        int i = 0;  // 카운터 변수를 선언하고 초기화합니다.

        if (verify_password(input_password)) {  // 사용자의 입력이 관리자 비밀번호와 일치하는 경우,
            printf("관리자 모드에 진입합니다.\n");  // 관리자 모드 진입 메시지를 출력하고,
            adminMode = 1;  // 관리자 모드를 활성화합니다.

            while (adminMode) {  // 관리자 모드가 활성화된 동안에 반복문을 실행합니다.
                printf("엘리베이터 점검은 1을, 비밀번호 변경은 2를, 엘리베이터 현황판은 3 ,관리자 모드 종료는 4을 입력해주세요: ");  // 사용자에게 입력을 요청하는 메시지를 출력합니다.
                scanf("%d", &e);  // 사용자의 입력을 받아 변수에 저장합니다.

                if (e == 1) {  // 사용자의 입력이 1인 경우,
                    select_elevator_for_maintenance();  // 엘리베이터 점검 함수를 호출합니다.
                }
                else if (e == 2) {  // 사용자의 입력이 2인 경우,
                    change_password();  // 비밀번호 변경 함수를 호출합니다.
                }
                else if (e == 3) {  // 사용자의 입력이 3인 경우,
                    admin_move_elevator();  // 엘리베이터 현황 출력 함수를 호출합니다.
                }
                else if (e == 4) {  // 사용자의 입력이 4인 경우,
                    printf("관리자 모드를 종료합니다.\n");  // 관리자 모드 종료 메시지를 출력하고,
                    adminMode = 0;  // 관리자 모드를 비활성화합니다.
                }
                else {  // 사용자의 입력이 1, 2, 3, 4 중 하나가 아닌 경우,
                    printf("잘못된 입력입니다.\n");  // 잘못된 입력임을 알리는 메시지를 출력합니다.
                }
            }
        }
        else {  // 사용자의 입력이 관리자 비밀번호와 일치하지 않는 경우,
            Sleep(1000);  // 1초 동안 대기하고,
            adminmode();  // 관리자 모드 함수를 다시 호출합니다.
        }
    }
}

/**@brief사용자의 성별을 물어보고 입력받아 그에 따른 문구를 출력하고 체중을 반환하는 함수
*  @date 23/11/14
*  @return int kg : 저장된 사용자의 무게.
*/
int user_sex() {  // 사용자의 성별에 따른 평균 체중을 반환하는 함수를 정의합니다.
    int sex;  // 성별을 저장할 변수를 선언합니다.
    printf("남자면 0 여자면 1을 눌러주세요.");  // 사용자에게 입력을 요청하는 메시지를 출력합니다.
    scanf("%d", &sex);  // 사용자의 입력을 받아 변수에 저장합니다.

    do {  // 반복문을 시작합니다.
        if (sex == 0) {  // 사용자의 입력이 0인 경우,
            break;  // 반복문을 종료합니다.
        }
        else if (sex == 1) {  // 사용자의 입력이 1인 경우,
            break;  // 반복문을 종료합니다.
        }
        else {  // 사용자의 입력이 0도 1도 아닌 경우,
            printf("잘못된 입력입니다 다시 입력해주세요.\n");  // 잘못된 입력임을 알리는 메시지를 출력하고,
            printf("남자면 0 여자면 1을 눌러주세요.");  // 다시 입력을 요청하는 메시지를 출력합니다.
            scanf("%d", &sex);  // 사용자의 새로운 입력을 받습니다.
        }
    } while (sex != 1 && sex != 0);  // 사용자의 입력이 1이 아니고, 0도 아닌 경우에 반복문을 계속 실행합니다.

    int kg;  // 체중을 저장할 변수를 선언합니다.
    if (sex == 0) {  // 사용자의 입력이 0인 경우,
        kg = 75;  // 체중을 75로 설정합니다.
    }
    else {  // 사용자의 입력이 1인 경우,
        kg = 55;  // 체중을 55로 설정합니다.
    }

    return kg;  // 설정된 체중을 반환합니다.
}

/** @brief 사용자가 가장 가까이 있고 이동방향이 같은  엘리베이터를 선택하고 그에따른 엘리베이터를 사용자에게 알려주는 함수
*  @date 2023/11/15
*  @return int closest : 사용자와 가장 가까운 엘리베이터의 인덱스 번호
*  @param int current_floor, int target_floor : 사용자의 현재 층, 사용자의 목표 층
*/
int elevator_selection(int current_floor, int target_floor) { // 사용자가 가장 가까이 있는 엘리베이터를 선택하는 함수를 정의합니다.
    int closest = 0; // 가장 가까운 엘리베이터의 인덱스를 저장하는 변수를 선언하고 초기화합니다.
    int min_dist = MAX_FLOOR; // 가장 가까운 엘리베이터까지의 거리를 저장하는 변수를 선언하고 초기화합니다.
    int direction = (target_floor > current_floor) ? 1 : -1; // 사용자가 올라가려면 1, 내려가려면 -1로 방향을 설정합니다.

    for (int i = 0; i < 6; i++) { // 모든 엘리베이터에 대해 반복합니다.
        if (ele.status[i] != 1 && (ele.direction[i] == 0 || ele.direction[i] == direction)) { // 엘리베이터가 사용 가능하고, 사용자와 같은 방향으로 가는 경우에만 고려합니다.
            int dist = abs(ele.floor[i] - current_floor); // 현재 층과 엘리베이터의 거리를 계산합니다.
            if (dist < min_dist) { // 만약 이 거리가 이전까지의 최소 거리보다 작은 경우,
                min_dist = dist; // 최소 거리를 업데이트하고,
                closest = i; // 가장 가까운 엘리베이터의 인덱스를 업데이트합니다.
            }
        }
    }

    if (closest != -1) { // 가장 가까운 엘리베이터가 존재하는 경우,
        printf("이동 방향이 같고 가장 가까운 엘리베이터는 %d번 입니다.\n", closest + 1); // 사용자에게 가장 가까운 엘리베이터 번호를 알립니다.
    }
    else { // 가장 가까운 엘리베이터가 존재하지 않는 경우,
        printf("선별할 수 있는 엘리베이터가 없습니다. 첫 번째 엘리베이터가 움직입니다.\n"); // 사용자에게 알립니다.
    }

    return closest; // 가장 가까운 엘리베이터의 인덱스를 반환합니다.
}

/** @brief 사용자의 현재위치를 입력받고 사용자의 현재 위치를 반환하는 함수
*  @date 2023/11/17
*  @return 사용자의 현재 위치를 반환
*/
int user_location() { // 사용자의 현재 위치를 입력받는 함수를 정의합니다.
    int a; // 사용자의 현재 위치를 저장할 변수를 선언합니다.
    printf("어느 층에 계신가요? (-10 ~ 100 / 0 제외 )\n"); // 사용자에게 현재 위치를 입력받습니다.
    scanf("%d", &a); // 사용자의 입력을 변수에 저장합니다.
    while ((a < -10 || a > 100)||(a==0))
    {
        printf("잘못 입력하셨습니다. 다시 입력해 주세요 (-10 ~ 100 / 0 제외 ): ");  // 잘못 입력하였음을 알리는 메시지를 출력합니다.
        scanf("%d", &a);  // 사용자의 입력을 다시 받아 'floor' 변수에 저장합니다.
    }

    return a; // 사용자의 현재 위치를 반환합니다.
}


/** @brief 엘리베이터의 번호, 층, 무게 를 출력해주는 함수
*  @date 2023_11_10
*/
void display_elevator_status() // 엘리베이터의 상태를 출력하는 함수를 정의합니다.
{

    printf("\n┌────────────┬────────────┬────────────┬────────────┬────────────┬────────────┐\n");// 상태판의 상단 테두리를 출력합니다.
    printf("  Elevator 1   Elevator 2   Elevator 3   Elevator 4   Elevator 5   Elevator 6  \n");
    printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");// 각 엘리베이터의 상태를 구분하는 라인을 출력합니다.
    printf("  Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d \n", ele.floor[0] != 0 ? ele.floor[0] : (ele.direction[0] == -1 ? -1 : 1),
        ele.floor[1] != 0 ? ele.floor[1] : (ele.direction[1] == -1 ? -1 : 1),
        ele.floor[2] != 0 ? ele.floor[2] : (ele.direction[2] == -1 ? -1 : 1),
        ele.floor[3] != 0 ? ele.floor[3] : (ele.direction[3] == -1 ? -1 : 1),
        ele.floor[4] != 0 ? ele.floor[4] : (ele.direction[4] == -1 ? -1 : 1),
        ele.floor[5] != 0 ? ele.floor[5] : (ele.direction[5] == -1 ? -1 : 1));// 각 엘리베이터의 현재 층을 출력합니다.
    printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");// 각 엘리베이터의 상태를 구분하는 라인을 출력합니다.
    printf("  무게 : %3d  무게 : %3d    무게 : %3d   무게 : %3d   무게 : %3d   무게 : %3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);
    printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");// 각 엘리베이터의 상태를 구분하는 라인을 출력합니다.
    printf(" ");
    printf(" ");
    for (int i = 0; i < MAX_ELEVATORS; i++) { // 모든 엘리베이터에 대해 반복합니다.
        if (ele.status[i] == 3) { // 만약 엘리베이터가 점검중인 경우,
            printf("  점검중 "); // "점검중"이라고 출력합니다.
        }
        else { // 점검중이 아닌 경우,
            printf("            "); // 공백을 출력합니다.
        }
        if (i == MAX_ELEVATORS - 1) { // 마지막 엘리베이터인 경우,
            printf("\n");// 새로운 줄로 넘어갑니다.
        }
    }
    printf("└────────────┴────────────┴────────────┴────────────┴────────────┴────────────┘\n");// 상태판의 하단 테두리를 출력합니다.

}

/** @brief 엘리베이터 문을 여닫는 경우 그에따른 문구출력과 그에따른 엘리베이터 동작 속도를 조절.
* @date 2023/11/17
*  @param close_door : 문을 여닫는 사용자의 의견을 인자로 가짐
*/
void close(int close_door)
{
    if (close_door) {//닫는 경우
        printf("문을 닫습니다. 1초 후 출발합니다.\n");//문을 닫는다는 문구를 출력.
        Sleep(1000); // 1초 대기
    }
    else {//닫지 않는 경우.
        printf("문을 닫지 않습니다. 3초 후 출발합니다.\n");//문을 닫지 않는다는 문구를 출력.
        Sleep(3000); // 3초 대기
    }
}

/** @brie엘리베이터를 이동해주며 문 닫힘을 실행하는 함수
*  @date 23/11/19
* @param target_floor1, user_floor1, target_floor2, user_floor2, target_floor3, user_floor3,
         target_floor4, user_floor4, target_floor5, user_floor5, target_floor6, user_floor6
         : 각각의 엘리베이터의 현재 위치와 목표 층
*/
void move_elevator()
{
    int weight[6] = { 0 }; // 각 엘리베이터의 탑승자 무게를 저장하는 배열 추가


    
    // 사용자의 현재 층과 원하는 목적지를 입력받습니다.
    int a = user_location(); // 사용자의 현재 층을 입력받습니다.
    int f = user_input(); // 사용자의 원하는 목적지를 입력받습니다.

    // 각 층의 엘리베이터 층수가 결정된 후에 가장 적합한 엘리베이터를 선택합니다.
    int index = elevator_selection(a, f); // 가장 가까운 엘리베이터의 인덱스를 선택합니다.
    Sleep(1500); // 1.5초 대기합니다.

    for (int i = 0; i < 6; i++)
    {
        if (i == index)
        {
            ele.target[i] = f;
            ele.user_floor[i] = a;
        }
    }
    // 각 엘리베이터의 사용자 현재 위치와 목적지를 설정합니다.

    int close_door;// 문을 닫을지 결정하는 변수입니다.

    ele.direction[0] = (ele.user_floor[0] > ele.floor[0]) ? 1 : -1;// 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    ele.direction[1] = (ele.user_floor[1] > ele.floor[1]) ? 1 : -1;// 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    ele.direction[2] = (ele.user_floor[2] > ele.floor[2]) ? 1 : -1;// 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    ele.direction[3] = (ele.user_floor[3] > ele.floor[3]) ? 1 : -1;// 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    ele.direction[4] = (ele.user_floor[4] > ele.floor[4]) ? 1 : -1;// 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    ele.direction[5] = (ele.user_floor[5] > ele.floor[5]) ? 1 : -1;// 사용자가 있는 층과 엘리베이터의 현재 층을 비교하여 이동 방향을 설정합니다.
    // 사용자가 있는 층으로 이동
    while (1) {
        
        
        //1번 엘리베이터

        Inspection_confirm(0);//엘리베이터의 점검 여부를 확인합니다.
        ele.floor[0] += ele.direction[0];//엘리베이터를 이동시킵니다.
        
        if (ele.floor[0] == ele.user_floor[0])//사용자가 탑승한 엘리베이터일 경우
        {   
            printf("1번 탑승\n");//탑승햇다는 문구 출력
            if (index == 0) {   // 선택된 엘리베이터가 1번일 때
           
                Sleep(500);// 0.5초 동안 프로그램을 정지합니다.
                system("cls");// 콘솔 화면을 지웁니다.
                display_elevator_status();//display_elevator_status 함수를 호출하여 엘리베이터의 상태를 출력.

                printf("탑승하셨습니다\n문을 닫으시겠습니까? (예: 1, 아니오: 0): ");//문을 닫을건지 사용자에게 물어봄.
                scanf("%d", &close_door);//대답을 close_door 변수에 저장.
                close(close_door);//close 함수를 호출.
                ele.kg[0] += user_sex();//탑승자의 무게만큼 증가.
            }
            kg(0);//임의의 탑승자의 무게를 증가.

        }
        get_off(0);
        //2번 엘리베이터
        Inspection_confirm(1);//엘리베이터의 점검 여부를 확인합니다.

        ele.floor[1] += ele.direction[1];//엘리베이터를 이동시킵니다.
        if (ele.floor[1] == ele.user_floor[1])//사용자가 탑승한 엘리베이터일 경우
        {
            printf("2번 탑승\n");//탑승햇다는 문구 출력
            if (index == 1) {   // 선택된 엘리베이터가 1번일 때
            
                Sleep(500);// 0.5초 동안 프로그램을 정지합니다.
                system("cls");// 콘솔 화면을 지웁니다.
                display_elevator_status();//display_elevator_status 함수를 호출하여 엘리베이터의 상태를 출력.
                printf("탑승하셨습니다\n문을 닫으시겠습니까? (예: 1, 아니오: 0): ");//문을 닫을건지 사용자에게 물어봄.
                scanf("%d", &close_door);//대답을 close_door 변수에 저장.
                close(close_door);//close 함수를 호출.
                ele.kg[1] += user_sex();//탑승자의 무게만큼 증가.
            }
            kg(1);//임의의 탑승자의 무게를 증가.
        }

        get_off(1);//탑승자의 무게를 제외시킵니다.

        //3번 엘리베이터
        Inspection_confirm(2);//엘리베이터의 점검 여부를 확인합니다.
     
        ele.floor[2] += ele.direction[2];//엘리베이터를 이동시킵니다.
        if (ele.floor[2] == ele.user_floor[2])//사용자가 탑승한 엘리베이터일 경우
        {
            printf("3번 탑승\n");//탑승햇다는 문구 출력
            if (index + 1 == 3) {   // 선택된 엘리베이터가 1번일 때
            
                Sleep(500);// 0.5초 동안 프로그램을 정지합니다.
                system("cls");// 콘솔 화면을 지웁니다.
                display_elevator_status();//display_elevator_status 함수를 호출하여 엘리베이터의 상태를 출력.

                printf("탑승하셨습니다\n문을 닫으시겠습니까? (예: 1, 아니오: 0): ");//문을 닫을건지 사용자에게 물어봄.
                scanf("%d", &close_door);//대답을 close_door 변수에 저장.
                close(close_door);//close 함수를 호출.
                ele.kg[2] += user_sex();//탑승자의 무게만큼 증가.
            }
            kg(2);//임의의 탑승자의 무게를 증가.
        }
        get_off(2);//탑승자의 무게를 제외시킵니다.

        //4번 엘리베이터
        Inspection_confirm(3);//엘리베이터의 점검 여부를 확인합니다.
    
        ele.floor[3] += ele.direction[3];//엘리베이터를 이동시킵니다.
        if (ele.floor[3] == ele.user_floor[3])//사용자가 탑승한 엘리베이터일 경우
        {
            printf("4번 탑승\n");//탑승햇다는 문구 출력
            if (index  == 3) {   // 선택된 엘리베이터가 1번일 때
    
                Sleep(500);// 0.5초 동안 프로그램을 정지합니다.
                system("cls");// 콘솔 화면을 지웁니다.
                display_elevator_status();//display_elevator_status 함수를 호출하여 엘리베이터의 상태를 출력.

                printf("탑승하셨습니다\n문을 닫으시겠습니까? (예: 1, 아니오: 0): ");//문을 닫을건지 사용자에게 물어봄.
                scanf("%d", &close_door);//대답을 close_door 변수에 저장.
                close(close_door);//close 함수를 호출.
                ele.kg[3] += user_sex();//탑승자의 무게만큼 증가.
            }
            kg(3);//임의의 탑승자의 무게를 증가.
        }
        get_off(3);//탑승자의 무게를 제외시킵니다.

        //5번 엘리베이터
        Inspection_confirm(4);//엘리베이터의 점검 여부를 확인합니다.
      
        ele.floor[4] += ele.direction[4];//엘리베이터를 이동시킵니다.
        if (ele.floor[4] == ele.user_floor[4])//사용자가 탑승한 엘리베이터일 경우
        {

            printf("5번 탑승\n");//탑승햇다는 문구 출력
            if (index == 4) {   // 선택된 엘리베이터가 1번일 때
              
                Sleep(500);// 0.5초 동안 프로그램을 정지합니다.
                system("cls");// 콘솔 화면을 지웁니다.
                display_elevator_status();//display_elevator_status 함수를 호출하여 엘리베이터의 상태를 출력.

                printf("탑승하셨습니다\n문을 닫으시겠습니까? (예: 1, 아니오: 0): ");//문을 닫을건지 사용자에게 물어봄.
                scanf("%d", &close_door);//대답을 close_door 변수에 저장.
                close(close_door);//close 함수를 호출.
                ele.kg[4] += user_sex();//탑승자의 무게만큼 증가.
            }
            kg(4);//임의의 탑승자의 무게를 증가.
        }
        get_off(4);//탑승자의 무게를 제외시킵니다.

        //6번엘리베이터
        Inspection_confirm(5);//엘리베이터의 점검 여부를 확인합니다.
     
        ele.floor[5] += ele.direction[5];//엘리베이터를 이동시킵니다.
        if (ele.floor[5] == ele.user_floor[5])//사용자가 탑승한 엘리베이터일 경우
        {
            printf("6번 탑승\n");//탑승햇다는 문구 출력
            if (index == 5) {   // 선택된 엘리베이터가 1번일 때
            
                Sleep(500);// 0.5초 동안 프로그램을 정지합니다.
                system("cls");// 콘솔 화면을 지웁니다.
                display_elevator_status();//display_elevator_status 함수를 호출하여 엘리베이터의 상태를 출력.

                printf("탑승하셨습니다\n문을 닫으시겠습니까? (예: 1, 아니오: 0): ");//문을 닫을건지 사용자에게 물어봄.
                scanf("%d", &close_door);//대답을 close_door 변수에 저장.
                close(close_door);//close 함수를 호출.
                ele.kg[5] += user_sex();//탑승자의 무게만큼 증가.
            }
            kg(5);//임의의 탑승자의 무게를 증가.
        }
        get_off(5);//탑승자의 무게를 제외시킵니다.
        Sleep(500);// 0.5초 동안 프로그램을 정지합니다.
        system("cls");// 콘솔 화면을 지웁니다.
        display_elevator_status();//display_elevator_status 함수를 호출하여 엘리베이터의 상태를 출력.

    
        // 모든 엘리베이터가 움직이지 않았을 경우 무한 반복문을 종료합니다.
        if ((ele.direction[0] == 0) && (ele.direction[1] == 0) && (ele.direction[2] == 0) && (ele.direction[3] == 0) && (ele.direction[5] == 0) && (ele.direction[5] == 0))
        {
            break;
        }

        if (ele.floor[index] == ele.target[index])
        {
            printf("하차 하셨습니다 다시 탑승하시려면 enter를 눌러주세요.\n");
            if (_kbhit()) {  // 키보드 입력이 있는지 체크
                if (_getch() == '\r') {  // 사용자가 엔터 키를 누르면 관리자 모드로 돌아갑니다.
                    adminmode();//adminmode 함수 호출
                    move_elevator();

                }
            }
        }

    }
}


/** @brief 엘리베이터의 상/하에 따른 변수를 저장.
*  @date 2023/11/17
*  상/하 에 따라 elevator의 button 배열에 저장.
*/
void button()//함수의 이름
{   
    for (int i = 0;i < 6;i++)//엘리베이터의 button배열을 초기화
    {
        ele.button[i] =0;//button[0]~button[5] 모두 0으로 초기화
    }


    for (int i=0 ;i < 6;i++)
    {
        if (ele.direction[i] == 1)//상승중일 경우
        {
            ele.button[i] = "상";// '상' 이라는 문구 저장
        }
        else {//하강중일 경우
            ele.button[i] = "하"; // '하' 라는 문구 저장
        }
    }
}

/** @brief 프로그램을 구동하는 main 함수.
*  @date 23/11/19
*/
int main()
{
    srand(time(NULL));  // 랜덤 시드 설정

    adminmode();//adminmode 함수 호출

    for (int i = 0; i < 6; i++)
    {
        ele.floor[i] = random();
        ele.user_floor[i] = random();
        ele.target[i] = random();
    }

    while (1)//무한반복
    {

        for (int i = 0; i < 6; i++)
        {
            ele.floor[i] = random();// 각 엘리베이터의 초기 위치를 무작위로 설정합니다.
        }

        move_elevator();//move_elevator 함수를 호출
    }
}