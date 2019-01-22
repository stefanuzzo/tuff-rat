#ifndef TR2COMM_H
#define TR2COMM_H

#define TR2_PRIO_PANIC			0
#define TR2_PRIO_HIGH			1
#define TR2_PRIO_NORM			2

#define TR2_CMD_PANIC			0
#define TR2_CMD_MOTORS_UP		1
#define TR2_CMD_MOTORS_DOWN	2
#define TR2_CMD_L_MOTOR_UP		3
#define TR2_CMD_L_MOTOR_DOWN	4
#define TR2_CMD_R_MOTOR_UP		5
#define TR2_CMD_R_MOTOR_DOWN	6

extern void send_command(unsigned int priority, int send_until_acknowledge, unsigned int commandType, const unsigned char *data, size_t data_size);
extern void send_single_byte_command(unsigned int priority, int send_until_acknowledge, unsigned int commandType, unsigned char data);


#endif // !TR2COMM_H
