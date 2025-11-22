#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Let's keep it simple: max 10 bills and 10 feedbacks
#define MAX_BILLS 10
#define MAX_FEEDBACK 10
#define MAX_SNACKS 5
#define NAME_SIZE 50
#define NOTE_SIZE 256

// Snack box (our yummy menu)
typedef struct {
    char name[NAME_SIZE];
    float price;
} Snack;

// Bill box (what you bought)
typedef struct {
    int id;
    char customer_name[NAME_SIZE];
    int counts[MAX_SNACKS];  // How many of each snack
    float total;
    char date[20];
} Bill;

// Feedback box (happy notes)
typedef struct {
    int id;
    char customer_name[NAME_SIZE];
    int stars;  // 1 to 5
    char note[NOTE_SIZE];
    char date[20];
} Feedback;

// Our global treasures (lists)
Snack snacks[MAX_SNACKS];
Bill bills[MAX_BILLS];
int bill_count = 0;
Feedback feedbacks[MAX_FEEDBACK];
int feedback_count = 0;

// Helper: Get today's date like a calendar sticker
char* get_date() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    static char date_str[20];
    strftime(date_str, 20, "%Y-%m-%d", tm_info);
    return date_str;
}

// Step 1: Fill the snack menu (your faves!)
void setup_snacks() {
    strcpy(snacks[0].name, "Cookie");
    snacks[0].price = 2.0;
    strcpy(snacks[1].name, "Juice");
    snacks[1].price = 1.5;
    strcpy(snacks[2].name, "Apple");
    snacks[2].price = 1.0;
    strcpy(snacks[3].name, "Candy");
    snacks[3].price = 3.0;
    strcpy(snacks[4].name, "Ice Cream");
    snacks[4].price = 4.0;
}

// Step 2: Show the adventure menu
void show_menu() {
    printf("\n=== Timmy's Snack Shop Adventure! ===\n");
    printf("1. Sell snacks (make a bill)\n");
    printf("2. See old bills (treasure list)\n");
    printf("3. Get a happy note (feedback)\n");
    printf("4. Read happy notes\n");
    printf("5. Bye bye! (save and go)\n");
    printf("Pick a number: ");
}

// Step 3: Make a bill (play store!)
void make_bill() {
    if (bill_count >= MAX_BILLS) {
        printf("Whoa, too many bills! Come back later.\n");
        return;
    }

    Bill *this_bill = &bills[bill_count];
    this_bill->id = bill_count + 1;

    printf("What's your name, explorer? ");
    fgets(this_bill->customer_name, NAME_SIZE, stdin);
    this_bill->customer_name[strcspn(this_bill->customer_name, "\n")] = 0;  // Clean up

    float money = 0.0;
    for (int i = 0; i < MAX_SNACKS; i++) {
        printf("How many %s ($%.2f each)? ", snacks[i].name, snacks[i].price);
        scanf("%d", &this_bill->counts[i]);
        getchar();  // Clean up
        money += this_bill->counts[i] * snacks[i].price;
    }

    float tax = money * 0.1;  // Tiny extra for the piggy bank
    this_bill->total = money + tax;
    strcpy(this_bill->date, get_date());

    // Show the receipt!
    printf("\n--- Your Magic Receipt! ---\n");
    printf("ID: %d | Explorer: %s\n", this_bill->id, this_bill->customer_name);
    printf("Before tax: $%.2f\n", money);
    printf("Tax fun: $%.2f\n", tax);
    printf("Total treasure: $%.2f\n", this_bill->total);
    printf("Date: %s\n", this_bill->date);

    bill_count++;
    printf("Yay! Bill saved in the diary.\n");
    getchar();  // Pause like a high five
}

// Step 4: See old bills
void see_bills() {
    if (bill_count == 0) {
        printf("No bills yet! Let's make one.\n");
        getchar();
        return;
    }

    printf("\n--- Treasure Chest of Bills! ---\n");
    for (int i = 0; i < bill_count; i++) {
        printf("ID: %d | %s owes $%.2f (on %s)\n",
               bills[i].id, bills[i].customer_name, bills[i].total, bills[i].date);
    }
    getchar();
}

// Step 5: Get a happy note
void add_feedback() {
    if (feedback_count >= MAX_FEEDBACK) {
        printf("Too many notes! We'll read them soon.\n");
        return;
    }

    Feedback *this_note = &feedbacks[feedback_count];
    this_note->id = feedback_count + 1;

    printf("Your name? ");
    fgets(this_note->customer_name, NAME_SIZE, stdin);
    this_note->customer_name[strcspn(this_note->customer_name, "\n")] = 0;

    printf("Stars (1-5, like rating a cartoon)? ");
    scanf("%d", &this_note->stars);
    getchar();

    printf("What did you like? (type a sentence) ");
    fgets(this_note->note, NOTE_SIZE, stdin);
    this_note->note[strcspn(this_note->note, "\n")] = 0;

    strcpy(this_note->date, get_date());

    printf("\n--- Your Super Note! ---\n");
    printf("ID: %d | %s gives %d stars!\n", this_note->id, this_note->customer_name, this_note->stars);
    printf("Note: %s\n", this_note->note);
    printf("Date: %s\n", this_note->date);

    feedback_count++;
    printf("Thanks! Note stuck on the fridge.\n");
    getchar();
}

// Step 6: Read happy notes
void see_feedback() {
    if (feedback_count == 0) {
        printf("No notes yet! Share one.\n");
        getchar();
        return;
    }

    printf("\n--- Fridge of Happy Notes! ---\n");
    int total_stars = 0;
    for (int i = 0; i < feedback_count; i++) {
        printf("ID: %d | %s: %d stars - '%s' (on %s)\n",
               feedbacks[i].id, feedbacks[i].customer_name, feedbacks[i].stars,
               feedbacks[i].note, feedbacks[i].date);
        total_stars += feedbacks[i].stars;
    }
    float avg = (float)total_stars / feedback_count;
    printf("Average stars: %.1f/5 - We're doing great!\n", avg);
    getchar();
}

// Step 7: Save to diary (files!)
void save_diary() {
    // Save bills
    FILE *bill_book = fopen("bills_diary.txt", "w");
    if (bill_book) {
        for (int i = 0; i < bill_count; i++) {
            fprintf(bill_book, "%d|%s|%.2f|%s\n",
                    bills[i].id, bills[i].customer_name, bills[i].total, bills[i].date);
            // Note: Counts not saved here for simplicity - add if you want!
        }
        fclose(bill_book);
    }

    // Save notes
    FILE *note_book = fopen("notes_diary.txt", "w");
    if (note_book) {
        for (int i = 0; i < feedback_count; i++) {
            fprintf(note_book, "%d|%s|%d|%s|%s\n",
                    feedbacks[i].id, feedbacks[i].customer_name, feedbacks[i].stars,
                    feedbacks[i].note, feedbacks[i].date);
        }
        fclose(note_book);
    }
    printf("Diary saved! Your adventures are safe.\n");
}

// Load from diary (on start)
void load_diary() {
    // Load bills (simple version)
    FILE *bill_book = fopen("bills_diary.txt", "r");
    if (bill_book) {
        char line[256];
        while (fgets(line, 256, bill_book) && bill_count < MAX_BILLS) {
            sscanf(line, "%d|%[^|]|%f|%s",
                   &bills[bill_count].id, bills[bill_count].customer_name,
                   &bills[bill_count].total, bills[bill_count].date);
            // Counts set to 0 for now
            memset(bills[bill_count].counts, 0, sizeof(bills[bill_count].counts));
            bill_count++;
        }
        fclose(bill_book);
    }

    // Load notes
    FILE *note_book = fopen("notes_diary.txt", "r");
    if (note_book) {
        char line[512];  // Bigger for notes
        while (fgets(line, 512, note_book) && feedback_count < MAX_FEEDBACK) {
            // Simple sscanf, assumes no | in names/notes (add strtok for fancy!)
            sscanf(line, "%d|%[^|]|%d|%[^|]|%s",
                   &feedbacks[feedback_count].id, feedbacks[feedback_count].customer_name,
                   &feedbacks[feedback_count].stars, feedbacks[feedback_count].note,
                   feedbacks[feedback_count].date);
            feedback_count++;
        }
        fclose(note_book);
    }
}

// The big main adventure!
int main() {
    setup_snacks();  // Fill the menu
    load_diary();    // Remember old stuff

    int pick;
    do {
        show_menu();
        scanf("%d", &pick);
        getchar();  // Clean up

        if (pick == 1) {
            make_bill();
        } else if (pick == 2) {
            see_bills();
        } else if (pick == 3) {
            add_feedback();
        } else if (pick == 4) {
            see_feedback();
        } else if (pick == 5) {
            printf("High five! Thanks for playing.\n");
            save_diary();
        } else {
            printf("Oops, that's not a magic number. Try again!\n");
            getchar();
        }
    } while (pick != 5);

    return 0;  // Adventure ends, yay!
}