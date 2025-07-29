#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define RED_COLOR   "\033[1;31m"
#define GREEN_COLOR "\033[1;32m"
#define RESET_COLOR "\033[0m"

struct User currentUser;
struct Bus currentBus;
struct Booking currentBooking;

#ifdef _WIN32
void setConsoleColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
#else
// Define color-changing functions for non-Windows systems if needed
void setConsoleColor(int color)
{
    // No color-changing logic for non-Windows systems
}
#endif

void printColoredSeat(int seatNumber, int isBooked)
{
    if (isBooked)
    {
        setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("  | %2d  |  ", seatNumber);
    }
    else
    {
        setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("  | %2d  |  ", seatNumber);
    }

    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

struct User
{
    char username[50];
    char password[50];
    struct User* next;
};

struct User* head=NULL;


struct User* createUser(char username[], char password[])
{
    struct User* newUser = (struct User*)malloc(sizeof(struct User));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->next = NULL;
    return newUser;
}

// Function to insert a User at the end of the linked list
void insertUser( char username[], char password[])
{
    struct User* newUser = createUser(username, password);
    if (head == NULL)
    {
        head = newUser;
        return;
    }
    struct User* temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newUser;
}

void start()
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return ;
    }

    char username[50];
    char password[50];

    // Read data from file and insert into linked list
    while (fscanf(file, "%s %s", username, password) != EOF)
    {
        insertUser( username, password);
    }

    fclose(file);

    return;
}

int searchNode( char username[], char password[])
{
    struct User* temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0)
        {
            return 1; // Node found
        }
        temp = temp->next;
    }
    return 0; // Node not found
}

struct Bus
{
    char from[50];
    char to[50];
    int totalSeats;
    int bookedSeats[40];
};

struct Booking
{
    char username[50];
    char password[50];
    int bookedSeats;
    int seatNo;
    char passengerName[50];
    int passengerAge;
    char passengerGender;
    char passengerPhoneNumber[15];
    struct Booking* next;
};

struct Booking* first=NULL;

struct Booking* createBooking(char username[], char password[],char passengerName[], int passengerAge, char passengerGender, char passengerPhoneNumber[], int seatNo)
{
    struct Booking* newBooking = (struct Booking*)malloc(sizeof(struct Booking));
    strcpy(newBooking->username, username);
    strcpy(newBooking->password, password);
    strcpy(newBooking->passengerName, passengerName);
    newBooking->passengerAge = passengerAge;
    newBooking->passengerGender = passengerGender;
    strcpy(newBooking->passengerPhoneNumber, passengerPhoneNumber);
    newBooking->seatNo = seatNo;
    newBooking->next = NULL;
    return newBooking;
}

void insertBooking( char username[], char password[],char passengerName[], int passengerAge, char passengerGender, char passengerPhoneNumber[], int seatNo)
{
    struct Booking* newBooking = createBooking(username, password, passengerName, passengerAge, passengerGender, passengerPhoneNumber, seatNo);
    if (first == NULL)
    {
        first = newBooking;
        return;
    }
    struct Booking* temp = first;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newBooking;
}

void startBooking()
{
    FILE *file = fopen("bookings.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return ;
    }
    char username[50], password[50], passengerName[50], passengerPhoneNumber[15];
    char passengerGender;
    int passengerAge, seatNo;
    // Read data from file and insert into linked list
    while (fscanf(file, "%49[^,], %49[^,], %49[^,], %d, %c, %14[^,], %d\n",username,password,passengerName,
                  &passengerAge, &passengerGender,
                  passengerPhoneNumber, &seatNo) ==7)
    {
        insertBooking(username, password, passengerName, passengerAge, passengerGender, passengerPhoneNumber, seatNo);
    }

    fclose(file);

    return;
}

void freeBookingList(struct Booking** first)
{
    struct Booking* current = *first;
    struct Booking* nextBooking;

    while (current != NULL)
    {
        nextBooking = current->next;
        free(current); // Deallocate memory for the current booking
        current = nextBooking;
    }

    *first = NULL; // Update the head pointer to indicate an empty list
}

void freeUserList(struct User** head)
{
    struct User* current = *head;
    struct User* nextUser;

    while (current != NULL)
    {
        nextUser = current->next;
        free(current); // Deallocate memory for the current user
        current = nextUser;
    }

    *head = NULL; // Update the head pointer to indicate an empty list
}

void print_first()
{
    for(int j=0; j<6; j++)
    {
        printf("  +-----+  ");
        if(j==2)
        {
            printf("    ||     ");
        }
        if(j==0 || j==3)
        {
            printf("    ");
        }
    }
    printf("\n");
}

void print_second()
{
    for(int j=0; j<6; j++)
    {
        printf("  |     |  ");
        if(j==2)
        {
            printf("    ||     ");
        }
        if(j==0 || j==3)
        {
            printf("    ");
        }
    }
    printf("\n");
}

void seat_no(int startValue, int bookedSeats[], int isBooked)
{
    for (int j = 0; j < 6; j++)
    {
        printColoredSeat(startValue++, bookedSeats[startValue - 1]);
        if (j == 2)
        {
            printf("    ||     ");
        }
        if (j == 0 || j == 3)
        {
            printf("    ");
        }
    }
    printf("\n");
}

void displayACSeats(int bookedSeats[])
{
    int start = 1;
    printf("\t\tLower Berth \t\t\t\t   Upper Berth\n\n");
    for (int i = 0; i < 5; i++)
    {
        print_first();
        print_second();
        seat_no(start, bookedSeats, 1);  // Pass 1 to indicate that seat is booked
        print_second();
        print_first();
        start += 6;
    }
}

void displayBusSeats()
{
    printf("AC Sleeper Bus Seats:\n");
    displayACSeats(currentBus.bookedSeats);
}

void displayStops()
{
    printf("Available Stops: Bengaluru, Chickmangaluru, Tumakuru, Mysuru, Mangaluru, Udupi, Kundapura, Davanagere\n");
}

void signUp()
{
    struct User newUser;
    printf("Enter your username: ");
    scanf("%s", newUser.username);
    printf("Enter your password: ");
    scanf("%s", newUser.password);
    insertUser(newUser.username,newUser.password);
    FILE *file = fopen("users.txt", "a");
    fprintf(file, "%s %s\n", newUser.username, newUser.password);
    fclose(file);
    printf("Account created successfully!\n");
}

void displayBookedSeats(struct Bus *bus)
{
    printf("\n");
    printf("BUS DETAILS: \n");
    printf("Booked Seats: ");

    FILE *file = fopen("seatList.txt", "r");

    if (file != NULL)
    {
        int seatNumber;
        int count=0;

        // Update bookedSeats array and count
        while (fscanf(file, "%d", &seatNumber) == 1)
        {
            printf("%d ", seatNumber);
            bus->bookedSeats[seatNumber - 1] = 1;
            count++;
        }

        fclose(file);
    }

    else
        printf("No seats booked yet.");


    printf("\n");

    displayBusSeats();
}


void bookTicket(struct User *currentUser, struct Bus *bus, struct Booking *booking, char username[], char password[])
{
    int numSeats;
    displayBookedSeats(bus);
    struct User* current = head;

    printf("Enter the number of seats you want to book: ");
    scanf("%d", &numSeats);

    for (int i = 0; i < numSeats; i++)
    {
        int seatNumber;
        do
        {
            printf("Enter seat number for seat %d: ", i + 1);
            scanf("%d", &seatNumber);

            if (seatNumber > 30 || seatNumber < 1 || bus->bookedSeats[seatNumber - 1] == 1)
            {
                printf("Enter a valid and available seat number.\n");
            }
        }
        while (seatNumber > 30 || seatNumber < 1 || bus->bookedSeats[seatNumber - 1] == 1);

        bus->bookedSeats[seatNumber - 1] = 1;
        booking->seatNo = seatNumber;

        // Prompt for passenger details for each seat booked
        printf("Enter passenger name for seat %d: ", i + 1);
        scanf(" %[^\n]", booking->passengerName);

        printf("Enter passenger age for seat %d: ", i + 1);
        scanf("%d", &booking->passengerAge);

        printf("Enter passenger gender (M/F) for seat %d: ", i + 1);
        scanf(" %c", &booking->passengerGender);

        printf("Enter passenger phone number for seat %d: ", i + 1);
        scanf("%s", booking->passengerPhoneNumber);


        while (current != NULL)
        {
            if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0)
            {
                strcpy(booking->username, current->username);
                strcpy(booking->password, current->password);
            }
            current = current->next;
        }

        // Save booking details to file
        FILE *bookingFile = fopen("bookings.txt", "a");
        fprintf(bookingFile, "%s, %s, %s, %d, %c, %s, %d\n", booking->username, booking->password, booking->passengerName,
                booking->passengerAge, booking->passengerGender,
                booking->passengerPhoneNumber, booking->seatNo);
        fclose(bookingFile);

        FILE *bookedSeats = fopen("seatList.txt", "a");
        fprintf(bookedSeats, "%d ", booking->seatNo);
        fclose(bookedSeats);

        startBooking();

        // Display booking preview
        printf("Booking successful! Details:\n");
        printf("Seat %d - Passenger: %s, Age: %d, Gender: %c, Phone: %s\n", booking->seatNo, booking->passengerName,
               booking->passengerAge, booking->passengerGender, booking->passengerPhoneNumber);
    }
}

void cancelTicket(struct User *currentUser, struct Booking *booking, struct Bus *bus, char uname[])
{
    FILE *bookingFile = fopen("bookings.txt", "r");
    FILE *tempFile = fopen("temp_bookings.txt", "w");
    FILE *seatlist = fopen("seatList.txt", "r");
    FILE *templist = fopen("temp_lists.txt", "w");

    if (seatlist == NULL)
    {
        printf("No seats booked.\n");
        return;
    }

    int seatToCancel;

    do
    {
        printf("Enter the seat number to cancel (between 1 and 30): ");
        scanf("%d", &seatToCancel);

        if (seatToCancel < 1 || seatToCancel > 30)
        {
            printf("Invalid seat number. Please enter a seat number between 1 and 30.\n");
        }
        else
        {
            break;  // Exit the loop if a valid seat number is entered
        }
    }
    while (1);

    int found = 0;

    while (fscanf(bookingFile, "%49[^,], %49[^,], %49[^,], %d, %c, %14[^,], %d ",
                  booking->username, booking->password, booking->passengerName,
                  &booking->passengerAge, &booking->passengerGender, booking->passengerPhoneNumber,
                  &booking->seatNo) == 7)
    {
        // Check if the current line belongs to the logged-in user and contains the seat to cancel
        if (strcmp(uname, booking->username) == 0 && booking->seatNo == seatToCancel)
        {
            found = 1;  // Seat to cancel found
            printf("Cancelled Booking for seat Number: %d\n", seatToCancel);
        }
        else
        {
            // Write the current line to the temporary file
            fprintf(tempFile, "%s, %s, %s, %d, %c, %s, %d\n",
                    booking->username, booking->password, booking->passengerName,
                    booking->passengerAge, booking->passengerGender, booking->passengerPhoneNumber,
                    booking->seatNo);

            int bookedSeat;
            // Read the booked seats from the seatList file
            while (fscanf(seatlist, "%d", &bookedSeat) == 1)
            {
                // Write the booked seat to the temporary list file only if it's not the seat to be canceled
                if (bookedSeat != seatToCancel)
                {
                    fprintf(templist, "%d ", bookedSeat);
                }
            }
        }
    }

    fclose(bookingFile);
    fclose(tempFile);
    fclose(seatlist);
    fclose(templist);

    if (!found)
    {
        printf("Seat %d not found in your bookings.\n", seatToCancel);
        remove("temp_bookings.txt");
        remove("temp_lists.txt");  // Remove the temporary file
        return;
    }

    // Rename the temporary file to overwrite the original bookings file
    if (remove("bookings.txt") == 0 && rename("temp_bookings.txt", "bookings.txt") == 0
            && remove("seatList.txt") == 0 && rename("temp_lists.txt", "seatList.txt") == 0)
    {
        printf("Cancellation successful.\n");
        bus->bookedSeats[seatToCancel - 1] = 0;  // Update bookedSeats array
        displayBookedSeats(bus);  // Display updated bus seats
    }
    else
    {
        printf("Error cancelling the seat. Please try again.\n");
    }
    //free the link list & call startBooking() to ensure that if a seat is cancelled, it gets updated.
    freeBookingList(&first);
    startBooking();
}


void showTickets(struct Booking *booking, char username[])
{
    //free the link list & call startBooking() to ensure that if a seat is cancelled, it gets updated while showing tickets.
    freeBookingList(&first);
    startBooking();
    struct Booking *temp = first;
    int nodeCount = 0;

    while(temp!=NULL)
    {
        if (strcmp(temp->username, username) == 0)
        {
            printf("-------------------------\n");
            printf("Seat Number: %d\n", temp->seatNo);
            printf("Name: %s\n", temp->passengerName);
            printf("Age: %d\n", temp->passengerAge);
            printf("Gender: %c\n", temp->passengerGender);
            printf("Phone No: %s\n", temp->passengerPhoneNumber);
            printf("-------------------------\n");

            nodeCount++;
        }
        temp=temp->next;
    }


    if (nodeCount == 0)
    {
        printf("No tickets found for the current user.\n");
    }
}


int main()
{
    start();
    char choice;
    int loggedIn = 0;
    char name[50];
    char pass[50];
    while (1)
    {
        if (!loggedIn)
        {
            printf("Welcome to the Bus Reservation System!\n");
            printf("1. Login\n");
            printf("2. Sign Up\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");
            scanf(" %c", &choice);

            switch (choice)
            {
            case '1':

                printf("Enter your username: ");
                scanf("%s", name);
                printf("Enter your password: ");
                scanf("%s", pass);

                if (searchNode(name,pass))
                {
                    printf("Login successful!\n");
                    loggedIn = 1;
                }
                else
                {
                    printf("Invalid username or password. Please try again.\n");
                }
                break;

            case '2':
                signUp();
                break;

            case '3':
                printf("Exiting the program.\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
            }
        }
        else
        {
            printf("1. Book Ticket\n");
            printf("2. Cancel Ticket\n");
            printf("3. Show Tickets\n");
            printf("4. Logout\n");
            printf("Enter your choice: ");
            scanf(" %c", &choice);

            switch (choice)
            {
            case '1':
                displayStops();
                printf("Enter the from stop: ");
                scanf("%s", currentBus.from);
                printf("Enter the to stop: ");
                scanf("%s", currentBus.to);
                bookTicket(&currentUser, &currentBus, &currentBooking, name, pass);
                break;

            case '2':
                cancelTicket(&currentUser, &currentBooking, &currentBus,name);
                break;

            case '3':
                showTickets(&currentBooking, name);
                break;

            case '4':
                printf("Logging out.\n");
                loggedIn = 0;
                freeUserList(&head);
                freeBookingList(&first);
                break;

            default:
                printf("Invalid choice. Please try again.\n");
            }
        }
    }

    return 0;
}
