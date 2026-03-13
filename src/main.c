#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 20000000UL

/*
 * PIC16F877A configuration bits for a common 20 MHz crystal setup.
 * Adjust these if your board uses a different clock or power strategy.
 */
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define SENSOR_LEFT      RB0
#define SENSOR_RIGHT     RB1

#define ENA              RC2
#define ENB              RC1
#define LEFT_MOTOR_IN1   RD0
#define LEFT_MOTOR_IN2   RD1
#define RIGHT_MOTOR_IN3  RD2
#define RIGHT_MOTOR_IN4  RD3

#define SENSOR_ON_LINE   0U
#define SENSOR_OFF_LINE  1U

typedef enum
{
    DIRECTION_NONE = 0,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} turn_direction_t;

static turn_direction_t last_turn = DIRECTION_NONE;

static void io_init(void);
static void motors_enable(void);
static void move_forward(void);
static void turn_left(void);
static void turn_right(void);
static void stop_motors(void);
static void recover_line(void);

static void io_init(void)
{
    ADCON1 = 0x06;   /* Make PORTA/PORTB digital where applicable */
    TRISB0 = 1;
    TRISB1 = 1;

    TRISC1 = 0;
    TRISC2 = 0;

    TRISD0 = 0;
    TRISD1 = 0;
    TRISD2 = 0;
    TRISD3 = 0;

    PORTC = 0x00;
    PORTD = 0x00;
}

static void motors_enable(void)
{
    ENA = 1;
    ENB = 1;
}

static void move_forward(void)
{
    LEFT_MOTOR_IN1 = 1;
    LEFT_MOTOR_IN2 = 0;
    RIGHT_MOTOR_IN3 = 1;
    RIGHT_MOTOR_IN4 = 0;
}

static void turn_left(void)
{
    LEFT_MOTOR_IN1 = 0;
    LEFT_MOTOR_IN2 = 0;
    RIGHT_MOTOR_IN3 = 1;
    RIGHT_MOTOR_IN4 = 0;
}

static void turn_right(void)
{
    LEFT_MOTOR_IN1 = 1;
    LEFT_MOTOR_IN2 = 0;
    RIGHT_MOTOR_IN3 = 0;
    RIGHT_MOTOR_IN4 = 0;
}

static void stop_motors(void)
{
    LEFT_MOTOR_IN1 = 0;
    LEFT_MOTOR_IN2 = 0;
    RIGHT_MOTOR_IN3 = 0;
    RIGHT_MOTOR_IN4 = 0;
}

static void recover_line(void)
{
    if (last_turn == DIRECTION_LEFT)
    {
        turn_left();
    }
    else if (last_turn == DIRECTION_RIGHT)
    {
        turn_right();
    }
    else
    {
        stop_motors();
    }

    __delay_ms(40);
}

void main(void)
{
    uint8_t left_state;
    uint8_t right_state;

    io_init();
    motors_enable();
    stop_motors();
    __delay_ms(500);

    while (1)
    {
        left_state = SENSOR_LEFT;
        right_state = SENSOR_RIGHT;

        if ((left_state == SENSOR_ON_LINE) && (right_state == SENSOR_ON_LINE))
        {
            move_forward();
            last_turn = DIRECTION_NONE;
        }
        else if ((left_state == SENSOR_ON_LINE) && (right_state == SENSOR_OFF_LINE))
        {
            turn_left();
            last_turn = DIRECTION_LEFT;
        }
        else if ((left_state == SENSOR_OFF_LINE) && (right_state == SENSOR_ON_LINE))
        {
            turn_right();
            last_turn = DIRECTION_RIGHT;
        }
        else
        {
            recover_line();
        }

        __delay_ms(10);
    }
}
