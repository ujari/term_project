#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>
#include <process.h>

#define MAX_ELEVATORS 6
#define PASSENGER_KG 1450 //識 鱉蝓濠曖 譆渠 鼠啪蒂 瞪籀葬
#define MAN_KG 75 //陴濠曖 跺鼠啪
#define WOMAN_KG 55 //罹濠曖 跺鼠啪
#define MAX_FLOOR 100 //譆渠類擊 100戲煎 瞪籀葬
#define MIN_FLOOR -10 //譆ж類擊 -10 戲煎 瞪籀葬


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





int random() //-10 ~ 100 梱雖 陪熱 儅撩, -10類 檜ж煎 頂溥陛雖 彊擠
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

// 撩滌縑 評艇 鼠啪 啗骯
int calculate_weight(int gender) {
    return (gender == 0) ? MAN_KG : WOMAN_KG;
}


int user_input() {

    int floor;
    printf("橫替 類戲煎 檜翕ж衛啊蝗棲梱? (彰嬪: -10 ~ 100): ");
    scanf("%d", &floor);

    // 殮溘高 匐隸
    while (floor < -10 || floor > 100)
    {
        printf("澀跤 殮溘ж樟蝗棲棻. 棻衛 殮溘п 輿撮蹂 (-10 ~ 100): ");
        scanf("%d", &floor);
    }

    return floor;
}

void admin_display()
{


    while (1) {
        system("cls");
        printf("\n忙式式式式式式式式式式式式成式式式式式式式式式式式式成式式式式式式式式式式式式成式式式式式式式式式式式式成式式式式式式式式式式式式成式式式式式式式式式式式式忖\n");
        printf("  Elevator 1   Elevator 2   Elevator 3   Elevator 4   Elevator 5   Elevator 6  \n");
        printf("戍式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式扣\n");
        printf("  Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d \n", ele.floor[0], ele.floor[1], ele.floor[2], ele.floor[3], ele.floor[4], ele.floor[5]);
        printf("戍式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式扣\n");
        printf("  Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);
        printf("戍式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式扣\n");
        printf(" ");
        for (int i = 0; i < MAX_ELEVATORS; i++) {
            if (ele.status[i] == 1) {
                printf("薄匐醞 ");
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
        printf("戌式式式式式式式式式式式式扛式式式式式式式式式式式式扛式式式式式式式式式式式式扛式式式式式式式式式式式式扛式式式式式式式式式式式式扛式式式式式式式式式式式式戎\n");
        printf("\n縛攪 酈蒂 援腦賊 婦葬濠 賅萄煎 給嬴骨棲棻.\n");
        if (getchar() == '\n') {  // 餌辨濠陛 縛攪蒂 揚毓朝雖 �挫�
            break;  // 縛攪蒂 援腦賊 while 瑞Щ蒂 緒螳釭馬
        }
        Sleep(500);  // 500塵葬蟾 翕寰 渠晦
    }
    adminmode();  // 婦葬濠 賅萄煎 給嬴陛晦
}

}




int password = 1234;  // 蟾晦 綠塵廓�� 撲薑
int weight[MAX_ELEVATORS] = { 0 };
int adminMode = 0; // 婦葬濠 賅萄 鼻鷓 (0: 綠�側瘓�, 1: �側瘓�)
int elevatorStatus[MAX_ELEVATORS] = { 0 }; // 陝 縣葬漆檜攪曖 鼻鷓 (0: 薑鼻, 1: 薄匐 醞)

void change_password() {  // 綠塵廓�� 滲唳 л熱
    printf("億煎遴 4濠葬 綠塵廓�ㄧ� 殮溘п輿撮蹂: ");
    scanf("%d", &password);
    printf("綠塵廓�ㄟ� 滲唳腎歷蝗棲棻.\n");
}

void select_elevator_for_maintenance() {
    int pick = 2;
    int lift = 2;
    while (1)
    {
        printf("給嬴陛晦朝 1廓擊 薄匐擊 霞ч擎 0擊 殮溘ж撮蹂 >> >");
        scanf("%d", &pick);
        if (pick == 0)
        {
            printf("薄匐 п薯朝 0廓 薄匐 撲薑擎 1廓擊 揚楝輿撮蹂 >> ");
            scanf("%d", &lift);
            if (lift == 1)
            {
                int elevator;
                printf("薄匐й 縣葬漆檜攪蒂 摹鷗п輿撮蹂 (1-6): ");
                scanf("%d", &elevator);
                if (elevator >= 1 && elevator <= MAX_ELEVATORS) {
                    ele.status[elevator - 1] = 1;
                    printf("%d廓 縣葬漆檜攪陛 薄匐 賅萄煎 瞪�秘Ц�蝗棲棻.\n", elevator);
                }
                else {
                    printf("澀跤脹 縣葬漆檜攪 廓��殮棲棻.\n");
                }
            }
            else {
                int elevator;
                printf("п薯й 縣葬漆檜攪蒂 摹鷗п輿撮蹂 (1-6): ");
                scanf("%d", &elevator);
                if (elevator >= 1 && elevator <= MAX_ELEVATORS) {
                    ele.status[elevator - 1] = 0;
                    printf("%d廓 縣葬漆檜攪陛 薄匐 п薯煎 瞪�秘Ц�蝗棲棻.\n", elevator);
                }
                else {
                    printf("澀跤脹 縣葬漆檜攪 廓��殮棲棻.\n");
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
    printf("Щ煎斜極 謙猿蒂 錳ж衛賊 0擊, 啗樓ж溥賊 1擊, 婦葬濠 賅萄 霞殮擎 2蒂 殮溘п輿撮蹂: ");
    scanf("%d", &e);

    while (e != 0 && e != 1 && e != 2) {
        printf("澀跤 殮溘ж樟蝗棲棻. 棻衛 殮溘п 輿撮蹂 (0, 1 傳朝 2): ");
        scanf("%d", &e);
    }
    if (e == 0) {
        printf("Щ煎斜極擊 謙猿м棲棻.\n");
        exit(0);  // Щ煎斜極 謙猿
    }
    else if (e == 2) {
        int input_password;
        printf("3廓 橾纂ж雖彊戲賊 餌辨濠 賅萄煎 給嬴骨棲棻.\n");
        printf("綠塵廓�ㄧ� 殮溘п輿撮蹂: ");
        scanf("%d", &input_password);
        int i = 0;

        if (input_password == password) {  // 綠塵廓�� 橾纂 �挫�
            printf("婦葬濠 賅萄縑 霞殮м棲棻.\n");
            adminMode = 1;

            while (adminMode) {
                printf("縣葬漆檜攪 薄匐擎 1擊, 綠塵廓�� 滲唳擎 2蒂, 縣葬漆檜攪 ⑷�笙Ш� 3 ,婦葬濠 賅萄 謙猿朝 4擊 殮溘п輿撮蹂: ");
                scanf("%d", &e);

                if (e == 1) {
                    select_elevator_for_maintenance();
                }
                else if (e == 2) {
                    change_password();  // 綠塵廓�� 滲唳
                }
                else if (e == 3) {
                    admin_display();  // 縣葬漆檜攪 ⑷��
                }
                else if (e == 4) {
                    printf("婦葬濠 賅萄蒂 謙猿м棲棻.\n");
                    adminMode = 0;
                }
                else {
                    printf("澀跤脹 殮溘殮棲棻.\n");
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
    int sex;  // 撩滌擊 盪濰й 滲熱蒂 摹樹м棲棻.
    printf("陴濠賊 0 罹濠賊 1擊 揚楝輿撮蹂.");  // 餌辨濠縑啪 殮溘擊 蹂羶ж朝 詭衛雖蒂 轎溘м棲棻.
    scanf("%d", &sex);  // 餌辨濠曖 殮溘擊 嫡嬴 滲熱縑 盪濰м棲棻.
    do {  // 奩犒僥擊 衛濛м棲棻.
        if (sex == 0)  // 餌辨濠曖 殮溘檜 0檣 唳辦,
        {
            break;  // 奩犒僥擊 謙猿м棲棻.
        }
        else if (sex == 1)  // 餌辨濠曖 殮溘檜 1檣 唳辦,
        {
            break;  // 奩犒僥擊 謙猿м棲棻.
        }
        else  // 餌辨濠曖 殮溘檜 0紫 1紫 嬴棋 唳辦,
        {
            printf("澀跤脹 殮溘殮棲棻 棻衛 殮溘п輿撮蹂.\n");  // 澀跤脹 殮溘歜擊 憲葬朝 詭衛雖蒂 轎溘ж堅,
            printf("陴濠賊 0 罹濠賊 1擊 揚楝輿撮蹂.");  // 棻衛 殮溘擊 蹂羶ж朝 詭衛雖蒂 轎溘м棲棻.
            scanf("%d", &sex);  // 餌辨濠曖 億煎遴 殮溘擊 嫡蝗棲棻.
        }
    } while (sex != 1 && sex != 0);  // 餌辨濠曖 殮溘檜 1檜 嬴棲堅, 0紫 嬴棋 唳辦縑 奩犒僥擊 啗樓 褒чм棲棻.

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
    int closest = 0; // 蟾晦 高擊 0戲煎 撲薑
    int min_dist = MAX_FLOOR;
    int direction = (target_floor > current_floor) ? 1 : -1; // 餌辨濠陛 螢塭陛溥賊 1, 頂溥陛溥賊 -1

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
        printf("檜翕 寞щ檜 偽堅 陛濰 陛梱遴 縣葬漆檜攪朝 %d廓 殮棲棻.\n", closest + 1);
    }
    else
    {
        printf("摹滌й 熱 氈朝 縣葬漆檜攪陛 橈蝗棲棻. 羅 廓簞 縣葬漆檜攪陛 遺霜殮棲棻.\n");
    }

    return closest;
}


int input()
{
    int a;
    printf("橫替 類縑 啗褐陛蹂?\n");
    scanf("%d", &a);

    return a;
}
void display_elevator_status()
{


    printf("\n忙式式式式式式式式式式式式成式式式式式式式式式式式式成式式式式式式式式式式式式成式式式式式式式式式式式式成式式式式式式式式式式式式成式式式式式式式式式式式式忖\n");
    printf("  Elevator 1   Elevator 2   Elevator 3   Elevator 4   Elevator 5   Elevator 6  \n");
    printf("戍式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式扣\n");
    printf("  Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d   Floor: %3d \n", ele.floor[0], ele.floor[1], ele.floor[2], ele.floor[3], ele.floor[4], ele.floor[5]);
    printf("戍式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式扣\n");
    printf("  Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d   Weight:%3d \n", ele.kg[0], ele.kg[1], ele.kg[2], ele.kg[3], ele.kg[4], ele.kg[5]);
    printf("戍式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式托式式式式式式式式式式式式扣\n");
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
    printf("戌式式式式式式式式式式式式扛式式式式式式式式式式式式扛式式式式式式式式式式式式扛式式式式式式式式式式式式扛式式式式式式式式式式式式扛式式式式式式式式式式式式戎\n");

}


void move_elevator(int target_floor1, int user_floor1, int target_floor2, int user_floor2, int target_floor3, int user_floor3, int target_floor4, int user_floor4, int target_floor5, int user_floor5, int target_floor6, int user_floor6)
{
    int weight[6] = { 0 }; // 陝 縣葬漆檜攪曖 鱉蝓濠 鼠啪蒂 盪濰ж朝 寡翮 蹺陛

    for (int i = 0;i < 6;i++)
    {
        ele.floor[i] = random();
    }

    // 餌辨濠曖 ⑷營 類婁 錳ж朝 跡瞳雖蒂 殮溘嫡蝗棲
    int a = input();
    int f = user_input();

    // 陝 類曖 縣葬漆檜攪 類熱陛 唸薑脹 �醴� 陛濰 瞳ми 縣葬漆檜攪蒂 摹鷗м棲棻.
    int index = elevator_selection(a, f);
    Sleep(1500);

    // 斜葬堅 奩�秣� + 1縑 п渡ж朝 滲熱菟虜 a諦 floor煎 撲薑м棲棻.
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
    printf("%d廓 縣葬漆檜攪縑 鱉蝓ж樟蝗棲棻. 僥擊 殘戲衛啊蝗棲梱? (蕨: 1, 嬴棲螃: 0): ", index + 1);
    scanf("%d", &close_door);

    if (close_door) {
        printf("僥擊 殘蝗棲棻. 1蟾 �� 轎嫦м棲棻.\n");
        Sleep(1000); // 1蟾 渠晦
    }
    else {
        printf("僥擊 殘雖 彊蝗棲棻. 3蟾 �� 轎嫦м棲棻.\n");
        Sleep(3000); // 3蟾 渠晦
    }
    int direction1, direction2, direction3, direction4, direction5, direction6;
    direction1 = (user_floor1 > ele.floor[0]) ? 1 : -1;
    direction2 = (user_floor2 > ele.floor[1]) ? 1 : -1;
    direction3 = (user_floor3 > ele.floor[2]) ? 1 : -1;
    direction4 = (user_floor4 > ele.floor[3]) ? 1 : -1;
    direction5 = (user_floor5 > ele.floor[4]) ? 1 : -1;
    direction6 = (user_floor6 > ele.floor[5]) ? 1 : -1;
    // 餌辨濠陛 氈朝 類戲煎 檜翕
    while (1) {




        //1廓 縣葬漆檜攪

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
            printf("1廓 鱉蝓");
            if (index + 1 == 1) {   // 摹鷗脹 縣葬漆檜攪陛 1廓橾 陽
                printf("僥擊 殘戲衛啊蝗棲梱? (蕨: 1, 嬴棲螃: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("僥擊 殘蝗棲棻. 1蟾 �� 轎嫦м棲棻.\n");
                    Sleep(1000); // 1蟾 渠晦
                }
                else {
                    printf("僥擊 殘雖 彊蝗棲棻. 3蟾 �� 轎嫦м棲棻.\n");
                    Sleep(3000); // 3蟾 渠晦
                }
                ele.kg[0] += user_sex();

                if (elevatorStatus[0] == 0)
                {
                    user_floor1 = -11;
                    direction1 = 0;
                }
            }
            while (ele.kg[0] + weight[0] <= PASSENGER_KG) { // 鼠啪 薯и擊 蟾婁ж雖 彊擊 陽梱雖 鱉蝓濠蒂 蹺陛
                ele.kg[0] += weight[0];
                weight[0] = calculate_weight(random_gender()); // 棻擠 鱉蝓濠曖 鼠啪蒂 啗骯
            }
            Sleep(500);
            direction1 = (target_floor1 > ele.floor[0]) ? 1 : -1;
            user_floor1 = -11;
        }
        if (ele.floor[0] == target_floor1)
        {
            ele.kg[0] -= weight[0]; // 跡瞳雖縑 紫雜ж賊 鱉蝓濠蒂 頂葡
            if (ele.kg[0] < 0) ele.kg[0] = 0; // 鼠啪陛 擠熱陛 腎雖 彊紫煙 л
            direction1 = 0;
        }
        //2廓 縣葬漆檜攪
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
            printf("2廓 鱉蝓");
            if (index + 1 == 2) {   // 摹鷗脹 縣葬漆檜攪陛 1廓橾 陽
                printf("僥擊 殘戲衛啊蝗棲梱? (蕨: 1, 嬴棲螃: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("僥擊 殘蝗棲棻. 1蟾 �� 轎嫦м棲棻.\n");
                    Sleep(1000); // 1蟾 渠晦
                }
                else {
                    printf("僥擊 殘雖 彊蝗棲棻. 3蟾 �� 轎嫦м棲棻.\n");
                    Sleep(3000); // 3蟾 渠晦
                }
                ele.kg[1] += user_sex();
                if (elevatorStatus[1] == 0)
                {
                    user_floor2 = -11;
                    direction2 = 0;
                }
            }
            while (ele.kg[1] + weight[1] <= PASSENGER_KG) { // 鼠啪 薯и擊 蟾婁ж雖 彊擊 陽梱雖 鱉蝓濠蒂 蹺陛
                ele.kg[1] += weight[1];
                weight[1] = calculate_weight(random_gender()); // 棻擠 鱉蝓濠曖 鼠啪蒂 啗骯
            }
            Sleep(500);
            direction2 = (target_floor2 > ele.floor[1]) ? 1 : -1;
            user_floor2 = -11;
        }
        if (ele.floor[1] == target_floor2)
        {
            ele.kg[1] -= weight[1]; // 跡瞳雖縑 紫雜ж賊 鱉蝓濠蒂 頂葡
            if (ele.kg[1] < 0) ele.kg[1] = 0; // 鼠啪陛 擠熱陛 腎雖 彊紫煙 л
            direction2 = 0;
        }

        //3廓 縣葬漆檜攪
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

            printf("3廓 鱉蝓");
            if (index + 1 == 3) {   // 摹鷗脹 縣葬漆檜攪陛 1廓橾 陽
                printf("僥擊 殘戲衛啊蝗棲梱? (蕨: 1, 嬴棲螃: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("僥擊 殘蝗棲棻. 1蟾 �� 轎嫦м棲棻.\n");
                    Sleep(1000); // 1蟾 渠晦
                }
                else {
                    printf("僥擊 殘雖 彊蝗棲棻. 3蟾 �� 轎嫦м棲棻.\n");
                    Sleep(3000); // 3蟾 渠晦
                }
                ele.kg[2] += user_sex();
            }
            while (ele.kg[2] + weight[2] <= PASSENGER_KG) { // 鼠啪 薯и擊 蟾婁ж雖 彊擊 陽梱雖 鱉蝓濠蒂 蹺陛
                ele.kg[2] += weight[2];
                weight[2] = calculate_weight(random_gender()); // 棻擠 鱉蝓濠曖 鼠啪蒂 啗骯
            }
            Sleep(500);
            direction3 = (target_floor3 > ele.floor[2]) ? 1 : -1;
            user_floor3 = -11;
        }
        if (ele.floor[2] == target_floor3)
        {
            ele.kg[2] -= weight[2]; // 跡瞳雖縑 紫雜ж賊 鱉蝓濠蒂 頂葡
            if (ele.kg[2] < 0) ele.kg[2] = 0; // 鼠啪陛 擠熱陛 腎雖 彊紫煙 л
            direction3 = 0;
        }

        //4廓 縣葬漆檜攪
        if (ele.status[4] == 1)
        {
            printf("4廓 薄匐醞");
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
            printf("4廓 鱉蝓");
            if (index + 1 == 4) {   // 摹鷗脹 縣葬漆檜攪陛 1廓橾 陽
                printf("僥擊 殘戲衛啊蝗棲梱? (蕨: 1, 嬴棲螃: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("僥擊 殘蝗棲棻. 1蟾 �� 轎嫦м棲棻.\n");
                    Sleep(1000); // 1蟾 渠晦
                }
                else {
                    printf("僥擊 殘雖 彊蝗棲棻. 3蟾 �� 轎嫦м棲棻.\n");
                    Sleep(3000); // 3蟾 渠晦
                }
                ele.kg[3] += user_sex();
            }
            while (ele.kg[3] + weight[3] <= PASSENGER_KG) { // 鼠啪 薯и擊 蟾婁ж雖 彊擊 陽梱雖 鱉蝓濠蒂 蹺陛
                ele.kg[3] += weight[3];
                weight[3] = calculate_weight(random_gender()); // 棻擠 鱉蝓濠曖 鼠啪蒂 啗骯
            }
            Sleep(500);
            direction4 = (target_floor4 > ele.floor[3]) ? 1 : -1;
            user_floor4 = -11;
        }
        if (ele.floor[3] == target_floor4)
        {
            ele.kg[3] -= weight[3]; // 跡瞳雖縑 紫雜ж賊 鱉蝓濠蒂 頂葡
            if (ele.kg[3] < 0) ele.kg[3] = 0; // 鼠啪陛 擠熱陛 腎雖 彊紫煙 л
            direction4 = 0;
        }
        //5廓 縣葬漆檜攪
        weight[3] = calculate_weight(random_gender());
        if (ele.status[4] == 1)
        {
            printf("5廓 薄匐醞");
            elevatorStatus[4] = 3;
            user_floor5 = -11;
            direction5 = 0;
        }
        ele.floor[4] += direction5;

        if (ele.floor[4] == user_floor5)
        {

            printf("5廓 鱉蝓");
            if (index + 1 == 5) {   // 摹鷗脹 縣葬漆檜攪陛 1廓橾 陽
                printf("僥擊 殘戲衛啊蝗棲梱? (蕨: 1, 嬴棲螃: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("僥擊 殘蝗棲棻. 1蟾 �� 轎嫦м棲棻.\n");
                    Sleep(1000); // 1蟾 渠晦
                }
                else {
                    printf("僥擊 殘雖 彊蝗棲棻. 3蟾 �� 轎嫦м棲棻.\n");
                    Sleep(3000); // 3蟾 渠晦
                }
                ele.kg[4] += user_sex();
            }
            while (ele.kg[4] + weight[4] <= PASSENGER_KG) { // 鼠啪 薯и擊 蟾婁ж雖 彊擊 陽梱雖 鱉蝓濠蒂 蹺陛
                ele.kg[4] += weight[4];
                weight[4] = calculate_weight(random_gender()); // 棻擠 鱉蝓濠曖 鼠啪蒂 啗骯
            }
            Sleep(500);
            direction5 = (target_floor5 > ele.floor[4]) ? 1 : -1;
            user_floor5 = -11;
        }
        if (ele.floor[4] == target_floor5)
        {
            ele.kg[4] -= weight[4]; // 跡瞳雖縑 紫雜ж賊 鱉蝓濠蒂 頂葡
            if (ele.kg[4] < 0) ele.kg[4] = 0; // 鼠啪陛 擠熱陛 腎雖 彊紫煙 л
            direction5 = 0;
        }
        //6廓縣葬漆檜攪
        weight[3] = calculate_weight(random_gender());
        if (ele.status[5] == 1)
        {
            printf("6廓 薄匐醞");
            elevatorStatus[5] = 3;
            user_floor6 = -11;
            direction6 = 0;
        }
        ele.floor[5] += direction6;
        if (ele.floor[5] == user_floor6)
        {
            printf("6廓 鱉蝓");
            if (index + 1 == 6) {   // 摹鷗脹 縣葬漆檜攪陛 1廓橾 陽
                printf("僥擊 殘戲衛啊蝗棲梱? (蕨: 1, 嬴棲螃: 0): ");
                scanf("%d", &close_door);
                if (close_door) {
                    printf("僥擊 殘蝗棲棻. 1蟾 �� 轎嫦м棲棻.\n");
                    Sleep(1000); // 1蟾 渠晦
                }
                else {
                    printf("僥擊 殘雖 彊蝗棲棻. 3蟾 �� 轎嫦м棲棻.\n");
                    Sleep(3000); // 3蟾 渠晦
                }
                ele.kg[5] += user_sex();
            }
            while (ele.kg[5] + weight[5] <= PASSENGER_KG) { // 鼠啪 薯и擊 蟾婁ж雖 彊擊 陽梱雖 鱉蝓濠蒂 蹺陛
                ele.kg[5] += weight[5];
                weight[5] = calculate_weight(random_gender()); // 棻擠 鱉蝓濠曖 鼠啪蒂 啗骯
            }
            Sleep(500);
            direction6 = (target_floor6 > ele.floor[5]) ? 1 : -1;
            user_floor6 = -11;
        }
        if (ele.floor[5] == target_floor6)
        {
            ele.kg[5] -= weight[5]; // 跡瞳雖縑 紫雜ж賊 鱉蝓濠蒂 頂葡
            if (ele.kg[5] < 0) ele.kg[5] = 0; // 鼠啪陛 擠熱陛 腎雖 彊紫煙 л
            direction6 = 0;
        }
        Sleep(500);
        system("cls");
        display_elevator_status();


    }
}




int main()
{
    srand(time(NULL));  // 楠渾 衛萄 撲薑


    adminmode();
    for (int i = 0;i < 6;i++) {
        printf("%d\n", ele.status[i]);
    }


    while (1)
    {


        // 試盪 賅萇 高擊 楠渾ж啪 撲薑м棲棻.
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