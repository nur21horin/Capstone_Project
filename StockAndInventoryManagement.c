#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

typedef struct {
    int itemno;
    char iname[50];
    float price, dis;
    int stock;
} Item;

FILE *f;
//user structure for registration
// User structure for registration
typedef struct {
    char username[50];
    char password[50];
} User;

void register_user() {
    FILE *f = fopen("users.dat", "ab");  // Open file in append mode
    if (f == NULL) {
        printf("Error opening file.\n");
        return;
    }

    User user;
    printf("Enter username: ");
    getchar(); // Consume the newline character left by previous input
    fgets(user.username, sizeof(user.username), stdin);
    user.username[strcspn(user.username, "\n")] = '\0';  // Remove newline character from the string

    printf("Enter password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = '\0';  // Remove newline character from the string

    fwrite(&user, sizeof(User), 1, f);  // Save user data to the file
    fclose(f);

    printf("Registration Successful!\n");
}
int login() {
    FILE *f = fopen("users.dat", "rb");  // Open file in read mode
    if (f == NULL) {
        printf("No users found. Please register first.\n");
        return 0;  // Return failure
    }

    char username[50], password[50];
    printf("Enter username: ");
    getchar();  // Consume newline character
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';  // Remove newline

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';  // Remove newline

    User user;
    while (fread(&user, sizeof(User), 1, f)) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            fclose(f);
            printf("Login Successful!\n");
            return 1;  // Successful login
        }
    }

    fclose(f);
    printf("Invalid username or password.\n");
    return 0;  // Login failed
}

// Delay function
void delay(int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

// Gotoxy function for console cursor positioning
void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Draw border function
void drawBorder(int a, int b, int length, int height) {
    gotoxy(a, b);
    for (int i = 0; i < length; i++) {
        printf("%c", 219);
    }
    for (int i = 1; i < height; i++) {
        gotoxy(a, b + i);
        printf("%c", 219);
    }
    for (int i = 0; i < length; i++) {
        gotoxy(a, b + height);
        printf("%c", 219);
    }
    for (int i = 1; i < height; i++) {
        gotoxy(a + length, b + i);
        printf("%c", 219);
    }
}

// Get item data from user
void getdata(Item *i) {
    printf("\nEnter Item Number: ");
    scanf("%d", &i->itemno);
    getchar();
    printf("Enter Item Name: ");
    fgets(i->iname, sizeof(i->iname), stdin);
    i->iname[strcspn(i->iname, "\n")] = '\0';
    printf("Enter Item Price: ");
    scanf("%f", &i->price);
    printf("Enter Discount (%%): ");
    scanf("%f", &i->dis);
}

// Display item data
void showdata(Item i) {
    printf("\nItem Number: %d\nItem Name: %s\nPrice: %.2f\nDiscount: %.2f%%\n",i.itemno, i.iname, i.price, i.dis);
}

// Write item to file
void write_to_file() {
    f = fopen("store.dat", "ab");
    if (!f) {
        perror("Error opening file");
        return;
    }
    Item i;
    getdata(&i);
    fwrite(&i, sizeof(Item), 1, f);
    fclose(f);
    printf("\nItem has been created.\n");
}

// Display all records
void disp_all() {
    f = fopen("store.dat", "rb");
    if (!f) {
        perror("Error opening file");
        return;
    }
    Item i;
    printf("\n\nDisplaying All Records:\n");
    while (fread(&i, sizeof(Item), 1, f)) {
        showdata(i);
        printf("-------------------------------\n");
    }
    fclose(f);
}

// Display specific record
void display_sp(int n) {
    int found = 0;
    f = fopen("store.dat", "rb");
    if (!f) {
        perror("Error opening file");
        return;
    }
    Item i;
    while (fread(&i, sizeof(Item), 1, f)) {
        if (i.itemno == n) {
            showdata(i);
            found = 1;
            break;
        }
    }//NUR MOhammod
    fclose(f);
    if (!found) {
        printf("\nRecord does not exist.\n");
    }
}

// Modify a record
void modify() {
    int no, found = 0;
    printf("\nEnter the Item Number to Modify: ");
    scanf("%d", &no);

    f = fopen("store.dat", "rb+");
    if (!f) {
        perror("Error opening file");
        return;
    }
    Item i;
    while (fread(&i, sizeof(Item), 1, f)) {
        if (i.itemno == no) {
            printf("\nCurrent Record Details:\n");
            showdata(i);
            printf("\nEnter New Details:\n");
            getdata(&i);
            fseek(f, -sizeof(Item), SEEK_CUR);
            fwrite(&i, sizeof(Item), 1, f);
            printf("\nRecord Updated Successfully.\n");
            found = 1;
            break;
        }
    }
    fclose(f);
    if (!found) {
        printf("\nRecord Not Found.\n");
    }
}

// Delete a record
void deleterec() {
    int no;
    printf("\nEnter the Item Number to Delete: ");
    scanf("%d", &no);

    f = fopen("store.dat", "rb");
    if (!f) {
        perror("Error opening file");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        perror("Error creating temporary file");
        fclose(f);
        return;
    }

    Item i;
    while (fread(&i, sizeof(Item), 1, f)) {
        if (i.itemno != no) {
            fwrite(&i, sizeof(Item), 1, temp);
        }
    }
    fclose(f);
    fclose(temp);

    remove("store.dat");
    rename("temp.dat", "store.dat");
    printf("\nRecord Deleted Successfully.\n");
}

// Password check for admin
/*int pass2() {
    char password[25], temp;
    const char correctpass[] = "DIU089";
    int f = 0;

    drawBorder(5, 3, 100, 24);
    gotoxy(40, 12);
    printf("Enter the password: ");

    while (1) {
        temp = getch();
        if (temp == 13) break;
        if (temp == 8 && f) {
            printf("\b \b");
            f--;
        } else {
            password[f++] = temp;
            printf("*");
        }
    }
    password[f] = '\0';
    return strcmp(correctpass, password) == 0;
}*/
void place_order();
void buy();
void sell();
//void profit();
void profit(Item *item, int qtySold, float sellingPrice);

// Function to buy items and add them to inventory
void buy(Item *item) {
    printf("Enter Item Number: ");
    scanf("%d", &item->itemno);
    printf("Enter Item Name: ");
    getchar();  // to consume the newline left by previous input
    fgets(item->iname, sizeof(item->iname), stdin);
    item->iname[strcspn(item->iname, "\n")] = '\0';  // Remove newline character from the string
    printf("Enter Purchase Price: ");
    scanf("%f", &item->price);
    printf("Enter Discount (%%): ");
    scanf("%f", &item->dis);
    printf("Enter Stock Quantity: ");
    scanf("%d", &item->stock);
    
    // Writing to a file (inventory file)
    f = fopen("inventory.txt", "a");
    if (f == NULL) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(f, "%d %s %.2f %.2f %d\n", item->itemno, item->iname, item->price, item->dis, item->stock);
    fclose(f);
    printf("Item Purchased and Added to Inventory Successfully!\n");
}

// Function to sell items and calculate profit
void sell(Item *item) {
    int sellQty;
    printf("Enter Item Number to Sell: ");
    scanf("%d", &item->itemno);
    
    // Read inventory file and find the item
    f = fopen("inventory.txt", "r+");
    if (f == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    int found = 0;
    while (fscanf(f, "%d %s %f %f %d\n", &item->itemno, item->iname, &item->price, &item->dis, &item->stock) != EOF) {
        if (item->itemno == item->itemno) {
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Item not found in inventory.\n");
        fclose(f);
        return;
    }

    printf("Enter Quantity to Sell: ");
    scanf("%d", &sellQty);
    
    if (sellQty > item->stock) {
        printf("Not enough stock available.\n");
        fclose(f);
        return;
    }
    
    // Calculate selling price after discount
    float sellingPrice = item->price * (1 - item->dis / 100);
    printf("Selling Price after Discount: %.2f\n", sellingPrice);
    
    // Update stock and write to file
    item->stock -= sellQty;
    long pos = ftell(f);
    fseek(f, pos - sizeof(Item), SEEK_SET);
    fprintf(f, "%d %s %.2f %.2f %d\n", item->itemno, item->iname, item->price, item->dis, item->stock);
    fclose(f);
    
    printf("Item Sold Successfully!\n");
    
    // Call profit function to calculate profit from this transaction
    profit(item, sellQty, sellingPrice);
}

// Function to calculate and display profit
void profit(Item *item, int qtySold, float sellingPrice) {
    float totalRevenue = sellingPrice * qtySold;
    float costPrice = item->price * qtySold;
    float profit = totalRevenue - costPrice;

    printf("Total Revenue from Sale: %.2f\n", totalRevenue);
    printf("Total Cost Price: %.2f\n", costPrice);
    printf("Profit from Sale: %.2f\n", profit);
}
// Admin menu
void admin_menu() {
    int choice;
    //if (!pass2()) {
    if(!login()){
        printf("\nIncorrect Password. Returning to Main Menu...\n");
        delay(2000);
        return;
    }

    do {
        printf("\nAdmin Menu:");
        printf("\n1. Create Item");
        printf("\n2. Display All Items");
        printf("\n3. Query Item");
        printf("\n4. Modify Item");
        printf("\n5. Delete Item");
        printf("\n6.Buy product");
        printf("\n7.Sell Product");
        printf("\n8.Profit : ");
        printf("\n9. Create Invoice");
        printf("\n10. Back to Main Menu");
        printf("\nEnter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: write_to_file(); break;
            case 2: disp_all(); break;
            case 3: {
                int num;
                printf("\nEnter Item Number to Query: ");
                scanf("%d", &num);
                display_sp(num);
                break;
            }
            case 4: modify(); break;
            case 5: deleterec(); break;
            case 6:{
                Item item;
                buy(&item);
                break;
            }
            case 7:{
                Item item;
                sell(&item);
                break;
            }
            case 8:{
                Item item;
                int qtySold = 10;  // Example: specify qty sold
                float sellingPrice = 20.0;  // Example: specify selling price
                profit(&item, qtySold, sellingPrice);  // Pass appropriate arguments
                break;
            }
            case 9:place_order(); break;
            case 10: return;
            default: printf("\nInvalid Choice! Try again.\n");
        }
    } while (choice != 10);
}

// Place an order
void place_order() {
    int order_arr[50], quan[50], count = 0;
    char ch;

    disp_all();
    printf("\nPlace Your Order:\n");
    do {
        printf("\nEnter Item Number: ");
        scanf("%d", &order_arr[count]);
        printf("Enter Quantity: ");
        scanf("%d", &quan[count]);
        count++;
        printf("Do you want to order another item? (y/n): ");
        scanf(" %c", &ch);
    } while (ch == 'y' || ch == 'Y');

    printf("\nGenerating Invoice...\n");
    float total = 0;
    printf("Item No\tName\tQuantity\tPrice\tAmount\tAmount after Discount\n");

    for (int x = 0; x < count; x++) {
        f = fopen("store.dat", "rb");
        if (!f) {
            perror("Error opening file");
            return;
        }
        Item i;
        while (fread(&i, sizeof(Item), 1, f)) {
            if (i.itemno == order_arr[x]) {
                float amt = i.price * quan[x];
                float damt = amt - (amt * i.dis / 100);
                total += damt;
                printf("%d\t%s\t%d\t%.2f\t%.2f\t%.2f\n", i.itemno, i.iname, quan[x], i.price, amt, damt);
            }
        }
        fclose(f);
    }
    printf("\nTotal: %.2f\n", total);
}

// Main menu
void menu() {
    int choice;
    do {
        printf("\nMain Menu:");
       // printf("\n1. Register");
        //printf("\n2. Login");
        printf("\n1.Product Purchase:");
        printf("\n2. Admin");
        printf("\n3. Exit");
        printf("\nEnter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            //case 1:register_user();break;
            //case 2:{
              //  if(login()){
                //    printf("Welcome to the Alo Stor\n");
                //}
                //break;
            //}
            case 1: place_order(); break;
            case 2: admin_menu(); break;
            case 3: exit(0);
            default: printf("\nInvalid Choice! Try again.\n");
        }
    } while (choice != 3);
}

// Main function
int main() {
    int choice;
    do{
    printf("1.Registration\n");
    printf("2.Login\n");
    printf("3.Exit");
    printf("\nEnter the choice: ");
    
    scanf("%d",&choice);
    switch(choice){
        case 1:register_user();break;
        case 2:{
                if(login()){
                    printf("Welcome to the Alo Stor\n");
                    menu();   
                }else{
                    printf("\n Incorrect Password . Returning to main menu");
                    delay(2000);
                }
                break;
            }
        case 3:{
             exit(0);
             printf("Good Bye\n");
        }
        default: printf("\nInvalid Choice! Try again.\n");
    }

    }while(choice!=3 || !login());
    
    return 0;
}
