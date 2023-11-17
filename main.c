#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>
#include <process.h>

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
    char button[];     // �� ������������ ��ư ���¸� �����մϴ�. (���� ��ư�� 1, �ƴϸ� 0)
}ele;// ele��� �̸��� ����ü ������ �����մϴ�.

// ����ڰ� ���� ��ġ�� ���� ��ǥ ��
volatile int user_floor = 0;  // ����ڰ� ���� ��ġ�� ���� �����ϴ� ������ �����ϰ� 0���� �ʱ�ȭ�մϴ�.
volatile int target_floor = 0;  // ����ڰ� �̵��Ϸ��� ��ǥ ���� �����ϴ� ������ �����ϰ� 0���� �ʱ�ȭ�մϴ�.





// -10 ~ 100 ���� ���� ����. ���������Ͱ� �̵��� ���� �������� �����ϴµ� ���
int random() 
{
    int random;  // ������ ������ ������ �����մϴ�.
    do {
        random = (rand() % 111) - 10;  // -10 ~ 100 ������ ������ �����մϴ�.
    } while (random < -10);  // ������ ������ -10���� ������ �ٽ� ������ �����մϴ�.
    return random;  // ������ ������ ��ȯ�մϴ�.
}

int random_gender() {  // �������� ������ �����ϴ� �Լ��� �����մϴ�.
    return rand() % 2;  // 0 �Ǵ� 1�� �������� ��ȯ�մϴ�.
}

int calculate_weight(int gender) {  // ������ ���� ���Ը� ����ϴ� �Լ��� �����մϴ�.
    return (gender == 0) ? MAN_KG : WOMAN_KG;  // ������ '0'�̸� 'MAN_KG'��, �ƴϸ� 'WOMAN_KG'�� ��ȯ�մϴ�.
}

int user_input()  // ����ڷκ��� �̵��Ϸ��� ���� �Է¹޴� �Լ��� �����մϴ�.
{
    int floor;  // ����ڰ� �Է��� ���� ������ ������ �����մϴ�.
    printf("��� ������ �̵��Ͻðڽ��ϱ�? (����: -10 ~ 100): ");  // ����ڿ��� �Է��� ��û�ϴ� �޽����� ����մϴ�.
    scanf("%d", &floor);  // ������� �Է��� �޾� 'floor' ������ �����մϴ�.

    // ����ڰ� �Է��� ���� -10 ~ 100 ���̰� �ƴϸ� �ٽ� �Է¹޽��ϴ�.
    while (floor < -10 || floor > 100)
    {
        printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (-10 ~ 100): ");  // �߸� �Է��Ͽ����� �˸��� �޽����� ����մϴ�.
        scanf("%d", &floor);  // ������� �Է��� �ٽ� �޾� 'floor' ������ �����մϴ�.
    }

    return floor;  // ����ڰ� �Է��� ���� ��ȯ�մϴ�.
}


int password = 1234;  // �ʱ� ��й�ȣ ����
int weight[MAX_ELEVATORS] = { 0 };
int adminMode = 0; // ������ ��� ���� (0: ��Ȱ��ȭ, 1: Ȱ��ȭ)
int elevatorStatus[MAX_ELEVATORS] = { 0 }; // �� ������������ ���� (0: ����, 1: ���� ��)

void change_password() {  // ��й�ȣ ���� �Լ�
    printf("���ο� 4�ڸ� ��й�ȣ�� �Է����ּ���: ");
    scanf("%d", &password);
    printf("��й�ȣ�� ����Ǿ����ϴ�.\n");
}

void select_elevator_for_maintenance() {
    int pick = 2;
    int lift = 2;
    while (1)
    {
        printf("���ư���� 1���� ������ ������ 0�� �Է��ϼ��� >> >");
        scanf("%d", &pick);
        if (pick == 0)
        {
            printf("���� ������ 0�� ���� ������ 1���� �����ּ��� >> ");
            scanf("%d", &lift);
            if (lift == 1)
            {
                int elevator;
                printf("������ ���������͸� �������ּ��� (1-6): ");
                scanf("%d", &elevator);
                if (elevator >= 1 && elevator <= MAX_ELEVATORS) {
                    ele.status[elevator - 1] = 1;
                    printf("%d�� ���������Ͱ� ���� ���� ��ȯ�Ǿ����ϴ�.\n", elevator);
                }
                else {
                    printf("�߸��� ���������� ��ȣ�Դϴ�.\n");
                }
            }
            else {
                int elevator;
                printf("������ ���������͸� �������ּ��� (1-6): ");
                scanf("%d", &elevator);
                if (elevator >= 1 && elevator <= MAX_ELEVATORS) {
                    ele.status[elevator - 1] = 0;
                    printf("%d�� ���������Ͱ� ���� ������ ��ȯ�Ǿ����ϴ�.\n", elevator);
                }
                else {
                    printf("�߸��� ���������� ��ȣ�Դϴ�.\n");
                }
            }

        }
        else if (pick == 1) {
            break;
        }
    }
}

void admin_display()
{


    while (1) {
        system("cls");
        printf("\n��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
        printf("  Elevator 1   Elevator 2   Elevator 3   Elevator 4   Elevator 5   Elevator 6  \n");
        printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
        printf("  Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d \n", ele.floor[0], ele.floor[1], ele.floor[2], ele.floor[3], ele.floor[4], ele.floor[5]);
        printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
        printf("  Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);
        printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
        printf(" ");
        for (int i = 0; i < MAX_ELEVATORS; i++) {
            if (ele.status[i] == 1) {
                printf("������ ");
            }
            else {
                printf("            ");
            }
            if (i == MAX_ELEVATORS - 1) {
                printf("\n");
            }
        }
        printf(" \n");
        printf("  ��ǥ�� :%3d   ��ǥ�� :%3d   ��ǥ�� :%3d   ��ǥ�� :%3d   ��ǥ�� :%3d   ��ǥ�� :%3d \n", ele.target[0], ele.target[1], ele.target[2], ele.target[3], ele.target[4], ele.target[5]);
        printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
        printf("  ���� : %c   ���� :%c   ����  :%c   ���� :%c   ���� :%c   ���� :%c \n", ele.button[0], ele.button[1], ele.button[2], ele.button[3], ele.button[4], ele.button[5]);
        printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
        printf("\n���� Ű�� ������ ������ ���� ���ư��ϴ�.\n");
        if (getchar() == '\n') {  // ����ڰ� ���͸� �������� Ȯ��
            break;  // ���͸� ������ while ������ ��������
        }
        Sleep(500);  // 500�и��� ���� ���
    }

}

void adminmode() {
    int e = 2;
    printf("���α׷� ���Ḧ ���Ͻø� 0��, ����Ϸ��� 1��, ������ ��� ������ 2�� �Է����ּ���: ");
    scanf("%d", &e);

    while (e != 0 && e != 1 && e != 2) {
        printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (0, 1 �Ǵ� 2): ");
        scanf("%d", &e);
    }
    if (e == 0) {
        printf("���α׷��� �����մϴ�.\n");
        exit(0);  // ���α׷� ����
    }
    else if (e == 2) {
        int input_password;
        printf("3�� ��ġ���������� ����� ���� ���ư��ϴ�.\n");
        printf("��й�ȣ�� �Է����ּ���: ");
        scanf("%d", &input_password);
        int i = 0;

        if (input_password == password) {  // ��й�ȣ ��ġ Ȯ��
            printf("������ ��忡 �����մϴ�.\n");
            adminMode = 1;

            while (adminMode) {
                printf("���������� ������ 1��, ��й�ȣ ������ 2��, ���������� ��Ȳ���� 3 ,������ ��� ����� 4�� �Է����ּ���: ");
                scanf("%d", &e);

                if (e == 1) {
                    select_elevator_for_maintenance();
                }
                else if (e == 2) {
                    change_password();  // ��й�ȣ ����
                }
                else if (e == 3) {
                    admin_display();  // ���������� ��Ȳ
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
        else {
            Sleep(1000);
            adminmode();
        }
    }
}




int user_sex()
{
    int sex;  // ������ ������ ������ �����մϴ�.
    printf("���ڸ� 0 ���ڸ� 1�� �����ּ���.");  // ����ڿ��� �Է��� ��û�ϴ� �޽����� ����մϴ�.
    scanf("%d", &sex);  // ������� �Է��� �޾� ������ �����մϴ�.
    do {  // �ݺ����� �����մϴ�.
        if (sex == 0)  // ������� �Է��� 0�� ���,
        {
            break;  // �ݺ����� �����մϴ�.
        }
        else if (sex == 1)  // ������� �Է��� 1�� ���,
        {
            break;  // �ݺ����� �����մϴ�.
        }
        else  // ������� �Է��� 0�� 1�� �ƴ� ���,
        {
            printf("�߸��� �Է��Դϴ� �ٽ� �Է����ּ���.\n");  // �߸��� �Է����� �˸��� �޽����� ����ϰ�,
            printf("���ڸ� 0 ���ڸ� 1�� �����ּ���.");  // �ٽ� �Է��� ��û�ϴ� �޽����� ����մϴ�.
            scanf("%d", &sex);  // ������� ���ο� �Է��� �޽��ϴ�.
        }
    } while (sex != 1 && sex != 0);  // ������� �Է��� 1�� �ƴϰ�, 0�� �ƴ� ��쿡 �ݺ����� ��� �����մϴ�.

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
    int closest = 0; // �ʱ� ���� 0���� ����
    int min_dist = MAX_FLOOR;
    int direction = (target_floor > current_floor) ? 1 : -1; // ����ڰ� �ö󰡷��� 1, ���������� -1

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
        printf("�̵� ������ ���� ���� ����� ���������ʹ� %d�� �Դϴ�.\n", closest + 1);
    }
    else
    {
        printf("������ �� �ִ� ���������Ͱ� �����ϴ�. ù ��° ���������Ͱ� �����Դϴ�.\n");
    }

    return closest;
}


int user_location()
{
    int a;
    printf("��� ���� ��Ű���?\n");
    scanf("%d", &a);

    return a;
}
void display_elevator_status()
{


    printf("\n��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
    printf("  Elevator 1   Elevator 2   Elevator 3   Elevator 4   Elevator 5   Elevator 6  \n");
    printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
    printf("  Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d \n", ele.floor[0], ele.floor[1], ele.floor[2], ele.floor[3], ele.floor[4], ele.floor[5]);
    printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
    printf("  ������:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);
    printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
    printf(" ");
    for (int i = 0; i < MAX_ELEVATORS; i++) {
        if (ele.status[i] == 1) {
            printf("  ������ ");
        }
        else {
            printf("            ");
        }
        if (i == MAX_ELEVATORS - 1) {
            printf("\n");
        }
    }
    printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");

}

void Inspection_confirm(int i)
{
    if (elevatorStatus[i] == 1)
    {
        ele.status[i] = 3;
        ele.user_floor[i] = -11;
        ele.direction[i] = 0;
    }
    else
    {
        weight[i] = calculate_weight(random_gender());
    }
}


void kg(int i)
{
    while (ele.kg[i] + weight[i] <= PASSENGER_KG) { // ���� ������ �ʰ����� ���� ������ ž���ڸ� �߰�
        ele.kg[i] += weight[i];
        weight[i] = calculate_weight(random_gender()); // ���� ž������ ���Ը� ���
    }
    Sleep(500);
    ele.direction[i] = (ele.target[i] > ele.floor[i]) ? 1 : -1;
    ele.user_floor[i] = -11;
}


void get_off(int i)
{
    if (ele.floor[i] == ele.target[i])
    {
        ele.kg[i] -= weight[i]; // �������� �����ϸ� ž���ڸ� ����
        if (ele.kg[i] < 0) ele.kg[i] = 0; // ���԰� ������ ���� �ʵ��� ��
        ele.direction[i] = 0;
    }
}

void move_elevator(int target_floor1, int user_floor1, int target_floor2, int user_floor2, int target_floor3, int user_floor3, int target_floor4, int user_floor4, int target_floor5, int user_floor5, int target_floor6, int user_floor6)
{
    int weight[6] = { 0 }; // �� ������������ ž���� ���Ը� �����ϴ� �迭 �߰�

    for (int i = 0;i < 6;i++)
    {
        ele.floor[i] = random();
    }

    // ������� ���� ���� ���ϴ� �������� �Է¹޽���
    int a = user_location();
    int f = user_input();

    // �� ���� ���������� ������ ������ �Ŀ� ���� ������ ���������͸� �����մϴ�.
    int index = elevator_selection(a, f);
    Sleep(1500);

    // �׸��� ��ȯ�� + 1�� �ش��ϴ� �����鸸 a�� floor�� �����մϴ�.
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
    ele.user_floor[0] = user_floor1;
    ele.user_floor[1] = user_floor2;
    ele.user_floor[2] = user_floor3;
    ele.user_floor[3] = user_floor4;
    ele.user_floor[4] = user_floor5;
    ele.user_floor[5] = user_floor6;

    ele.target[0] = target_floor1;
    ele.target[1] = target_floor2;
    ele.target[2] = target_floor3;
    ele.target[3] = target_floor4;
    ele.target[4] = target_floor5;
    ele.target[5] = target_floor6;

    int close_door;

    int direction1, direction2, direction3, direction4, direction5, direction6;
    ele.direction[0] = (user_floor1 > ele.floor[0]) ? 1 : -1;
    ele.direction[1] = (user_floor2 > ele.floor[1]) ? 1 : -1;
    ele.direction[2] = (user_floor3 > ele.floor[2]) ? 1 : -1;
    ele.direction[3] = (user_floor4 > ele.floor[3]) ? 1 : -1;
    ele.direction[4] = (user_floor5 > ele.floor[4]) ? 1 : -1;
    ele.direction[5] = (user_floor6 > ele.floor[5]) ? 1 : -1;
    // ����ڰ� �ִ� ������ �̵�
    while (1) {
        
        //1�� ����������

        Inspection_confirm(0);
        ele.floor[0] += ele.direction[0];
        if (ele.floor[0] == ele.user_floor[0])
        {
            printf("1�� ž��");
            if (index + 1 == 1) {   // ���õ� ���������Ͱ� 1���� ��
                printf("���� �����ðڽ��ϱ�? (��: 1, �ƴϿ�: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("���� �ݽ��ϴ�. 1�� �� ����մϴ�.\n");
                    Sleep(1000); // 1�� ���
                }
                else {
                    printf("���� ���� �ʽ��ϴ�. 3�� �� ����մϴ�.\n");
                    Sleep(3000); // 3�� ���
                }
                ele.kg[0] += user_sex();
            }
            kg(0);

        }
        get_off(0);
        //2�� ����������
        Inspection_confirm(1);
        ele.floor[1] += ele.direction[1];
        if (ele.floor[1] == ele.user_floor[1])
        {
            printf("2�� ž��");
            if (index + 1 == 2) {   // ���õ� ���������Ͱ� 1���� ��
                printf("���� �����ðڽ��ϱ�? (��: 1, �ƴϿ�: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("���� �ݽ��ϴ�. 1�� �� ����մϴ�.\n");
                    Sleep(1000); // 1�� ���
                }
                else {
                    printf("���� ���� �ʽ��ϴ�. 3�� �� ����մϴ�.\n");
                    Sleep(3000); // 3�� ���
                }
                ele.kg[1] += user_sex();
            }
            kg(1);
        }

        get_off(1);

        //3�� ����������
        Inspection_confirm(2);
        ele.floor[2] += ele.direction[2];
        if (ele.floor[2] == ele.user_floor[2])
        {
            printf("3�� ž��");
            if (index + 1 == 3) {   // ���õ� ���������Ͱ� 1���� ��
                printf("���� �����ðڽ��ϱ�? (��: 1, �ƴϿ�: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("���� �ݽ��ϴ�. 1�� �� ����մϴ�.\n");
                    Sleep(1000); // 1�� ���
                }
                else {
                    printf("���� ���� �ʽ��ϴ�. 3�� �� ����մϴ�.\n");
                    Sleep(3000); // 3�� ���
                }
                ele.kg[2] += user_sex();
            }
            kg(2);
        }
        get_off(2);

        //4�� ����������
        Inspection_confirm(3);
        ele.floor[3] += ele.direction[3];
        if (ele.floor[3] == ele.user_floor[3])
        {
            printf("4�� ž��");
            if (index + 1 == 4) {   // ���õ� ���������Ͱ� 1���� ��
                printf("���� �����ðڽ��ϱ�? (��: 1, �ƴϿ�: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("���� �ݽ��ϴ�. 1�� �� ����մϴ�.\n");
                    Sleep(1000); // 1�� ���
                }
                else {
                    printf("���� ���� �ʽ��ϴ�. 3�� �� ����մϴ�.\n");
                    Sleep(3000); // 3�� ���
                }
                ele.kg[3] += user_sex();
            }
            kg(3);
        }
        get_off(3);
        //5�� ����������
        Inspection_confirm(4);
        ele.floor[4] += ele.direction[4];

        if (ele.floor[4] == ele.user_floor[4])
        {

            printf("5�� ž��");
            if (index + 1 == 5) {   // ���õ� ���������Ͱ� 1���� ��
                printf("���� �����ðڽ��ϱ�? (��: 1, �ƴϿ�: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("���� �ݽ��ϴ�. 1�� �� ����մϴ�.\n");
                    Sleep(1000); // 1�� ���
                }
                else {
                    printf("���� ���� �ʽ��ϴ�. 3�� �� ����մϴ�.\n");
                    Sleep(3000); // 3�� ���
                }
                ele.kg[4] += user_sex();
            }
            kg(4);
        }
        get_off(4);
        //6������������
        Inspection_confirm(5);
        ele.floor[5] += ele.direction[5];
        if (ele.floor[5] == ele.user_floor[5])
        {
            printf("6�� ž��");
            if (index + 1 == 6) {   // ���õ� ���������Ͱ� 1���� ��
                printf("���� �����ðڽ��ϱ�? (��: 1, �ƴϿ�: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("���� �ݽ��ϴ�. 1�� �� ����մϴ�.\n");
                    Sleep(1000); // 1�� ���
                }
                else {
                    printf("���� ���� �ʽ��ϴ�. 3�� �� ����մϴ�.\n");
                    Sleep(3000); // 3�� ���
                }
                ele.kg[5] += user_sex();
            }
            kg(5);
        }
        get_off(5);
        Sleep(500);
        system("cls");
        display_elevator_status();

        if ((ele.direction[0] == 0) && (ele.direction[1] == 0) && (ele.direction[2] == 0) && (ele.direction[3] == 0) && (ele.direction[5] == 0) && (ele.direction[5] == 0))
        {
            break;
        }


    }
}

int button()
{   
    for (int i = 0;i < 6;i++)
    {
        ele.button[i] =0;
    }


    for (int i=0 ;i < 6;i++)
    {
        if (ele.direction[i] == 1)
        {
            ele.button[i] = "��";
        }
        else {
            ele.button[i] = "��";
        }
    }
}


int main()
{
    srand(time(NULL));  // ���� �õ� ����


    while (1)
    {
        adminmode();
        for (int i = 0;i < 6;i++) {
            printf("%d\n", ele.status[i]);
        }

        // ���� ��� ���� �����ϰ� �����մϴ�.
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
}