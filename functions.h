#ifndef FUNCTIONS
#define FUNCTIONS


void Write_Log(const char, float  ); //maybe needs string[50]

void Write_Log_Sent( float sent_token);

float New_Token(float, float, float );

struct message_L {
  char from[5];
  float sent_token, received_token;
};

#endif // FUNCTIONS
