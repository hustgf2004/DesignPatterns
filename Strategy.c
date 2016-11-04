#include <stdio.h>
#define true 1
#define false 0
typedef struct tag_FLYBEHAVIOR_ST{
    int (*pfly)();
}FLYBEHAVIOR_ST;

typedef struct tag_QUACK_ST{
    int (*pquack)();
}QUACK_ST;

typedef struct tag_SIMU_DUCK_ST{
    FLYBEHAVIOR_ST stFly;
    QUACK_ST stQuack;
    void (*pdisplay)();
    int (*pduckfly)(FLYBEHAVIOR_ST);
    int (*pduckquack)(QUACK_ST);
}SIMU_DUCK_ST;

int FlyBehavior(FLYBEHAVIOR_ST stFly)
{
    return stFly.pfly();
}
int QuackBehavior(QUACK_ST stQuack)
{
    return stQuack.pquack();
}
void MallardDuckDisplay()
{
    printf("\r\n I'm a mallard duck");
}
void ModelDuckDisplay()
{
    printf("\r\n I'm a model duck");
}
int FlyWithWings()
{
    printf("\r\n I'm flying!!\r\n");
    return true;
}

int FlyNoWay()
{
    printf("\r\n I can't fly");
    return true;
}

int Quack()
{
    printf("\r\n Quack\r\n");
    return true;
}

int Squack()
{
    printf("\r\n SQuack \r\n");
    return true;
}

int MuteQuack()
{
    printf("\r\n Silence\r\n");
}


int CreatMallardDuck(SIMU_DUCK_ST *pstDuck)
{
    if (NULL == pstDuck)
    {
        printf("\r\n pstDuck is NULL");
        return false;
    }
    pstDuck->stFly.pfly = FlyWithWings;
    pstDuck->stQuack.pquack = Quack;
    pstDuck->pdisplay = MallardDuckDisplay;
    pstDuck->pduckfly = FlyBehavior;
    pstDuck->pduckquack = QuackBehavior;   
    return true;
}

void main(void)
{
    SIMU_DUCK_ST stMallardDuck;
    CreatMallardDuck(&stMallardDuck);

    stMallardDuck.pduckfly(stMallardDuck.stFly);
    stMallardDuck.pduckquack(stMallardDuck.stQuack);
    stMallardDuck.pdisplay();
    
}





