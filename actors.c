typedef struct
{
    int health;         /* Always in integer: 1 - 100 - Each unit corresponds to half a heart */
    int buffer;         /* Each number points to a different buffer. Example: 1 = Wrong answer protection */
    int spriteState;    /* For now, we have 2 states: idle, typing number on the watch */
    int correctAnswers; /* Contains the number of correctly given answers */
    int score;          /* Contains the overall score of the player */

} Dex;

typedef struct
{
    /* The propperties of the Entity Slave follows the same pattern of the main character, Dex */
    int health;
    int spriteState;

} EntitySlave;

typedef struct
{
    /* The propperties of the Entity follows the same pattern of the main character, Dex */
    int health;
    int spriteState;

} Entity;
