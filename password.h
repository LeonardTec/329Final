/*
 * password.h
 *
 *  Created on: May 30, 2023
 *      Author: Edgar A. Mora
 */

#ifndef INC_PASSWORD_H_
#define INC_PASSWORD_H_

// MACRO DEFINITIONS
#define PASSWORD_LENGTH	5

// Function Prototypes
void password_init(void);
int  password_type(void);
void password_update(void);
void password_welcome(void);
void password_display(int attempt, int lock);

#endif /* INC_PASSWORD_H_ */
