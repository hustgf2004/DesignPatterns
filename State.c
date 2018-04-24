#include <stdio.h>

int ghour = 0;
struct tag_State;
struct tag_Person;
typedef struct tag_Person{
    int (*pfGetHour)(void);
    void (*pfSetHour)(int);
    void (*pfSetState)(struct tag_Person *,struct tag_State *);
    struct tag_State *pStat;
}PERSON_S;

typedef struct tag_State{
    void (*pfDoSth)(struct tag_Person *);
}STATE_S;

STATE_S GetUpStat;
STATE_S HavLunchStat;
STATE_S HavDinnerStat;
STATE_S SleepStat;
PERSON_S person;

void SetHour(int hour)
{
  ghour = hour;
}

int GetHour(void)
{
  return ghour;
}
void SetState(struct tag_Person *Person,struct tag_State *pState)
{
   Person->pStat = pState;
} 
void GetUpdoing(struct tag_Person *pPerson)
{
  if (pPerson->pfGetHour() == 7)
  {
    printf("\r\nmorning !");
  }
  else 
  {
    pPerson->pfSetState(pPerson,&HavLunchStat);
    pPerson->pStat->pfDoSth(pPerson);
  }
}

void HaveLunchdoing(struct tag_Person *pPerson)
{
  if (pPerson->pfGetHour() == 11)
  {
    printf("\r\nhaving lunch!");
  }
  else
  {
    pPerson->pfSetState(pPerson,&HavDinnerStat);
    pPerson->pStat->pfDoSth(pPerson);
  }
}

void HaveDinnerdoing(struct tag_Person *pPerson)
{
  if (pPerson->pfGetHour() == 19)
  {
    printf("\r\nhaving dinner!");
  }
  else
  {
    pPerson->pfSetState(pPerson,&SleepStat);
    pPerson->pStat->pfDoSth(pPerson);
  }
}

void Sleepdoing(struct tag_Person *pPerson)
{
  if (pPerson->pfGetHour() == 22)
  {
    printf("\r\ngood night !");
  }
  else
  {
    pPerson->pfSetState(pPerson,&GetUpStat);
    pPerson->pStat->pfDoSth(pPerson);
  }
}


void initStat(void)
{
  GetUpStat.pfDoSth = GetUpdoing;
  HavLunchStat.pfDoSth = HaveLunchdoing;
  HavDinnerStat.pfDoSth = HaveDinnerdoing;
  SleepStat.pfDoSth = Sleepdoing;
}

void initPerson(void)
{
  person.pfGetHour = GetHour;
  person.pfSetHour = SetHour;
  person.pfSetState = SetState;
  person.pStat = &GetUpStat;
}



void main(void)
{
   initStat();
   initPerson();

   person.pfSetHour(7);
   person.pStat->pfDoSth(&person);

   person.pfSetHour(11);
   person.pStat->pfDoSth(&person);

   person.pfSetHour(19);
   person.pStat->pfDoSth(&person);

   person.pfSetHour(22);
   person.pStat->pfDoSth(&person);

   person.pfSetHour(7);
   person.pStat->pfDoSth(&person);
   printf("\r\n");
}
