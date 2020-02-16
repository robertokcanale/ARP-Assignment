#ifndef FUNCTIONS
#define FUNCTIONS

float R_Frequency; //maybe needed here

struct message_L {
  float sent_token, received_token;
};

void Write_Log_Received( float received_token); //maybe needs string[50]

void Write_Log_Sent( float sent_token);

void Write_Log (char string[50]);

void Print_Log ();

float New_Token(float, float, float );

void error(char *msg);

#endif // FUNCTIONS
