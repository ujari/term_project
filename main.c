/*
�۾���   : 20233523_����ȣ
�Ҽ�     : ��õ����б� ��ǻ�ͼ���Ʈ������а�
�۾�ȯ�� : desktop
os       : windows 10
IDE      : Visual Studio 2022
�����Ϸ� : Microsoft�� C/C++ Compiler
��Ŀ �������̽� : Microsoft�� Incremental Linker
�÷��� ���� ���� : Visual Studio 2022 (v143)
*/



/**
* �� ���α׷��� ���������� �ùķ����ͷ� �� 6���� ���������Ͱ� 
* ������� ��û�� ���� �̵��ϸ� ����� ��Ȳ�� �����ϸ� ���������� ����
* ���� ���� , ���� �� ��� ��Ȳ���� �����ϰ� �ֽ��ϴ�.
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
#define PASSENGER_KG 1450 //�� ž������ �ִ� ���Ը� ��ó��
#define MAN_KG 75 //������ ������
#define WOMAN_KG 55 //������ ������
#define MAX_FLOOR 100 //�ִ����� 100���� ��ó��
#define MIN_FLOOR -10 //�������� -10 ���� ��ó��

// ������������ ���¸� �����ϴ� ����ü ����
struct elevator
{
    int num[6];        // �� ������������ ��ȣ�� �����մϴ�.
    int kg[6];         // �� ������������ ���� ���Ը� �����մϴ�.
    int floor[6];      // �� ���������Ͱ� ���� ��ġ�� ���� �����մϴ�.
    int Inspection[6]; // �� ������������ ���� ���¸� �����մϴ�. (���� ���̸� 1, �ƴϸ� 0)
    int direction[6];  // �� ������������ �̵� ������ �����մϴ�. (����̸� 1, �ϰ��̸� -1)
    int status[6];     // �� ������������ ���� ���¸� �����մϴ�. (��: ���� ��, ��� �� ��)
    int target[6];     // �� ������������ ��ǥ ���� �����մϴ�.
    int user_floor[6]; // �� ���������͸� ȣ���� ������� ���� �����մϴ�.
    int button[6];     // �� ������������ ��ư ���¸� �����մϴ�. (���� ��ư�� 1, �ƴϸ� 0)
}ele;// ele��� �̸��� ����ü ������ �����մϴ�.

// ����ڰ� ���� ��ġ�� ���� ��ǥ ��
volatile int user_floor = 0;  // ����ڰ� ���� ��ġ�� ���� �����ϴ� ������ �����ϰ� 0���� �ʱ�ȭ�մϴ�.
volatile int target_floor = 0;  // ����ڰ� �̵��Ϸ��� ��ǥ ���� �����ϴ� ������ �����ϰ� 0���� �ʱ�ȭ�մϴ�.

/** @brief  -10 ~ 100 ���� ������ �����ϴ� �Լ�
*  @date 23/11/10
*  @param ramdom : ������ ������ ��ȯ
*/
int random() // -10 ~ 100 ���� ���� ����. ���������Ͱ� �̵��� ���� �������� �����ϴµ� ���
{
    int random;  // ������ ������ ������ �����մϴ�.
    do {
        random = (rand() % 111) - 10;  // -10 ~ 100 ������ ������ �����մϴ�.
    } while (random < -10);  // ������ ������ -10���� ������ �ٽ� ������ �����մϴ�.
    return random;  // ������ ������ ��ȯ�մϴ�.
}

/** @brief �������� ������ �����ϴ� �Լ�.
*  @date 23/11/19
*  @return 0 �Ǵ� 1�� �������� ��ȯ
*/
int random_gender() {  // �������� ������ �����ϴ� �Լ��� �����մϴ�.
    return rand() % 2;  // 0 �Ǵ� 1�� �������� ��ȯ�մϴ�.
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


/** @brief ������ ���� ���Ը� ����ϴ� �Լ�.
*  @date 23/11/17
*  @return MAN_KG, WOMAN_KG : ������ '0'�̸� 'MAN_KG'��, �ƴϸ� 'WOMAN_KG'�� ��ȯ
*/
int calculate_weight(int gender) {  // ������ ���� ���Ը� ����ϴ� �Լ��� �����մϴ�.
    return (gender == 0) ? MAN_KG : WOMAN_KG;  // ������ '0'�̸� 'MAN_KG'��, �ƴϸ� 'WOMAN_KG'�� ��ȯ�մϴ�.
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



/** @brief ����ڷκ��� �̵��Ϸ��� ���� �Է¹ް� �����ϴ� �Լ�
*  @date 23/11/27
*  @return floor : ����ڰ� �Է��� ���� ��ȯ
*/
int user_input() {
    int floor;
    printf("��� ������ �̵��Ͻðڽ��ϱ�? (����: -10 ~ 100 / 0 ����): ");
    scanf("%d", &floor);

    while ((floor < -10 || floor > 100) || (floor == 0)) {
        printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (-10 ~ 100 / 0 ����): ");
        while (getchar() != '\n');  // ������ �Է��� ���ڵ��� ���ۿ��� �����մϴ�.
        scanf("%d", &floor);
    }

    return floor;
}

char password[] = "1234567*";  // �ʱ� ��й�ȣ�� �����մϴ�.
int weight[MAX_ELEVATORS] = { 0 };  // �� ������������ ���� ���Ը� �����ϴ� �迭�� �����ϰ� 0���� �ʱ�ȭ�մϴ�.
int adminMode = 0; // ������ ��� ���¸� �����ϴ� ������ �����ϰ� ��Ȱ��ȭ ����(0)�� �ʱ�ȭ�մϴ�.
int elevatorStatus[MAX_ELEVATORS] = { 0 }; // �� ������������ ���� ���¸� �����ϴ� �迭�� �����ϰ� ���� ����(0)�� �ʱ�ȭ�մϴ�.

/** @brief ��й�ȣ�� �����ϴ� �Լ�
*  @date 23/11/16
*/
char user_password[21]="1234567*";  // ��й�ȣ�� ������ ���ڿ� ����
void change_password() {
    

    while (true) {
        printf("���ο� ��й�ȣ�� �Է����ּ��� (8�� �̻�, Ư������ �� ���� ����): ");
        scanf("%20s", user_password);

        if (is_valid_password(user_password)) {
            printf("��й�ȣ�� ����Ǿ����ϴ�.\n");
                break;
        }
        else {
            printf("�ùٸ� ������ ��й�ȣ�� �Է����ּ���.\n");
        }
    }
}
bool verify_password(const char* password) {
    // �ʱ� ��й�ȣ�� ����ڰ� �Է��� ��й�ȣ�� ���Ͽ� �����ϴ� ������ �����մϴ�.
    // ���� ���, �ʱ� ��й�ȣ�� "admin123"�̶�� �����ϰ� ���մϴ�.
    if (strcmp(password,user_password) == 0)
        return true;
    else
        return false;
}
/** @brief ���������� ���˿��θ� �����ϰ� ���������͸� ���� ���� ��ȯ�ϴ� �Լ�
*  @date 23/11/27
*/

void select_elevator_for_maintenance() {
    int pick = 2;
    int lift = 2;

    while (1) {
        printf("���ư���� 1���� ������ ������ 0�� �Է��ϼ��� >> ");
        if (scanf("%d", &pick) != 1) {
            printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (0 �Ǵ� 1 �Է�): ");
            clear_input_buffer();
            continue;
        }

        if (pick == 0) {
            while (1) {
                printf("���� ������ 0��, ���� ������ 1��, ���ư���� 2���� �Է��ϼ��� >> ");
                if (scanf("%d", &lift) != 1) {
                    printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (0, 1, 2 �Է�): ");
                    clear_input_buffer();
                    continue;
                }

                if (lift == 1) {
                    int elevator;
                    printf("������ ���������͸� �������ּ��� (1-6): ");
                    if (scanf("%d", &elevator) != 1) {
                        printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (1-6 ������ ���� �Է�): ");
                        clear_input_buffer();
                        continue;
                    }

                    if (elevator >= 1 && elevator <= 6) {
                        ele.status[elevator - 1] = 1;
                        printf("%d�� ���������Ͱ� ���� ���� ��ȯ�Ǿ����ϴ�.\n", elevator);
                    }
                    else {
                        printf("�߸��� ���������� ��ȣ�Դϴ�.\n");
                    }
                }
                else if (lift == 0) {
                    int elevator;
                    printf("������ ���������͸� �������ּ��� (1-6): ");
                    if (scanf("%d", &elevator) != 1) {
                        printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (1-6 ������ ���� �Է�): ");
                        clear_input_buffer();
                        continue;
                    }

                    if (elevator >= 1 && elevator <= 6) {
                        ele.status[elevator - 1] = 0;
                        printf("%d�� ���������Ͱ� ���� ������ ��ȯ�Ǿ����ϴ�.\n", elevator);
                    }
                    else {
                        printf("�߸��� ���������� ��ȣ�Դϴ�.\n");
                    }
                }
                else {
                    break;
                }
            }
        }
        else if (pick == 1) {
            break;
        }
        else {
            printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (0 �Ǵ� 1 �Է�): ");
        }
    }
}

/** @brief ������������ ���� ���¸� Ȯ���ϰ� ó���ϴ� �Լ�.
*  @date 23/11/17
*  @param int i : �ش� ������������ �ε��� ��ȣ.
*/
void Inspection_confirm(int i) // ������������ ���� ���¸� Ȯ���ϰ� ó���ϴ� �Լ��� �����մϴ�.
{
    if (ele.status[i] == 1) { // ���� ���������Ͱ� �������� ���,
        ele.status[i] = 3; // ������������ ���¸� "������"���� �����մϴ�.
        ele.user_floor[i] =-10; // ������������ ���� ���� ������ ������ �����մϴ�.
        ele.direction[i] = 0; // ������������ ������ "����"�� �����մϴ�.
        ele.floor[i] = 0;
        ele.kg[i] = 0;
    }
    else { // ���������Ͱ� �������� �ƴ� ���,
        weight[i] = calculate_weight(random_gender()); // ������������ ���Ը� ������ ������ �����մϴ�.
    }
}

/** @brief ž���ڰ� ž���� �� ������������ ���Ը� ����ϴ� �Լ�.
*  @date 23/11/17
*  @param int i : �ش� ������������ �ε��� ��ȣ.
*/
void kg(int i) // ž���ڰ� ž���� �� ������������ ���Ը� ����ϴ� �Լ��� �����մϴ�.
{
    while (ele.kg[i] + weight[i] <= PASSENGER_KG) { // ���� ������ �ʰ����� ���� ������ ž���ڸ� �߰��մϴ�.
        ele.kg[i] += weight[i]; // ���� ������������ ���Կ� ž������ ���Ը� ���մϴ�.
        weight[i] = calculate_weight(random_gender()); // ���� ž������ ���Ը� ����մϴ�.
    }
    Sleep(500); // 0.5�� ���� ����մϴ�.
    ele.direction[i] = (ele.target[i] > ele.floor[i]) ? 1 : -1; // ��ǥ ���� ���� ������ ������ ������ 1��, ������ -1�� �����մϴ�.
    ele.user_floor[i] = -11; // ������� ���� ���� ������ ������ �����մϴ�.
}

/** @brief ž���ڰ� ���� �� ������������ ���Ը� ����ϴ� �Լ�.
*  @date 23/11/17
*  @param int i : �ش� ������������ �ε��� ��ȣ.
*/
void get_off(int i) // ž���ڰ� ���� �� ������������ ���Ը� ����ϴ� �Լ��� �����մϴ�.
{
    if (ele.floor[i] == ele.target[i]) { // ���������Ͱ� ��ǥ ���� ������ ���,
        ele.kg[i] -= weight[i]; // ������������ ���Կ��� ž������ ���Ը� ���ϴ�.
        if (ele.kg[i] < 0) ele.kg[i] = 0; // ���԰� ������ ���� �ʵ��� �մϴ�.
        ele.direction[i] = 0; // ������������ ������ ���� ���·� �����մϴ�.
    }
}

/** @brief admin��忡�� ������������ ��ȣ, ��, ����, ���˿���, ��ǥ ��, ������ �˷��ִ� �Լ�
*  @date 23/11/17
*/
void admin_display()  // ������ ȭ���� ����ϴ� �Լ��� �����մϴ�.
{
   
            system("cls");  // ȭ���� ����ϴ�.
            // �Ʒ��� ������������ ���¸� ����ϴ� �κ��Դϴ�.
            printf("\n��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
            printf("  Elevator 1   Elevator 2   Elevator 3   Elevator 4   Elevator 5   Elevator 6  \n");
            printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
            printf("  Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d \n", ele.floor[0] != 0 ? ele.floor[0] : (ele.direction[0] == -1 ? -1 : 1),
                ele.floor[1] != 0 ? ele.floor[1] : (ele.direction[1] == -1 ? -1 : 1),
                ele.floor[2] != 0 ? ele.floor[2] : (ele.direction[2] == -1 ? -1 : 1),
                ele.floor[3] != 0 ? ele.floor[3] : (ele.direction[3] == -1 ? -1 : 1),
                ele.floor[4] != 0 ? ele.floor[4] : (ele.direction[4] == -1 ? -1 : 1),
                ele.floor[5] != 0 ? ele.floor[5] : (ele.direction[5] == -1 ? -1 : 1)); // �� ������������ ���� ���� ����մϴ�.
            printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
            printf("  Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);// �� ������������ ���� ���Ը� ����մϴ�.
            printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
            for (int i = 0; i < MAX_ELEVATORS; i++) {  // �� ���������Ϳ� ���� �ݺ��մϴ�.
                if (ele.status[i] == 3) {  // �ش� ���������Ͱ� ���� ���� ���
                    printf(" ������\t\t");  // '������'�̶�� �޽����� ����մϴ�.
                }
                else {
                    printf("\t\t");
                }

            }
            printf("\n");
            printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
            printf(" ��ǥ�� :%3d  ��ǥ�� :%3d  ��ǥ�� :%3d  ��ǥ�� :%3d  ��ǥ�� :%3d  ��ǥ�� :%3d \n", ele.target[0], ele.target[1], ele.target[2], ele.target[3], ele.target[4], ele.target[5]);// �� ������������ ��ǥ ���� ����մϴ�.
            printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
            printf("  ���� : %s   ���� :%s     ����  :%s   ���� :%s     ���� :%s     ���� :%s \n",
                ele.direction[0] == 1 ? "��" : (ele.direction[0] == 0 ? "����" : "��"),
                ele.direction[1] == 1 ? "��" : (ele.direction[1] == 0 ? "����" : "��"),
                ele.direction[2] == 1 ? "��" : (ele.direction[2] == 0 ? "����" : "��"),
                ele.direction[3] == 1 ? "��" : (ele.direction[3] == 0 ? "����" : "��"),
                ele.direction[4] == 1 ? "��" : (ele.direction[4] == 0 ? "����" : "��"),
                ele.direction[5] == 1 ? "��" : (ele.direction[5] == 0 ? "����" : "��"));// �� ������������ �̵� ������ ����մϴ�.
            printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
            printf("���������� ��Ȳ ������ >>> enter Ŭ��\n");
            fflush(stdin);  // �Է� ���۸� ���ϴ�.
            if (_kbhit()) {  // Ű���� �Է��� �ִ��� üũ
                if (_getch() == '\r') {  // ����ڰ� ���� Ű�� ������ ������ ���� ���ư��ϴ�.
                    select_elevator_for_maintenance();  // ������ ��� �Լ��� ȣ���մϴ�.
                }
            }
            
}

/** @brief admin ��忡�� ���������͸� �̵����ִ� �Լ�
*  @date 23/11/17
*/
void admin_move_elevator()
{
    int weight[6] = { 0 }; // �� ������������ ž���� ���Ը� �����ϴ� �迭 �߰�

    for (int i = 0; i < 6; i++)
    {
        ele.direction[i] = (ele.user_floor[i] > ele.floor[i]) ? 1 : -1; // ����ڰ� �ִ� ���� ������������ ���� ���� ���Ͽ� �̵� ������ �����մϴ�.

    }

    // ����ڰ� �ִ� ������ �̵�
    while (1) {


        for (int i = 0; i < 6; i++)
        {

            Inspection_confirm(i); // ���������� ������ Ȯ���մϴ�.
            ele.floor[i] += ele.direction[i]; // ���������͸� �̵���ŵ�ϴ�.
            if (ele.floor[i] == ele.user_floor[i])
            {
                kg(i);// kg �Լ��� ȣ���Ͽ� ž������ ���Ը� üũ�մϴ�.
            }
            get_off(i);//ž������ ���Ը� ���ܽ�ŵ�ϴ�.
        }

        Sleep(500);// 0.5�� ���� ���α׷��� �����մϴ�.
        system("cls");// �ܼ� ȭ���� ����ϴ�.
        admin_display();// admin_display �Լ��� ȣ���Ͽ� ������������ ���¸� ����մϴ�.

        for (int i = 0; i < 6; i++)
        {
            if (ele.direction[i] == 0)
            {
                ele.user_floor[i] = random();
                ele.target[i] = random();
            }
        }
    }
}

/**@brief ������ ��带 �������� ����� � ����� ������� ��� 
*  @date 23/11/27
*/
void adminmode() {
    int e = 2;
    printf("���α׷� ���Ḧ ���Ͻø� 0��, ���������� �̿��� 1��, ������ ��� ������ 2�� �Է����ּ���: ");
    scanf("%d", &e);

    while (e != 0 && e != 1 && e != 2) {
        printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (0, 1 �Ǵ� 2): ");
        clear_input_buffer();
        scanf("%d", &e);
    }

    if (e == 0) {
        printf("���α׷��� �����մϴ�.\n");
            exit(0);
    }
    else if (e == 2) {
        char input_password[21];
        printf("3�� ��ġ���� ������ ����� ���� ���ư��ϴ�.\n");
            printf("��й�ȣ�� �Է����ּ���: ");
        clear_input_buffer();
        scanf("%20s", input_password);

        int i = 0;
        int wrong_count = 0;

        while (!verify_password(input_password)) {
            wrong_count++;
            if (wrong_count == 3) {
                printf("��й�ȣ�� ��ġ���� �ʽ��ϴ�. �ٽ� �Է��� �ּ���.\n");
                    adminmode();
                return;
            }

            printf("��й�ȣ�� ��ġ���� �ʽ��ϴ�. �ٽ� �Է��� �ּ���: ");
            clear_input_buffer();
            scanf("%20s", input_password);
        }

        printf("������ ��忡 �����մϴ�.\n");
            int adminMode = 1;

        while (adminMode) {
            printf("���������� ������ 1��, ��й�ȣ ������ 2��, ���������� ��Ȳ���� 3, ������ ��� ����� 4�� �Է����ּ���: ");
            scanf("%d", &e);

            if (e == 1) {
                select_elevator_for_maintenance();
            }
            else if (e == 2) {
                change_password();
            }
            else if (e == 3) {

                for (int i = 0; i < 6; i++)
                {
                    ele.floor[i] = random();
                    ele.user_floor[i] = random();
                    ele.target[i] = random();
                }
                admin_move_elevator();
            }
            else if (e == 4) {
                printf("������ ��带 �����մϴ�.\n");
                    adminMode = 0;
            }
            else {
                printf("�߸��� �Է��Դϴ�.\n");
            }
        }
    }
}

/**@brief������� ������ ����� �Է¹޾� �׿� ���� ������ ����ϰ� ü���� ��ȯ�ϴ� �Լ�
*  @date 23/11/14
*  @return int kg : ����� ������� ����.
*/
int user_sex() {  // ������� ������ ���� ��� ü���� ��ȯ�ϴ� �Լ��� �����մϴ�.
    int sex;  // ������ ������ ������ �����մϴ�.
    printf("���ڸ� 0 ���ڸ� 1�� �����ּ���.");  // ����ڿ��� �Է��� ��û�ϴ� �޽����� ����մϴ�.
    scanf("%d", &sex);  // ������� �Է��� �޾� ������ �����մϴ�.

    while (sex != 1 && sex != 0) {
        printf("�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���.\n");
        printf("���ڸ� 0, ���ڸ� 1�� �Է����ּ���: ");
        while (getchar() != '\n');  // ������ �Է��� ���ڵ��� ���ۿ��� �����մϴ�.
        scanf("%d", &sex);
    }


    int kg;  // ü���� ������ ������ �����մϴ�.
    if (sex == 0) {  // ������� �Է��� 0�� ���,
        kg = 75;  // ü���� 75�� �����մϴ�.
    }
    else {  // ������� �Է��� 1�� ���,
        kg = 55;  // ü���� 55�� �����մϴ�.
    }

    return kg;  // ������ ü���� ��ȯ�մϴ�.
}

/** @brief ����ڰ� ���� ������ �ְ� �̵������� ����  ���������͸� �����ϰ� �׿����� ���������͸� ����ڿ��� �˷��ִ� �Լ�
*  @date 2023/11/15
*  @return int closest : ����ڿ� ���� ����� ������������ �ε��� ��ȣ
*  @param int current_floor, int target_floor : ������� ���� ��, ������� ��ǥ ��
*/
int elevator_selection(int current_floor, int target_floor) { // ����ڰ� ���� ������ �ִ� ���������͸� �����ϴ� �Լ��� �����մϴ�.
    int closest = 0; // ���� ����� ������������ �ε����� �����ϴ� ������ �����ϰ� �ʱ�ȭ�մϴ�.
    int min_dist = MAX_FLOOR; // ���� ����� ���������ͱ����� �Ÿ��� �����ϴ� ������ �����ϰ� �ʱ�ȭ�մϴ�.
    int direction = (target_floor > current_floor) ? 1 : -1; // ����ڰ� �ö󰡷��� 1, ���������� -1�� ������ �����մϴ�.

    for (int i = 0; i < 6; i++) { // ��� ���������Ϳ� ���� �ݺ��մϴ�.
        if (ele.status[i] != 1 && (ele.direction[i] == 0 || ele.direction[i] == direction)) { // ���������Ͱ� ��� �����ϰ�, ����ڿ� ���� �������� ���� ��쿡�� ����մϴ�.
            int dist = abs(ele.floor[i] - current_floor); // ���� ���� ������������ �Ÿ��� ����մϴ�.
            if (dist < min_dist) { // ���� �� �Ÿ��� ���������� �ּ� �Ÿ����� ���� ���,
                min_dist = dist; // �ּ� �Ÿ��� ������Ʈ�ϰ�,
                closest = i; // ���� ����� ������������ �ε����� ������Ʈ�մϴ�.
            }
        }
    }

    if (closest != -1) { // ���� ����� ���������Ͱ� �����ϴ� ���,
        printf("�̵� ������ ���� ���� ����� ���������ʹ� %d�� �Դϴ�.\n", closest + 1); // ����ڿ��� ���� ����� ���������� ��ȣ�� �˸��ϴ�.
    }
    else { // ���� ����� ���������Ͱ� �������� �ʴ� ���,
        printf("������ �� �ִ� ���������Ͱ� �����ϴ�. ù ��° ���������Ͱ� �����Դϴ�.\n"); // ����ڿ��� �˸��ϴ�.
    }

    return closest; // ���� ����� ������������ �ε����� ��ȯ�մϴ�.
}

/** @brief ������� ������ġ�� �Է¹ް� ������� ���� ��ġ�� ��ȯ�ϴ� �Լ�
*  @date 2023/11/27
*  @return ������� ���� ��ġ�� ��ȯ
*/
int user_location() { // ������� ���� ��ġ�� �Է¹޴� �Լ��� �����մϴ�.
    int a; // ������� ���� ��ġ�� ������ ������ �����մϴ�.
    printf("��� ���� ��Ű���? (-10 ~ 100 / 0 ���� )\n"); // ����ڿ��� ���� ��ġ�� �Է¹޽��ϴ�.
    scanf("%d", &a); // ������� �Է��� ������ �����մϴ�.
    while ((a < -10 || a > 100) || (a == 0)) {
        printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (-10 ~ 100 / 0 ���� ): ");
        while (getchar() != '\n');  // ������ �Է��� ���ڵ��� ���ۿ��� �����մϴ�.
            scanf("%d", &a);
    }

    return a;
}


/** @brief ������������ ��ȣ, ��, ���� �� ������ִ� �Լ�
*  @date 2023_11_10
*/
void display_elevator_status() // ������������ ���¸� ����ϴ� �Լ��� �����մϴ�.
{

    printf("\n��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");// �������� ��� �׵θ��� ����մϴ�.
    printf("  Elevator 1   Elevator 2   Elevator 3   Elevator 4   Elevator 5   Elevator 6  \n");
    printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");// �� ������������ ���¸� �����ϴ� ������ ����մϴ�.
    printf("  Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d \n", ele.floor[0] != 0 ? ele.floor[0] : (ele.direction[0] == -1 ? -1 : 1),
        ele.floor[1] != 0 ? ele.floor[1] : (ele.direction[1] == -1 ? -1 : 1),
        ele.floor[2] != 0 ? ele.floor[2] : (ele.direction[2] == -1 ? -1 : 1),
        ele.floor[3] != 0 ? ele.floor[3] : (ele.direction[3] == -1 ? -1 : 1),
        ele.floor[4] != 0 ? ele.floor[4] : (ele.direction[4] == -1 ? -1 : 1),
        ele.floor[5] != 0 ? ele.floor[5] : (ele.direction[5] == -1 ? -1 : 1));// �� ������������ ���� ���� ����մϴ�.
    printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");// �� ������������ ���¸� �����ϴ� ������ ����մϴ�.
    printf("  ���� : %3d  ���� : %3d  ���� : %3d   ���� : %3d  ���� : %3d   ���� : %3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);
    printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");// �� ������������ ���¸� �����ϴ� ������ ����մϴ�.
    printf(" ");
    printf(" ");
    for (int i = 0; i < MAX_ELEVATORS; i++) { // ��� ���������Ϳ� ���� �ݺ��մϴ�.
        if (ele.status[i] == 3) { // ���� ���������Ͱ� �������� ���,
            printf("  ������ "); // "������"�̶�� ����մϴ�.
        }
        else { // �������� �ƴ� ���,
            printf("            "); // ������ ����մϴ�.
        }
        if (i == MAX_ELEVATORS - 1) { // ������ ������������ ���,
            printf("\n");// ���ο� �ٷ� �Ѿ�ϴ�.
        }
    }
    printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");// �������� �ϴ� �׵θ��� ����մϴ�.

}

/** @brief ���������� ���� ���ݴ� ��� �׿����� ������°� �׿����� ���������� ���� �ӵ��� ����.
* @date 2023/11/27
*  @param close_door : ���� ���ݴ� ������� �ǰ��� ���ڷ� ����
*/
void close(int close_door)
{
    while (( close_door !=0 )&& (close_door != 1)) 
    {
        printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� : ");
        clear_input_buffer();
        scanf("%d", &close_door);
    }

    if (close_door) {//�ݴ� ���
        printf("���� �ݽ��ϴ�. 1�� �� ����մϴ�.\n");//���� �ݴ´ٴ� ������ ���.
        Sleep(1000); // 1�� ���
    }
    else {//���� �ʴ� ���.
        printf("���� ���� �ʽ��ϴ�. 3�� �� ����մϴ�.\n");//���� ���� �ʴ´ٴ� ������ ���.
        Sleep(3000); // 3�� ���
    }
}

/** @brie���������͸� �̵����ָ� �� ������ �����ϴ� �Լ�
* @date 23/11/19
* @param target_floor1, user_floor1, target_floor2, user_floor2, target_floor3, user_floor3,
         target_floor4, user_floor4, target_floor5, user_floor5, target_floor6, user_floor6
         : ������ ������������ ���� ��ġ�� ��ǥ ��
*/
void move_elevator()
{
    int weight[6] = { 0 }; // �� ������������ ž���� ���Ը� �����ϴ� �迭 �߰�


    
    // ������� ���� ���� ���ϴ� �������� �Է¹޽��ϴ�.
    int a = user_location(); // ������� ���� ���� �Է¹޽��ϴ�.
    int f = user_input(); // ������� ���ϴ� �������� �Է¹޽��ϴ�.

    while (1)
    {
        if (a == f)
        {
            printf("���� ������ �̵��� �� �����ϴ�.\n");
            a=user_location();
            f=user_input(); //
        }
        else
        {
            break;
        }
    }

    // �� ���� ���������� ������ ������ �Ŀ� ���� ������ ���������͸� �����մϴ�.
    int index = elevator_selection(a, f); // ���� ����� ������������ �ε����� �����մϴ�.
    Sleep(1500); // 1.5�� ����մϴ�.

    for (int i = 0; i < 6; i++)
    {
        if (i == index)
        {
            ele.target[i] = f;
            ele.user_floor[i] = a;
        }
    }
    // �� ������������ ����� ���� ��ġ�� �������� �����մϴ�.

    int close_door;// ���� ������ �����ϴ� �����Դϴ�.

    for (int i = 0; i < 6; i++)
    {
        if (ele.user_floor[i]!=-11)
        {
            ele.direction[i] = (ele.user_floor[i] > ele.floor[i]) ? 1 : -1;// ����ڰ� �ִ� ���� ������������ ���� ���� ���Ͽ� �̵� ������ �����մϴ�.
        }
    }

    // ����ڰ� �ִ� ������ �̵�
    while (1) {
        
        for (int i = 0; i < 6; i++)
        {
            Inspection_confirm(i);//������������ ���� ���θ� Ȯ���մϴ�.
            ele.floor[i] += ele.direction[i];//���������͸� �̵���ŵ�ϴ�.

            if (ele.floor[i] == ele.user_floor[i])//����ڰ� ž���� ������������ ���
            {
                printf("1�� ž��\n");//ž���޴ٴ� ���� ���
                if (index == i) {   // ���õ� ���������Ͱ� 1���� ��

                    Sleep(500);// 0.5�� ���� ���α׷��� �����մϴ�.
                    system("cls");// �ܼ� ȭ���� ����ϴ�.
                    display_elevator_status();//display_elevator_status �Լ��� ȣ���Ͽ� ������������ ���¸� ���.

                    printf("ž���ϼ̽��ϴ�\n���� �����ðڽ��ϱ�? (��: 1, �ƴϿ�: 0): ");//���� �������� ����ڿ��� ���.
                    scanf("%d", &close_door);//����� close_door ������ ����.
                    close(close_door);//close �Լ��� ȣ��.
                    ele.kg[i] += user_sex();//ž������ ���Ը�ŭ ����.
                }
                kg(i);//������ ž������ ���Ը� ����.

            }
            get_off(i);
        }
        Sleep(500);// 0.5�� ���� ���α׷��� �����մϴ�.
        system("cls");// �ܼ� ȭ���� ����ϴ�.
        display_elevator_status();//display_elevator_status �Լ��� ȣ���Ͽ� ������������ ���¸� ���.

        for (int i = 0; i < 6; i++)
        {
            if (ele.direction[i] == 0)
            {
                    ele.user_floor[i] = random();
                    ele.target[i] = random();
                
            }
        }
 


        if ((ele.floor[index] ==0))
        {
            printf("���� �ϼ̽��ϴ� �޴��� ���ư��÷��� enter�� �����ּ���.\n");
            if (_kbhit()) {  // Ű���� �Է��� �ִ��� üũ
                if (_getch() == '\r') {  // ����ڰ� ���� Ű�� ������ ������ ���� ���ư��ϴ�.
                    adminmode();//adminmode �Լ� ȣ��


                }
                else
                {
                    ele.target[index]=random();
                    ele.user_floor[index] = random();
                }
            }
        }
    }
}


/** @brief ���α׷��� �����ϴ� main �Լ�.
*  @date 23/11/19
*/
int main()
{
    srand(time(NULL));  // ���� �õ� ����

    for (int i = 0; i < 6; i++)
    {
        ele.floor[i] = random();
        ele.user_floor[i] = random();
        ele.target[i] = random();
    }

    adminmode();//adminmode �Լ� ȣ��



    while (1)//���ѹݺ�
    {

        for (int i = 0; i < 6; i++)
        {
            ele.floor[i] = random();// �� ������������ �ʱ� ��ġ�� �������� �����մϴ�.
        }

        move_elevator();//move_elevator �Լ��� ȣ��
    }
}