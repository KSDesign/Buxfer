#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Add a group with name group_name to the group_list referred to by 
* group_list_ptr. The groups are ordered by the time that the group was 
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.) 
*/
int add_group(Group **group_list_ptr, const char *group_name) {
    //Returns -1 if group already contains the user name
    if (find_group(*group_list_ptr, group_name) != NULL)
    {
        return -1;
    }
    else
    {
        //Allocates memory
        Group *newGroup = malloc(sizeof(Group));
        //Error printed if creating pointer does not work
        if (newGroup == NULL)
        {
            perror("Could not provide memory.");
            exit(0);
        }
        //Allocates memory for name
        int nameLength = strlen(group_name);
        newGroup->name = malloc(nameLength + 1);
        //Error printed if name not set properly
        if (newGroup->name == NULL)
        {
            perror("Could not provide memory.");
            exit(0);
        }
        strncpy(newGroup->name, group_name, nameLength);
        //New group points to nothing since it has no info
        newGroup->users = NULL;
        newGroup->xcts = NULL;
        newGroup->next = NULL;
        
        //Group list pointer points to the new group if there is nothing in the list
        if(*group_list_ptr == NULL)
        {
            *group_list_ptr = newGroup;
            return 0;
        }
        else
        {
            //New group is added to the end of the list
            Group *current = *group_list_ptr;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newGroup;
            return 0;
        }
    }
    
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
    Group *current = group_list;
    //Output if the group list is empty
    if (group_list == NULL) {
        printf("The list is empty.\n");
    }
    //Outputs group name while group list is not empty
    while (current != NULL) {
        printf("Group name: %s\n", current->name);
        current = current->next;
    }

}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the 
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
    Group *current = group_list;
    //Performs actions while group list is not empty
    while (current != NULL){
        //Compares the name of the group with the provided group name
        if (strcmp(current->name, group_name) == 0)
        {
            return current;
        }
        //Points to next in group list
        else
        {
            current = current->next;
        }
    }
    return NULL;
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
    //Returns -1 if user already contains the user name
    if (find_prev_user(group, user_name) != NULL)
    {
        return -1;
    }
    else
    {
        //Allocates memory for new user
        User *newUser = malloc(sizeof(User));
        //Error printed if creating pointer does not work
        if (newUser == NULL)
        {
            perror("Could not provide memory.");
            exit(0);
        }
        //Allocates memory for name
        int nameLength = strlen(user_name);
        newUser->name = malloc(nameLength + 1);
        //Error printed if name not set properly
        if (newUser->name == NULL)
        {
            perror("Could not provide memory.");
            exit(0);
        }
        strncpy(newUser->name, user_name, nameLength);
        //New user points to nothing since it has no info
        newUser->balance = 0.00;
        newUser->next = NULL;
        
        //If there are no users, the new user becomes start of the list
        if(group->users == NULL)
        {
            group->users = newUser;
            return 0;
        }
        else
        {
            //New user is added to the front of the list since it has smallest balance
            newUser->next = group->users;
            group->users = newUser;
            return 0;
        }
    }
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list. 
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you 
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
    User *prevUser = find_prev_user(group, user_name);
    User *userToRemove;
    //Returns -1 if user does not exist
    if (find_prev_user(group, user_name) == NULL)
    {
        return -1;
    }
    else
    {
        //Checks to see if the first in the list is the matching user
        if (strcmp(prevUser->name, user_name) == 0)
        {
            userToRemove = prevUser;
            if(prevUser->next != NULL)
            {
                group->users = prevUser->next;
            }
            else
            {
                group->users = NULL;
            }
        }
        //Performs action if matching user is not first in the list
        else
        {
            userToRemove = prevUser->next;
            prevUser->next = userToRemove->next;
        }
        //the user that is being removed points to nothing
        userToRemove->next = NULL;
        //Transactions removed and space freed
        remove_xct(group,user_name);
        free(userToRemove);
        return 0;
    }

}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely 
* lowest payer first.
*/
void list_users(Group *group) {
    User *currentUser =  group->users;
    //Performs actions if the currentUser exists
    if (currentUser != NULL)
    {
        //Prints the user names while currentUser exists
        while (currentUser != NULL) {
            printf("User name: %s User balance: $%.2f\n", currentUser->name, currentUser->balance);
            user_balance(group,currentUser->name);
            currentUser = currentUser->next;
        }
    }
    //Output if there are no users in the group
    else
    {
        printf("No users exist.\n");
    }
}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {
    User *prevUser = find_prev_user(group, user_name);
    //Returns -1 if user does not exist
    if (find_prev_user(group, user_name) == NULL)
    {
        return -1;
    }
    else
    {
        //Checks to see if the first in the list is the matching user
        if (strcmp(prevUser->name, user_name) == 0)
        {
            printf("The balance of User %s is $%.2f.\n", user_name, prevUser->balance);
            return 0;
        }
        //Output if the user is not first in the list
        else
        {
            printf("The balance of User %s is $%.2f.\n", user_name, prevUser->next->balance);
            return 0;
        }
    }
}

/* Print to standard output the name of the user who has paid the least 
* If there are several users with equal least amounts, all names are output. 
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance). 
*/
int under_paid(Group *group) {
    User *currentUser =  group->users;
    if (currentUser == NULL)
    {
        //Return -1 if the list of users is empty
        return -1;
    }
    //Performs actions if the currentUser exists
    else
    {
        //Sets the the user who least paid as the first user
        User *inDebtUser = currentUser;
        while (currentUser != NULL) {
            //Checks to see if the next user paid less or equal to the "inDebtUser"
            if (currentUser->balance <= inDebtUser->balance) {
                printf("%s paid $%.2f.\n", currentUser->name, currentUser->balance);
            }
            currentUser = currentUser->next;
        }
        return 0;
    }
}

/* Return a pointer to the user prior to the one in group with user_name. If 
* the matching user is the first in the list (i.e. there is no prior user in 
* the list), return a pointer to the matching user itself. If no matching user 
* exists, return NULL. 
*
* The reason for returning the prior user is that returning the matching user 
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
    User *currentUser = group->users;
    User *prevUser;
    //Performs actions if the currentUser exists
    if (currentUser != NULL)
    {
        //If matching user is the first user
        if (strcmp(currentUser->name, user_name) == 0)
        {
            return currentUser;
        }
        //If matching user is not the first user
        else
        {
            prevUser = currentUser;
            currentUser = currentUser->next;
            //Compares if currentuser matches name, then returns the previous user
            while (currentUser != NULL) {
                if (strcmp(currentUser->name, user_name) == 0)
                {
                    return prevUser;
                }
                prevUser = currentUser;
                currentUser = currentUser->next;
            }
        }
        return NULL;
    }
    else
    {
        return NULL;
    }
}

/* Add the transaction represented by user_name and amount to the appropriate 
* transaction list, and update the balances of the corresponding user and group. 
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {

    User *prevUser = find_prev_user(group, user_name);
    User *userToAdjust;
    User *userAfter;
    User *firstUser;
    User *newUserAfter;
    //Returns -1 if user does not exist
    if (find_prev_user(group, user_name) == NULL)
    {
        return -1;
    }
    else
    {
        //Checks to see if the first user is the matching user
        if (strcmp(prevUser->name, user_name) == 0)
        {
            userToAdjust = prevUser;
            firstUser = prevUser;
        }
        else
        {
            userToAdjust = prevUser->next;
        }
        //Allocates memory
        Xct *newXct = malloc(sizeof(Xct));
        //Error printed if creating pointer doesnt work
        if (newXct == NULL)
        {
            perror("Could not provide memory.");
            exit(0);
        }
        //Allocates memory for name
        int nameLength = strlen(user_name);
        newXct->name = malloc(nameLength + 1);
        //Error printed if name is not set properly
        if (newXct->name == NULL)
        {
            perror("Could not provide memory.");
            exit(0);
        }
        //Info for newXct set
        strncpy(newXct->name, user_name, nameLength);
        newXct->amount = amount;
        newXct->next = NULL;
        
        //If there are no transactions, the new transactions becomes the start of the list
        if(group->xcts == NULL)
        {
            group->xcts = newXct;
        }
        //New transaction is added to the front of the list since that is the order it came
        else
        {
            newXct->next = group->xcts;
            group->xcts = newXct;
        }
        
        //Add the new amount to the balance of the user
        userToAdjust->balance += amount;
        
        //Switches placement of the user based on the new balance
        if (userToAdjust->next != NULL) {
            if (userToAdjust->balance > userToAdjust->next->balance)
            {
                    userAfter = userToAdjust->next;
                    if(userToAdjust != prevUser)
                    {
                        prevUser->next = userAfter;
                    }
                    prevUser = userAfter;
                    newUserAfter = userAfter->next;
                    prevUser->next = userToAdjust;
                    userToAdjust->next = newUserAfter;
                    
                    //Changes initial pointer if the first in the list is being adjusted
                    if(userToAdjust == firstUser)
                    {
                        group->users = prevUser;
                    }

            }
        }
        return 0;
    }
    
}

/* Print to standard output the num_xct most recent transactions for the
* specified group (or fewer transactions if there are less than num_xct 
* transactions posted for this group). The output should have one line per 
* transaction that prints the name and the amount of the transaction. If 
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long nu_xct) {
    Xct *currentXct =  group->xcts;
    //Performs actions if the current xct exists
    if (currentXct != NULL)
    {
        //Loop while nu_xct recent transactions is not 0
        while(nu_xct != 0)
        {
            //Output while transactions exist
            if (currentXct != NULL) {
                printf("User name: %s. Amount: $%.2f.\n", currentXct->name, currentXct->amount);
                currentXct = currentXct->next;
            }
            nu_xct -=1;
        }
    }
}

/* Remove all transactions that belong to the user_name from the group's 
* transaction list. This helper function should be called by remove_user. 
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {
    Xct *xctToRemove = group->xcts;
    Xct *prevXct = xctToRemove;
    //Performs actions while there are transactions to remove
    while (xctToRemove != NULL)
    {
        //Remove transaction that belong to the user name
        if (strcmp(xctToRemove->name, user_name) == 0)
        {
            if(xctToRemove->next != NULL)
            {
                group->xcts = xctToRemove->next;
                xctToRemove->next = NULL;
            }
            else
            {
                prevXct->next = NULL;
            }
        }
        prevXct = xctToRemove;
        xctToRemove = xctToRemove->next;
    }
    //Frees space
    free(xctToRemove);
}

