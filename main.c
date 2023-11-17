#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>
#include <process.h>

#define MAX_ELEVATORS 6
#define PASSENGER_KG 1450 //총 탑승자의 최대 무게를 전처리
#define MAN_KG 75 //남자의 몸무게
#define WOMAN_KG 55 //여자의 몸무게
#define MAX_FLOOR 100 //최대층을 100으로 전처리
#define MIN_FLOOR -10 //최하층을 -10 으로 전처리


struct elevator
{
    int num[6];
    int kg[6];
    int floor[6];
    int Inspection[6];
    int direction[6];
    int status[6];
    int target[6];
    int top[6];
    int bottom[6];
}ele;


volatile int user_floor = 0;
volatile int target_floor = 0;





int random() //-10 ~ 100 까지 난수 생성, -10층 이하로 내려가지 않음
{
    int random;
    do {
        random = (rand() % 111) - 10;
    } while (random < -10);
    return random;
}

int random_gender() {
    return rand() % 2;
}

// 성별에 따른 무게 계산
int calculate_weight(int gender) {
    return (gender == 0) ? MAN_KG : WOMAN_KG;
}


int user_input() {

    int floor;
    printf("어느 층으로 이동하시겠습니까? (범위: -10 ~ 100): ");
    scanf("%d", &floor);

    // 입력값 검증
    while (floor < -10 || floor > 100)
    {
        printf("잘못 입력하셨습니다. 다시 입력해 주세요 (-10 ~ 100): ");
        scanf("%d", &floor);
    }

    return floor;
}

void admin_display()
{


    while (1) {
        system("cls");
        printf("\n┌────────────┬────────────┬────────────┬────────────┬────────────┬────────────┐\n");
        printf("  Elevator 1   Elevator 2   Elevator 3   Elevator 4   Elevator 5   Elevator 6  \n");
        printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");
        printf("  Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d \n", ele.floor[0], ele.floor[1], ele.floor[2], ele.floor[3], ele.floor[4], ele.floor[5]);
        printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");
        printf("  Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);
        printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");
        printf(" ");
        for (int i = 0; i < MAX_ELEVATORS; i++) {
            if (ele.status[i] == 1) {
                printf("점검중 ");
            }
            else {
                printf("            ");
            }
            if (i == MAX_ELEVATORS - 1) {
                printf("\n");
            }
        }
        printf(" \n");
        printf("  Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.target[0], ele.target[1], ele.target[2], ele.target[3], ele.target[4], ele.target[5]);
        printf("└────────────┴────────────┴────────────┴────────────┴────────────┴────────────┘\n");
        printf("\n엔터 키를 누르면 관리자 모드로 돌아갑니다.\n");
        if (getchar() == '\n') {  // 사용자가 엔터를 눌렀는지 확인
            break;  // 엔터를 누르면 while 루프를 빠져나감
        }
        Sleep(500);  // 500밀리초 동안 대기
    }
    adminmode();  // 관리자 모드로 돌아가기
}

}




int password = 1234;  // 초기 비밀번호 설정
int weight[MAX_ELEVATORS] = { 0 };
int adminMode = 0; // 관리자 모드 상태 (0: 비활성화, 1: 활성화)
int elevatorStatus[MAX_ELEVATORS] = { 0 }; // 각 엘리베이터의 상태 (0: 정상, 1: 점검 중)

void change_password() {  // 비밀번호 변경 함수
    printf("새로운 4자리 비밀번호를 입력해주세요: ");
    scanf("%d", &password);
    printf("비밀번호가 변경되었습니다.\n");
}

void select_elevator_for_maintenance() {
    int pick = 2;
    int lift = 2;
    while (1)
    {
        printf("돌아가기는 1번을 점검을 진행은 0을 입력하세요 >> >");
        scanf("%d", &pick);
        if (pick == 0)
        {
            printf("점검 해제는 0번 점검 설정은 1번을 눌러주세요 >> ");
            scanf("%d", &lift);
            if (lift == 1)
            {
                int elevator;
                printf("점검할 엘리베이터를 선택해주세요 (1-6): ");
                scanf("%d", &elevator);
                if (elevator >= 1 && elevator <= MAX_ELEVATORS) {
                    ele.status[elevator - 1] = 1;
                    printf("%d번 엘리베이터가 점검 모드로 전환되었습니다.\n", elevator);
                }
                else {
                    printf("잘못된 엘리베이터 번호입니다.\n");
                }
            }
            else {
                int elevator;
                printf("해제할 엘리베이터를 선택해주세요 (1-6): ");
                scanf("%d", &elevator);
                if (elevator >= 1 && elevator <= MAX_ELEVATORS) {
                    ele.status[elevator - 1] = 0;
                    printf("%d번 엘리베이터가 점검 해제로 전환되었습니다.\n", elevator);
                }
                else {
                    printf("잘못된 엘리베이터 번호입니다.\n");
                }
            }

        }
        else if (pick == 1) {
            break;
        }
    }
}

void adminmode() {
    int e = 2;
    printf("프로그램 종료를 원하시면 0을, 계속하려면 1을, 관리자 모드 진입은 2를 입력해주세요: ");
    scanf("%d", &e);

    while (e != 0 && e != 1 && e != 2) {
        printf("잘못 입력하셨습니다. 다시 입력해 주세요 (0, 1 또는 2): ");
        scanf("%d", &e);
    }
    if (e == 0) {
        printf("프로그램을 종료합니다.\n");
        exit(0);  // 프로그램 종료
    }
    else if (e == 2) {
        int input_password;
        printf("3번 일치하지않으면 사용자 모드로 돌아갑니다.\n");
        printf("비밀번호를 입력해주세요: ");
        scanf("%d", &input_password);
        int i = 0;

        if (input_password == password) {  // 비밀번호 일치 확인
            printf("관리자 모드에 진입합니다.\n");
            adminMode = 1;

            while (adminMode) {
                printf("엘리베이터 점검은 1을, 비밀번호 변경은 2를, 엘리베이터 현황판은 3 ,관리자 모드 종료는 4을 입력해주세요: ");
                scanf("%d", &e);

                if (e == 1) {
                    select_elevator_for_maintenance();
                }
                else if (e == 2) {
                    change_password();  // 비밀번호 변경
                }
                else if (e == 3) {
                    admin_display();  // 엘리베이터 현황
                }
                else if (e == 4) {
                    printf("관리자 모드를 종료합니다.\n");
                    adminMode = 0;
                }
                else {
                    printf("잘못된 입력입니다.\n");
                }
            }
        }
        else {
            Sleep(1000);
            adminmode();
        }
    }
}
int user_sex()
{
    int sex;  // 성별을 저장할 변수를 선언합니다.
    printf("남자면 0 여자면 1을 눌러주세요.");  // 사용자에게 입력을 요청하는 메시지를 출력합니다.
    scanf("%d", &sex);  // 사용자의 입력을 받아 변수에 저장합니다.
    do {  // 반복문을 시작합니다.
        if (sex == 0)  // 사용자의 입력이 0인 경우,
        {
            break;  // 반복문을 종료합니다.
        }
        else if (sex == 1)  // 사용자의 입력이 1인 경우,
        {
            break;  // 반복문을 종료합니다.
        }
        else  // 사용자의 입력이 0도 1도 아닌 경우,
        {
            printf("잘못된 입력입니다 다시 입력해주세요.\n");  // 잘못된 입력임을 알리는 메시지를 출력하고,
            printf("남자면 0 여자면 1을 눌러주세요.");  // 다시 입력을 요청하는 메시지를 출력합니다.
            scanf("%d", &sex);  // 사용자의 새로운 입력을 받습니다.
        }
    } while (sex != 1 && sex != 0);  // 사용자의 입력이 1이 아니고, 0도 아닌 경우에 반복문을 계속 실행합니다.

    int kg;
    if (sex == 0)
    {
        kg = 75;
    }
    else
    {
        kg = 55;
    }

    return kg;
}

int elevator_selection(int current_floor, int target_floor)
{
    int closest = 0; // 초기 값을 0으로 설정
    int min_dist = MAX_FLOOR;
    int direction = (target_floor > current_floor) ? 1 : -1; // 사용자가 올라가려면 1, 내려가려면 -1

    for (int i = 0; i < 6; i++)
    {
        if (ele.status[i] != 1 && (ele.direction[i] == 0 || ele.direction[i] == direction))
        {
            int dist = abs(ele.floor[i] - current_floor);
            if (dist < min_dist)
            {
                min_dist = dist;
                closest = i;
            }
        }
    }

    if (closest != -1)
    {
        printf("이동 방향이 같고 가장 가까운 엘리베이터는 %d번 입니다.\n", closest + 1);
    }
    else
    {
        printf("선별할 수 있는 엘리베이터가 없습니다. 첫 번째 엘리베이터가 움직입니다.\n");
    }

    return closest;
}


int input()
{
    int a;
    printf("어느 층에 계신가요?\n");
    scanf("%d", &a);

    return a;
}
void display_elevator_status()
{


    printf("\n┌────────────┬────────────┬────────────┬────────────┬────────────┬────────────┐\n");
    printf("  Elevator 1   Elevator 2   Elevator 3   Elevator 4   Elevator 5   Elevator 6  \n");
    printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");
    printf("  Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d \n", ele.floor[0], ele.floor[1], ele.floor[2], ele.floor[3], ele.floor[4], ele.floor[5]);
    printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");
    printf("  Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);
    printf("├────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");
    printf(" ");
    for (int i = 0; i < MAX_ELEVATORS; i++) {
        if (ele.status[i] == 1) {
            printf("Maintenance ");
        }
        else {
            printf("            ");
        }
        if (i == MAX_ELEVATORS - 1) {
            printf("\n");
        }
    }
    printf(" \n");
    printf("└────────────┴────────────┴────────────┴────────────┴────────────┴────────────┘\n");

}


void move_elevator(int target_floor1, int user_floor1, int target_floor2, int user_floor2, int target_floor3, int user_floor3, int target_floor4, int user_floor4, int target_floor5, int user_floor5, int target_floor6, int user_floor6)
{
    int weight[6] = { 0 }; // 각 엘리베이터의 탑승자 무게를 저장하는 배열 추가

    for (int i = 0;i < 6;i++)
    {
        ele.floor[i] = random();
    }

    // 사용자의 현재 층과 원하는 목적지를 입력받습니
    int a = input();
    int f = user_input();

    // 각 층의 엘리베이터 층수가 결정된 후에 가장 적합한 엘리베이터를 선택합니다.
    int index = elevator_selection(a, f);
    Sleep(1500);

    // 그리고 반환값 + 1에 해당하는 변수들만 a와 floor로 설정합니다.
    switch (index + 1) {
    default:
        break;
    case 1:
        target_floor1 = f;
        user_floor1 = a;
        break;
    case 2:
        target_floor2 = f;
        user_floor2 = a;
        break;
    case 3:
        target_floor3 = f;
        user_floor3 = a;
        break;
    case 4:
        target_floor4 = f;
        user_floor4 = a;
        break;
    case 5:
        target_floor5 = f;
        user_floor5 = a;
        break;
    case 6:
        target_floor6 = f;
        user_floor6 = a;
        break;
    }
    target[0] = target_floor1;
    target[1] = target_floor2;
    target[2] = target_floor3;
    target[3] = target_floor4;
    target[4] = target_floor5;
    target[5] = target_floor6;
    int close_door;
    printf("%d번 엘리베이터에 탑승하셨습니다. 문을 닫으시겠습니까? (예: 1, 아니오: 0): ", index + 1);
    scanf("%d", &close_door);

    if (close_door) {
        printf("문을 닫습니다. 1초 후 출발합니다.\n");
        Sleep(1000); // 1초 대기
    }
    else {
        printf("문을 닫지 않습니다. 3초 후 출발합니다.\n");
        Sleep(3000); // 3초 대기
    }
    int direction1, direction2, direction3, direction4, direction5, direction6;
    direction1 = (user_floor1 > ele.floor[0]) ? 1 : -1;
    direction2 = (user_floor2 > ele.floor[1]) ? 1 : -1;
    direction3 = (user_floor3 > ele.floor[2]) ? 1 : -1;
    direction4 = (user_floor4 > ele.floor[3]) ? 1 : -1;
    direction5 = (user_floor5 > ele.floor[4]) ? 1 : -1;
    direction6 = (user_floor6 > ele.floor[5]) ? 1 : -1;
    // 사용자가 있는 층으로 이동
    while (1) {




        //1번 엘리베이터

        if (elevatorStatus[0] == 1)
        {
            ele.status[0] = 3;
            user_floor1 = -11;
            direction1 = 0;
        }
        else
        {
            weight[0] = calculate_weight(random_gender());
        }
        ele.floor[0] += direction1;
        if (ele.floor[0] == user_floor1)
        {
            printf("1번 탑승");
            if (index + 1 == 1) {   // 선택된 엘리베이터가 1번일 때
                printf("문을 닫으시겠습니까? (예: 1, 아니오: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("문을 닫습니다. 1초 후 출발합니다.\n");
                    Sleep(1000); // 1초 대기
                }
                else {
                    printf("문을 닫지 않습니다. 3초 후 출발합니다.\n");
                    Sleep(3000); // 3초 대기
                }
                ele.kg[0] += user_sex();

                if (elevatorStatus[0] == 0)
                {
                    user_floor1 = -11;
                    direction1 = 0;
                }
            }
            while (ele.kg[0] + weight[0] <= PASSENGER_KG) { // 무게 제한을 초과하지 않을 때까지 탑승자를 추가
                ele.kg[0] += weight[0];
                weight[0] = calculate_weight(random_gender()); // 다음 탑승자의 무게를 계산
            }
            Sleep(500);
            direction1 = (target_floor1 > ele.floor[0]) ? 1 : -1;
            user_floor1 = -11;
        }
        if (ele.floor[0] == target_floor1)
        {
            ele.kg[0] -= weight[0]; // 목적지에 도착하면 탑승자를 내림
            if (ele.kg[0] < 0) ele.kg[0] = 0; // 무게가 음수가 되지 않도록 함
            direction1 = 0;
        }
        //2번 엘리베이터
        if (elevatorStatus[1] == 1)
        {
            ele.status[1] = 3;
            user_floor2 = -11;
            direction2 = 0;
        }
        else
        {
            weight[1] = calculate_weight(random_gender());
        }
        ele.floor[1] += direction2;
        if (ele.floor[1] == user_floor2)
        {
            printf("2번 탑승");
            if (index + 1 == 2) {   // 선택된 엘리베이터가 1번일 때
                printf("문을 닫으시겠습니까? (예: 1, 아니오: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("문을 닫습니다. 1초 후 출발합니다.\n");
                    Sleep(1000); // 1초 대기
                }
                else {
                    printf("문을 닫지 않습니다. 3초 후 출발합니다.\n");
                    Sleep(3000); // 3초 대기
                }
                ele.kg[1] += user_sex();
                if (elevatorStatus[1] == 0)
                {
                    user_floor2 = -11;
                    direction2 = 0;
                }
            }
            while (ele.kg[1] + weight[1] <= PASSENGER_KG) { // 무게 제한을 초과하지 않을 때까지 탑승자를 추가
                ele.kg[1] += weight[1];
                weight[1] = calculate_weight(random_gender()); // 다음 탑승자의 무게를 계산
            }
            Sleep(500);
            direction2 = (target_floor2 > ele.floor[1]) ? 1 : -1;
            user_floor2 = -11;
        }
        if (ele.floor[1] == target_floor2)
        {
            ele.kg[1] -= weight[1]; // 목적지에 도착하면 탑승자를 내림
            if (ele.kg[1] < 0) ele.kg[1] = 0; // 무게가 음수가 되지 않도록 함
            direction2 = 0;
        }

        //3번 엘리베이터
        if (ele.status[2] == 1)
        {
            elevatorStatus[2] = 3;
            user_floor3 = -11;
            direction3 = 0;
        }
        else
        {
            weight[2] = calculate_weight(random_gender());
        }
        ele.floor[2] += direction2;
        if (ele.floor[2] == user_floor3)
        {
            if (elevatorStatus[3] == 1)
            {

                elevatorStatus[3] = 3;
                user_floor3 = -11;
                direction3 = 0;
            }

            printf("3번 탑승");
            if (index + 1 == 3) {   // 선택된 엘리베이터가 1번일 때
                printf("문을 닫으시겠습니까? (예: 1, 아니오: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("문을 닫습니다. 1초 후 출발합니다.\n");
                    Sleep(1000); // 1초 대기
                }
                else {
                    printf("문을 닫지 않습니다. 3초 후 출발합니다.\n");
                    Sleep(3000); // 3초 대기
                }
                ele.kg[2] += user_sex();
            }
            while (ele.kg[2] + weight[2] <= PASSENGER_KG) { // 무게 제한을 초과하지 않을 때까지 탑승자를 추가
                ele.kg[2] += weight[2];
                weight[2] = calculate_weight(random_gender()); // 다음 탑승자의 무게를 계산
            }
            Sleep(500);
            direction3 = (target_floor3 > ele.floor[2]) ? 1 : -1;
            user_floor3 = -11;
        }
        if (ele.floor[2] == target_floor3)
        {
            ele.kg[2] -= weight[2]; // 목적지에 도착하면 탑승자를 내림
            if (ele.kg[2] < 0) ele.kg[2] = 0; // 무게가 음수가 되지 않도록 함
            direction3 = 0;
        }

        //4번 엘리베이터
        if (ele.status[4] == 1)
        {
            printf("4번 점검중");
            elevatorStatus[3] = 3;
            user_floor4 = -11;
            direction4 = 0;
        }
        else
        {
            weight[3] = calculate_weight(random_gender());
        }
        ele.floor[3] += direction4;
        if (ele.floor[3] == user_floor4)
        {
            printf("4번 탑승");
            if (index + 1 == 4) {   // 선택된 엘리베이터가 1번일 때
                printf("문을 닫으시겠습니까? (예: 1, 아니오: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("문을 닫습니다. 1초 후 출발합니다.\n");
                    Sleep(1000); // 1초 대기
                }
                else {
                    printf("문을 닫지 않습니다. 3초 후 출발합니다.\n");
                    Sleep(3000); // 3초 대기
                }
                ele.kg[3] += user_sex();
            }
            while (ele.kg[3] + weight[3] <= PASSENGER_KG) { // 무게 제한을 초과하지 않을 때까지 탑승자를 추가
                ele.kg[3] += weight[3];
                weight[3] = calculate_weight(random_gender()); // 다음 탑승자의 무게를 계산
            }
            Sleep(500);
            direction4 = (target_floor4 > ele.floor[3]) ? 1 : -1;
            user_floor4 = -11;
        }
        if (ele.floor[3] == target_floor4)
        {
            ele.kg[3] -= weight[3]; // 목적지에 도착하면 탑승자를 내림
            if (ele.kg[3] < 0) ele.kg[3] = 0; // 무게가 음수가 되지 않도록 함
            direction4 = 0;
        }
        //5번 엘리베이터
        weight[3] = calculate_weight(random_gender());
        if (ele.status[4] == 1)
        {
            printf("5번 점검중");
            elevatorStatus[4] = 3;
            user_floor5 = -11;
            direction5 = 0;
        }
        ele.floor[4] += direction5;

        if (ele.floor[4] == user_floor5)
        {

            printf("5번 탑승");
            if (index + 1 == 5) {   // 선택된 엘리베이터가 1번일 때
                printf("문을 닫으시겠습니까? (예: 1, 아니오: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("문을 닫습니다. 1초 후 출발합니다.\n");
                    Sleep(1000); // 1초 대기
                }
                else {
                    printf("문을 닫지 않습니다. 3초 후 출발합니다.\n");
                    Sleep(3000); // 3초 대기
                }
                ele.kg[4] += user_sex();
            }
            while (ele.kg[4] + weight[4] <= PASSENGER_KG) { // 무게 제한을 초과하지 않을 때까지 탑승자를 추가
                ele.kg[4] += weight[4];
                weight[4] = calculate_weight(random_gender()); // 다음 탑승자의 무게를 계산
            }
            Sleep(500);
            direction5 = (target_floor5 > ele.floor[4]) ? 1 : -1;
            user_floor5 = -11;
        }
        if (ele.floor[4] == target_floor5)
        {
            ele.kg[4] -= weight[4]; // 목적지에 도착하면 탑승자를 내림
            if (ele.kg[4] < 0) ele.kg[4] = 0; // 무게가 음수가 되지 않도록 함
            direction5 = 0;
        }
        //6번엘리베이터
        weight[3] = calculate_weight(random_gender());
        if (ele.status[5] == 1)
        {
            printf("6번 점검중");
            elevatorStatus[5] = 3;
            user_floor6 = -11;
            direction6 = 0;
        }
        ele.floor[5] += direction6;
        if (ele.floor[5] == user_floor6)
        {
            printf("6번 탑승");
            if (index + 1 == 6) {   // 선택된 엘리베이터가 1번일 때
                printf("문을 닫으시겠습니까? (예: 1, 아니오: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("문을 닫습니다. 1초 후 출발합니다.\n");
                    Sleep(1000); // 1초 대기
                }
                else {
                    printf("문을 닫지 않습니다. 3초 후 출발합니다.\n");
                    Sleep(3000); // 3초 대기
                }
                ele.kg[5] += user_sex();
            }
            while (ele.kg[5] + weight[5] <= PASSENGER_KG) { // 무게 제한을 초과하지 않을 때까지 탑승자를 추가
                ele.kg[5] += weight[5];
                weight[5] = calculate_weight(random_gender()); // 다음 탑승자의 무게를 계산
            }
            Sleep(500);
            direction6 = (target_floor6 > ele.floor[5]) ? 1 : -1;
            user_floor6 = -11;
        }
        if (ele.floor[5] == target_floor6)
        {
            ele.kg[5] -= weight[5]; // 목적지에 도착하면 탑승자를 내림
            if (ele.kg[5] < 0) ele.kg[5] = 0; // 무게가 음수가 되지 않도록 함
            direction6 = 0;
        }
        Sleep(500);
        system("cls");
        display_elevator_status();


    }
}




int main()
{
    srand(time(NULL));  // 랜덤 시드 설정


    adminmode();
    for (int i = 0;i < 6;i++) {
        printf("%d\n", ele.status[i]);
    }


    while (1)
    {


        // 먼저 모든 값을 랜덤하게 설정합니다.
        int t1 = random();
        int c1 = random();
        int t2 = random();
        int c2 = random();
        int t3 = random();
        int c3 = random();
        int t4 = random();
        int c4 = random();
        int t5 = random();
        int c5 = random();
        int t6 = random();
        int c6 = random();






        move_elevator(t1, c1, t2, c2, t3, c3, t4, c4, t5, c5, t6, c6);
    }