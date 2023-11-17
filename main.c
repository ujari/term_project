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





int random() //-10 ~ 100 ���� ���� ����, -10�� ���Ϸ� �������� ����
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

// ������ ���� ���� ���
int calculate_weight(int gender) {
    return (gender == 0) ? MAN_KG : WOMAN_KG;
}


int user_input() {

    int floor;
    printf("��� ������ �̵��Ͻðڽ��ϱ�? (����: -10 ~ 100): ");
    scanf("%d", &floor);

    // �Է°� ����
    while (floor < -10 || floor > 100)
    {
        printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��� �ּ��� (-10 ~ 100): ");
        scanf("%d", &floor);
    }

    return floor;
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
        printf("  Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.target[0], ele.target[1], ele.target[2], ele.target[3], ele.target[4], ele.target[5]);
        printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
        printf("\n���� Ű�� ������ ������ ���� ���ư��ϴ�.\n");
        if (getchar() == '\n') {  // ����ڰ� ���͸� �������� Ȯ��
            break;  // ���͸� ������ while ������ ��������
        }
        Sleep(500);  // 500�и��� ���� ���
    }
    adminmode();  // ������ ���� ���ư���
}

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


int input()
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
    printf("  Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);
    printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
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
    printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");

}


void move_elevator(int target_floor1, int user_floor1, int target_floor2, int user_floor2, int target_floor3, int user_floor3, int target_floor4, int user_floor4, int target_floor5, int user_floor5, int target_floor6, int user_floor6)
{
    int weight[6] = { 0 }; // �� ������������ ž���� ���Ը� �����ϴ� �迭 �߰�

    for (int i = 0;i < 6;i++)
    {
        ele.floor[i] = random();
    }

    // ������� ���� ���� ���ϴ� �������� �Է¹޽���
    int a = input();
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
    target[0] = target_floor1;
    target[1] = target_floor2;
    target[2] = target_floor3;
    target[3] = target_floor4;
    target[4] = target_floor5;
    target[5] = target_floor6;
    int close_door;
    printf("%d�� ���������Ϳ� ž���ϼ̽��ϴ�. ���� �����ðڽ��ϱ�? (��: 1, �ƴϿ�: 0): ", index + 1);
    scanf("%d", &close_door);

    if (close_door) {
        printf("���� �ݽ��ϴ�. 1�� �� ����մϴ�.\n");
        Sleep(1000); // 1�� ���
    }
    else {
        printf("���� ���� �ʽ��ϴ�. 3�� �� ����մϴ�.\n");
        Sleep(3000); // 3�� ���
    }
    int direction1, direction2, direction3, direction4, direction5, direction6;
    direction1 = (user_floor1 > ele.floor[0]) ? 1 : -1;
    direction2 = (user_floor2 > ele.floor[1]) ? 1 : -1;
    direction3 = (user_floor3 > ele.floor[2]) ? 1 : -1;
    direction4 = (user_floor4 > ele.floor[3]) ? 1 : -1;
    direction5 = (user_floor5 > ele.floor[4]) ? 1 : -1;
    direction6 = (user_floor6 > ele.floor[5]) ? 1 : -1;
    // ����ڰ� �ִ� ������ �̵�
    while (1) {




        //1�� ����������

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

                if (elevatorStatus[0] == 0)
                {
                    user_floor1 = -11;
                    direction1 = 0;
                }
            }
            while (ele.kg[0] + weight[0] <= PASSENGER_KG) { // ���� ������ �ʰ����� ���� ������ ž���ڸ� �߰�
                ele.kg[0] += weight[0];
                weight[0] = calculate_weight(random_gender()); // ���� ž������ ���Ը� ���
            }
            Sleep(500);
            direction1 = (target_floor1 > ele.floor[0]) ? 1 : -1;
            user_floor1 = -11;
        }
        if (ele.floor[0] == target_floor1)
        {
            ele.kg[0] -= weight[0]; // �������� �����ϸ� ž���ڸ� ����
            if (ele.kg[0] < 0) ele.kg[0] = 0; // ���԰� ������ ���� �ʵ��� ��
            direction1 = 0;
        }
        //2�� ����������
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
                if (elevatorStatus[1] == 0)
                {
                    user_floor2 = -11;
                    direction2 = 0;
                }
            }
            while (ele.kg[1] + weight[1] <= PASSENGER_KG) { // ���� ������ �ʰ����� ���� ������ ž���ڸ� �߰�
                ele.kg[1] += weight[1];
                weight[1] = calculate_weight(random_gender()); // ���� ž������ ���Ը� ���
            }
            Sleep(500);
            direction2 = (target_floor2 > ele.floor[1]) ? 1 : -1;
            user_floor2 = -11;
        }
        if (ele.floor[1] == target_floor2)
        {
            ele.kg[1] -= weight[1]; // �������� �����ϸ� ž���ڸ� ����
            if (ele.kg[1] < 0) ele.kg[1] = 0; // ���԰� ������ ���� �ʵ��� ��
            direction2 = 0;
        }

        //3�� ����������
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
            while (ele.kg[2] + weight[2] <= PASSENGER_KG) { // ���� ������ �ʰ����� ���� ������ ž���ڸ� �߰�
                ele.kg[2] += weight[2];
                weight[2] = calculate_weight(random_gender()); // ���� ž������ ���Ը� ���
            }
            Sleep(500);
            direction3 = (target_floor3 > ele.floor[2]) ? 1 : -1;
            user_floor3 = -11;
        }
        if (ele.floor[2] == target_floor3)
        {
            ele.kg[2] -= weight[2]; // �������� �����ϸ� ž���ڸ� ����
            if (ele.kg[2] < 0) ele.kg[2] = 0; // ���԰� ������ ���� �ʵ��� ��
            direction3 = 0;
        }

        //4�� ����������
        if (ele.status[4] == 1)
        {
            printf("4�� ������");
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
            while (ele.kg[3] + weight[3] <= PASSENGER_KG) { // ���� ������ �ʰ����� ���� ������ ž���ڸ� �߰�
                ele.kg[3] += weight[3];
                weight[3] = calculate_weight(random_gender()); // ���� ž������ ���Ը� ���
            }
            Sleep(500);
            direction4 = (target_floor4 > ele.floor[3]) ? 1 : -1;
            user_floor4 = -11;
        }
        if (ele.floor[3] == target_floor4)
        {
            ele.kg[3] -= weight[3]; // �������� �����ϸ� ž���ڸ� ����
            if (ele.kg[3] < 0) ele.kg[3] = 0; // ���԰� ������ ���� �ʵ��� ��
            direction4 = 0;
        }
        //5�� ����������
        weight[3] = calculate_weight(random_gender());
        if (ele.status[4] == 1)
        {
            printf("5�� ������");
            elevatorStatus[4] = 3;
            user_floor5 = -11;
            direction5 = 0;
        }
        ele.floor[4] += direction5;

        if (ele.floor[4] == user_floor5)
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
            while (ele.kg[4] + weight[4] <= PASSENGER_KG) { // ���� ������ �ʰ����� ���� ������ ž���ڸ� �߰�
                ele.kg[4] += weight[4];
                weight[4] = calculate_weight(random_gender()); // ���� ž������ ���Ը� ���
            }
            Sleep(500);
            direction5 = (target_floor5 > ele.floor[4]) ? 1 : -1;
            user_floor5 = -11;
        }
        if (ele.floor[4] == target_floor5)
        {
            ele.kg[4] -= weight[4]; // �������� �����ϸ� ž���ڸ� ����
            if (ele.kg[4] < 0) ele.kg[4] = 0; // ���԰� ������ ���� �ʵ��� ��
            direction5 = 0;
        }
        //6������������
        weight[3] = calculate_weight(random_gender());
        if (ele.status[5] == 1)
        {
            printf("6�� ������");
            elevatorStatus[5] = 3;
            user_floor6 = -11;
            direction6 = 0;
        }
        ele.floor[5] += direction6;
        if (ele.floor[5] == user_floor6)
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
            while (ele.kg[5] + weight[5] <= PASSENGER_KG) { // ���� ������ �ʰ����� ���� ������ ž���ڸ� �߰�
                ele.kg[5] += weight[5];
                weight[5] = calculate_weight(random_gender()); // ���� ž������ ���Ը� ���
            }
            Sleep(500);
            direction6 = (target_floor6 > ele.floor[5]) ? 1 : -1;
            user_floor6 = -11;
        }
        if (ele.floor[5] == target_floor6)
        {
            ele.kg[5] -= weight[5]; // �������� �����ϸ� ž���ڸ� ����
            if (ele.kg[5] < 0) ele.kg[5] = 0; // ���԰� ������ ���� �ʵ��� ��
            direction6 = 0;
        }
        Sleep(500);
        system("cls");
        display_elevator_status();


    }
}




int main()
{
    srand(time(NULL));  // ���� �õ� ����


    adminmode();
    for (int i = 0;i < 6;i++) {
        printf("%d\n", ele.status[i]);
    }


    while (1)
    {


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